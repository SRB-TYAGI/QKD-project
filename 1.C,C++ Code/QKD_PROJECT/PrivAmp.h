/* PrivAmp.h - generate Toeplitz Matrix and calculate final key (privacy amplification) */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 * Usage:
 * Creating a PrivAmp object one needs to specify the length of the
 * privacy amplified output key.
 * The next step is to gain a Toeplitz matrix by generating it:
 * generateToeplitzMat() or by setting it from a source:
 * setToeplitzMat(.)
 *
 * Now we can calculate an output key with calcPAKey(.)
 * and get a pointer to the result by getPAKey() or
 * copy the result to a bool array using copyPAKey(.) 
 *
 */


// #pragma once

// #include "qkdtools.h"
// #include <time.h>

// #define KEYLENGTH 1000

// namespace qkdtools
// {
// 	class PrivAmp
// 	{
// 	public:
// 		PrivAmp(int paKeyLength);
// 		~PrivAmp();

// 		//set the length of the ouput key
// 		void setPAKeyLength(int paKeyLength);

// 		//generate matrix
// 		void generateToeplitzMat();

// 		//set matrix
// 		void setToeplitzMat(char* toeplitzMat);

// 		//calculate privacy amplified output key
// 		void calcPAKey(bool* key);

// 		//get matrix / len
// 		char* getToeplitzMat();
// 		int getToeplitzMatLen();

// 		//get calculated output key / len
// 		char* getPAKey();
// 		int getPAKeyLen();

// 		//copy output key to a bool array
// 		void copyPAKey(bool* pakey);

// 	private:

// 		char* toepMat; // 0/1 - KEYLENGTH + paLen bits
// 		int toepMatLen;

// 		char* paKey; // 0/1 - paLen bits
// 		int paLen; //length of hash code

// 	};
// }










// main code


// #pragma once

// #include "qkdtools.h"
// #include <time.h>

// #define KEYLENGTH 1000

// namespace qkdtools
// {
// 	class PrivAmp
// 	{
// 	public:
// 		PrivAmp(int paKeyLength);
// 		~PrivAmp();

// 		// Set the length of the output key
// 		void setPAKeyLength(int paKeyLength);

// 		// Generate matrix
// 		void generateToeplitzMat();

// 		// Set matrix
// 		void setToeplitzMat(char* toeplitzMat);

// 		// Calculate privacy amplified output key (UPDATED to use char*)
// 		void calcPAKey(char* key);

// 		// Get matrix / len
// 		char* getToeplitzMat();
// 		int getToeplitzMatLen();

// 		// Get calculated output key / len
// 		char* getPAKey();
// 		int getPAKeyLen();

// 		// Copy output key to a char array (UPDATED from bool*)
// 		void copyPAKey(char* pakey);

// 	private:
// 		char* toepMat; // KEYLENGTH + paLen - 1 bits
// 		int toepMatLen;

// 		char* paKey;   // Output key (0/1) - paLen bits
// 		int paLen;
// 	};
// }













// dynamic key length code

#pragma once
#include <ctime>

namespace qkdtools {
class PrivAmp {
public:
PrivAmp(int paKeyLength);
~PrivAmp();


void setPAKeyLength(int paKeyLength, int keyLen);
void generateToeplitzMat();
void setToeplitzMat(char* toeplitzMat);
void calcPAKey(char* key, int keyLen);
char* getToeplitzMat();
int getToeplitzMatLen();
char* getPAKey();
int getPAKeyLen();
void copyPAKey(char* pakey);
private:
char* toepMat;
int toepMatLen;
char* paKey;
int paLen;
};
}