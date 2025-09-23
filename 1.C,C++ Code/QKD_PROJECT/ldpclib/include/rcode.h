/* RCODE.H - Parity chk and gen matrix storage, and procedures to read them.*/

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

#pragma once

#include "mod2dense.h"
#include "mod2sparse.h"

#include "alloc.h"
#include "intio.h"
#include "open.h"
#include "mod2sparse.h"
#include "mod2dense.h"
#include "mod2convert.h"


class rCode
{

public:
	/* VARIABLES DECLARED IN RCODE.H.  These global variables are set to
   representations of the parity check and generator matrices by read_pchk
   and read_gen. */

/* PROCEDURES FOR READING DATA. */

void read_pchk (char *);
void read_gen  (char *, int, int);

int M;			/* Number of rows in parity check matrix */
int N;			/* Number of columns in parity check matrix */

char type;		/* Type of generator matrix representation (s/d/m) */
int *cols;		/* Ordering of columns in generator matrix */

mod2sparse *H;		/* Parity check matrix */

protected:

mod2sparse *L, *U;	/* Sparse LU decomposition, if type=='s' */
int *rows;		/* Ordering of rows in generator matrix (type 's') */

mod2dense *G;		/* Dense or mixed representation of generator matrix,
			   if type=='d' or type=='m' */


};