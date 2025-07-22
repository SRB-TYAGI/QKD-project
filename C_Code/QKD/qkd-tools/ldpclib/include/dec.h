/* DEC.H - provides class ldpc_decoder with decoding procedures for ldpcMgr */

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


/* DECODING METHOD, ITS PARAMETERS, AND OTHER VARIABLES.  The global variables 
   declared here are located in dec.c. */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "alloc.h"
#include "mod2sparse.h"
#include "mod2dense.h"
#include "mod2convert.h"
#include "rcode.h"
#include "check.h"
#include "enc.h"


typedef enum 
{ Enum_block, Enum_bit, Prprp
} decoding_method;


class ldpc_decoder: public ldpc_encoder
{

public:

/* GLOBAL VARIABLES.  Declared in dec.h. */

decoding_method dec_method;	/* Decoding method to use */

int table;	/* Trace option, 2 for a table of decoding details */
int block_no;	/* Number of current block, from zero */

int max_iter;	/* Maximum number of iteratons of decoding to do */


/* PROCEDURES RELATING TO DECODING METHODS. */

void prprp_decode_setup(void);
unsigned prprp_decode(mod2sparse *, double *, char *, char *, double *);

void initprp (mod2sparse *, double *, char *, double *);
void iterprp (mod2sparse *, double *, char *, double *);

};