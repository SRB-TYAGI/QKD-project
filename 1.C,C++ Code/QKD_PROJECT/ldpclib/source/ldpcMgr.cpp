/* ldpcMgr.cpp - providing class to apply LDPC error correction */

/* Copyright (c) 1995-2012 by Radford M. Neal.
 *
 * Changes: 
 *			- Assembled parts of the C-Code into a C++ Library ldpclib
 *			  supporting only BSC channel
 *			- Interface supplied by qkdtools::ldpcMgr (ldpcMgr.h)
 * Copyright (c) 2012 by Pascal Notz
 *
 * Permission is granted for anyone to copy, use, modify, and distribute
 * these programs and accompanying documents for any purpose, provided
 * this copyright notice is retained and prominently displayed, and note
 * is made of any changes made to these programs.  These programs and
 * documents are distributed without any warranty, express or implied.
 * As the programs were written for research purposes only, they have not
 * been tested to the degree that would be advisable in any important
 * application.  All use of these programs is entirely at the user's own
 * risk.
 */

#include "../include/ldpcMgr.h"

qkdtools::ldpcMgr::ldpcMgr(char* pchk_filename, char* gen_filename)
{
	pchk_file = pchk_filename; // "winldpc.pchk";
	gen_file = gen_filename; //"winldpc.gen";

	ready=false;

	if(!initEncoder())
	{
		return;
	}

	if(!initDecoder())
	{
		return;
	}

	ready=true;
}

int qkdtools::ldpcMgr::getCodewordLength()
{
	return myEncoder.N;
}

bool qkdtools::ldpcMgr::initEncoder()
{
	/* Read parity check file */
	myEncoder.read_pchk(pchk_file);

	if (myEncoder.N<=myEncoder.M)
	{	
		//"Can't encode if number of bits (%d) not greater than number of checks (%d)\n", myEncoder.N,myEncoder.M
		return false;
	}

	/* Read generator matrix file. */
	myEncoder.read_gen(gen_file,0,0);

	/* Allocate needed space. */
	if (myEncoder.type=='d')
	{ u = mod2dense_allocate(myEncoder.N-myEncoder.M,1);
	v = mod2dense_allocate(myEncoder.M,1);
	}

	if (myEncoder.type=='m')
	{ u = mod2dense_allocate(myEncoder.M,1);
	v = mod2dense_allocate(myEncoder.M,1);
	}

	sblk = (char*) chk_alloc (myEncoder.N-myEncoder.M, sizeof *sblk);
	cblk = (char*) chk_alloc (myEncoder.N, sizeof *cblk);
	chks = (char*) chk_alloc (myEncoder.M, sizeof *chks);

	return true;
}

bool qkdtools::ldpcMgr::initDecoder()
{
	//set channel to BSC
	channel= BSC;

	//set decoder method
	myDecoder.dec_method = Prprp;

	//QBER 
	error_prob= 0.16;

	//max number of iterations
	myDecoder.max_iter = 50;

	/* Read parity check file. */
	myDecoder.read_pchk(pchk_file);

	if (myDecoder.N<=myDecoder.M)
	{ 
		//"Number of bits (%d) should be greater than number of checks (%d)\n",myDecoder.N,myDecoder.M);
		return false;
	}

	/* Allocate space for data from channel. */

	switch (channel)
	{ 
		case BSC:
		{ 
			bsc_data = (int*) chk_alloc (myDecoder.N, sizeof *bsc_data);
			break;
		}
		default:
		{ 
			return false;
		}
	}

  /* Allocate other space. */

  dblk   = (char*) chk_alloc (myDecoder.N, sizeof *dblk);
  lratio = (double*) chk_alloc (myDecoder.N, sizeof *lratio);
  pchk   = (char*) chk_alloc (myDecoder.M, sizeof *pchk);
  bitpr  = (double*) chk_alloc (myDecoder.N, sizeof *bitpr);

  /* Do the setup for the decoding method. */

  switch (myDecoder.dec_method)
  { case Prprp: 
    { myDecoder.prprp_decode_setup();
      break;
    }
    default: return false;
  }

  return true;

}

void qkdtools::ldpcMgr::setErrorProbabilityForDec(double error_prob)
{
	this->error_prob=error_prob;
}

void qkdtools::ldpcMgr::setMaxNrOfIterations(int max_it)
{
	this->myDecoder.max_iter=max_it;
}

bool qkdtools::ldpcMgr::encode(char* src)
{
	if(!ready)
	{
		return false;
	}

	//copy source data for encoding
	for(int i0=0;i0<myEncoder.N-myEncoder.M;i0++)
	{
		sblk[i0]=src[i0];
	}

	/* Compute encoded block. */

	switch (myEncoder.type)
	{ 
		case 's':
		{ 
			myEncoder.sparse_encode (sblk, cblk);
			break;
		}
		case 'd':
		{ 
			myEncoder.dense_encode (sblk, cblk, u, v);
			break;
		}
		case 'm':
		{ 
			myEncoder.mixed_encode (sblk, cblk, u, v);
			break;
		}
	}


	/* Check that encoded block is a code word. */

	mod2sparse_mulvec (myEncoder.H, cblk, chks);

	for (int i = 0; i<myEncoder.M; i++) 
	{ 
		if (chks[i]==1)
		{ 
			printf("Output block is not a code word!  (Fails check %d)\n",i);
			return false;
		}
	}

	  
	//fprintf(stderr,
    //"Encoded %d blocks, source block size %d, encoded block size %d\n",n,myEncoder.N-myEncoder.M,myEncoder.N);

	return true;
}

char* qkdtools::ldpcMgr::getParityData()
{
	//allocate memory for parity data
	char* par   = (char*) chk_alloc (myEncoder.N, sizeof(char));

	//first copy all data
	for(int i0=0;i0<myEncoder.N;i0++)
	{
		par[i0]=cblk[i0];
	}

	//then del src data
	for(int i0=myEncoder.M;i0<myEncoder.N;i0++)
	{
		par[myEncoder.cols[i0]]=0;
	}

	return par;
}

char* qkdtools::ldpcMgr::getDecodedData()
{
	//allocate memory for decoded data
	char* dec   = (char*) chk_alloc (myDecoder.N-myDecoder.M, sizeof(char));

	//copy data
	for(int i0=myDecoder.M;i0<myDecoder.N;i0++)
	{
		dec[i0-myDecoder.M]=dblk[myEncoder.cols[i0]];
	}

	return dec;
}

//returns true if decoding was successful, false otherwise
bool qkdtools::ldpcMgr::decodeParityAndData(char* parity, char* data)
{
	if(!ready)
	{
		return false;
	}

	//merge data: first copy parity data
	for(int i0=0;i0<myDecoder.N;i0++)
	{
		bsc_data[i0]=parity[i0];
	}

	//then add src data
	for(int i0=myDecoder.M;i0<myDecoder.N;i0++)
	{
		bsc_data[myEncoder.cols[i0]]=data[i0-myDecoder.M];
	}


    /* Find likelihood ratio for each bit. */

    switch (channel)
    { case BSC:
      { 
		//set values for checksum data without errors
		for (int i = 0; i<myDecoder.N; i++)
        { lratio[i] = bsc_data[i]==1 ? 0xFFFFFF //infinity for p=0
                                     : 0; //zero for p=0
        }

		//set errorprob for src data
		for(int i0=myDecoder.M;i0<myDecoder.N;i0++)
		{
			lratio[myEncoder.cols[i0]] = bsc_data[myEncoder.cols[i0]]==1 ? 
											(1-error_prob) / error_prob
											: error_prob / (1-error_prob);
		}
        break;
      }
      default: return false;
    }

    /* Try to decode using the specified method. */
	
    switch (myDecoder.dec_method)
    { case Prprp:
      { unsigned int iters = myDecoder.prprp_decode (myDecoder.H, lratio, dblk, pchk, bitpr);
        break;
      }
      default: return false;
    }

  /* Return true if it worked, false otherwise */
  return (check(myDecoder.H,dblk,pchk)==0);

}