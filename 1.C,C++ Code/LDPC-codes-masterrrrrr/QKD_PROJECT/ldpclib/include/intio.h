/* INTIO.H - Interface for reading and writing integers one byte at a time. */

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

// #pragma once

// #include <stdio.h>
// #include <stdlib.h>

// int  intio_read  (FILE *);	/* Read an integer */
// void intio_write (FILE *, int);	/* Write an integer */


#ifndef INTIO_H
#define INTIO_H

#include <stdio.h>

int intio_read(FILE*);
void intio_write(FILE*, int);

// Add this:
FILE* open_file_std(char*, char*);

#endif