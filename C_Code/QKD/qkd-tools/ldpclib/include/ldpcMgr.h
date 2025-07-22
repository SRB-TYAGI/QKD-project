/* ldpcMgr.h - providing class to apply LDPC error correction */

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
 *
 *
 * Usage: 
 * Create ldpcMgr with filenames to a pchk_Matrix and a gen_Matrix.
 * These Matrix files can be created by using the programs of 
 * Radford M. Neal, example:
 * make-ldpc.exe winldpc.pchk 1000 2000 1 evenboth 3 no4cycle
 * make-gen.exe winldpc.pchk winldpc.gen dense
 * 
 * Encoding: call encode(.) with a pointer to a char array
 * providing the message data of length N-M (defined by the
 * pchk / gen matrices)
 * The resulting parity data is given by getParityData(),
 * returning a char array of length N
 *
 * Decoding: first one should set the estimated QBitErrorRate
 * by calling setErrorProbabilityForDec(.)
 * Then call decodeParityAndData(.) with pointers to a
 * char array of length N as parity and of length N-M as 
 * message data. It returns true if the decoding was
 * successful.
 * The resulting corrected message data is given by
 * getDecodedData(), which returns a char array of length N
 * 
 */

#pragma once

#include "rcode.h"
#include "mod2dense.h"
#include "mod2sparse.h"
#include "alloc.h"
#include "enc.h"
#include "dec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <time.h>

namespace qkdtools
{
	typedef enum { BSC, AWGN, AWLN } channel_type;

	class ldpcMgr
	{
	public:
		ldpcMgr(char* pchk_filename, char* gen_filename);
		//for encoding: give source of length N-M
		bool encode(char* src);
		//export only parity data
		char* getParityData();

		//for decoding: give parity of length N (with 0s where data should be)
		//and data to merge to codeword
		//returns true if decoding was successful (decoded data is valid)
		bool decodeParityAndData(char* parity, char* data);
		//export only data
		char* getDecodedData();

		//set the approximated error probability for decoding: default=0.16
		void setErrorProbabilityForDec(double error_prob);
		//set the max number of iterations for decoding: default=50
		void setMaxNrOfIterations(int max_it);

		int getCodewordLength();

	private:

		bool initEncoder();
		bool initDecoder();

		bool ready;

		unsigned int iters; //stores number of iterations needed (by decode)
		int valid; //stores 1 if decoding resulted in a valid block, 0 otherwise

		char *pchk_file, *gen_file;
		mod2dense *u, *v;
		char *sblk, *cblk, *chks;

		char **meth;

		char *dblk, *pchk;
		double *lratio;
		double *bitpr;

		int *bsc_data;

		double error_prob;	/* Error probability for BSC */

		channel_type channel;

		ldpc_encoder myEncoder;
		ldpc_decoder myDecoder;

	};
}