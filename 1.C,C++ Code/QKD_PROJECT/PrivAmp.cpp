/* PrivAmp.cpp - generate Toeplitz Matrix and calculate final key (privacy amplification) */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 */

// #include "PrivAmp.h"

// qkdtools::PrivAmp::PrivAmp(int paKeyLength)
// {
// 	toepMat=NULL;
// 	paKey=NULL;
// 	setPAKeyLength(paKeyLength);
// }

// qkdtools::PrivAmp::~PrivAmp()
// {
// 	if(toepMat != NULL)
// 	{
// 		delete [] toepMat;
// 	}

// 	if(paKey != NULL)
// 	{
// 		delete [] paKey;
// 	}
// }

// //set length of output key and allocate memory for matrix and key
// void qkdtools::PrivAmp::setPAKeyLength(int paKeyLength)
// {
// 	if(toepMat != NULL)
// 	{
// 		delete [] toepMat;
// 	}

// 	if(paKey != NULL)
// 	{
// 		delete [] paKey;
// 	}

// 	paLen=paKeyLength;

// 	toepMatLen=KEYLENGTH + paLen - 1;

// 	toepMat = new char[toepMatLen];
// 	paKey = new char[paLen];

// 	srand(time(NULL));

// }

// //generate a random Toeplitz matrix
// void qkdtools::PrivAmp::generateToeplitzMat()
// {
// 	for(int i=0;i<toepMatLen;i++)
// 	{
// 		toepMat[i]=(rand()%2==0)?0:1;
// 	}
// }

// //calculate privacy amplified output key
// void qkdtools::PrivAmp::calcPAKey(bool* key)
// {
// 	//matrix multiplication
// 	for(int i=0;i<paLen;i++)
// 	{
// 		paKey[i]=0;
// 		for(int j=0;j<KEYLENGTH;j++)
// 		{
// 			int id=i-j+KEYLENGTH-1; //id of Toeplitz sequence for matrix element i,j
// 			paKey[i]+=toepMat[id] * key[j];
			
// 			//keep it binary (mod 2)
// 			paKey[i]=paKey[i]%2;
// 		}
// 	}
// }

// int qkdtools::PrivAmp::getPAKeyLen()
// {
// 	return paLen;
// }

// int qkdtools::PrivAmp::getToeplitzMatLen()
// {
// 	return toepMatLen;
// }

// char* qkdtools::PrivAmp::getToeplitzMat()
// {
// 	return toepMat;
// }

// char* qkdtools::PrivAmp::getPAKey()
// {
// 	return paKey;
// }

// //copy Toeplitz matrix from given source
// void qkdtools::PrivAmp::setToeplitzMat(char* toeplitzMat)
// {
// 	for(int i=0;i<toepMatLen;i++)
// 	{
// 		toepMat[i]=toeplitzMat[i];
// 	}
// }

// //copy calculated privacy amplified output key to given bool array
// void qkdtools::PrivAmp::copyPAKey(bool* pakey)
// {
// 	for(int i=0;i<paLen;i++)
// 	{
// 		pakey[i]=(this->paKey[i]==1)?1:0;
// 	}
// }











// main code



// #include "PrivAmp.h"

// qkdtools::PrivAmp::PrivAmp(int paKeyLength)
// {
// 	toepMat = nullptr;
// 	paKey = nullptr;
// 	setPAKeyLength(paKeyLength);
// }

// qkdtools::PrivAmp::~PrivAmp()
// {
// 	if (toepMat != nullptr) {
// 		delete[] toepMat;
// 	}

// 	if (paKey != nullptr) {
// 		delete[] paKey;
// 	}
// }

// void qkdtools::PrivAmp::setPAKeyLength(int paKeyLength)
// {
// 	if (toepMat != nullptr) {
// 		delete[] toepMat;
// 	}

// 	if (paKey != nullptr) {
// 		delete[] paKey;
// 	}

// 	paLen = paKeyLength;
// 	toepMatLen = KEYLENGTH + paLen - 1;

// 	toepMat = new char[toepMatLen];
// 	paKey = new char[paLen];

// 	srand(time(NULL));
// }

// void qkdtools::PrivAmp::generateToeplitzMat()
// {
// 	for (int i = 0; i < toepMatLen; i++) {
// 		toepMat[i] = (rand() % 2 == 0) ? 0 : 1;
// 	}
// }

// void qkdtools::PrivAmp::calcPAKey(char* key) // updated from bool* to char*
// {
// 	for (int i = 0; i < paLen; i++) {
// 		paKey[i] = 0;
// 		for (int j = 0; j < KEYLENGTH; j++) {
// 			int id = i - j + KEYLENGTH - 1; // Toeplitz matrix index
// 			paKey[i] += toepMat[id] * key[j];
// 			paKey[i] %= 2; // mod 2
// 		}
// 	}
// }

// int qkdtools::PrivAmp::getPAKeyLen()
// {
// 	return paLen;
// }

// int qkdtools::PrivAmp::getToeplitzMatLen()
// {
// 	return toepMatLen;
// }

// char* qkdtools::PrivAmp::getToeplitzMat()
// {
// 	return toepMat;
// }

// char* qkdtools::PrivAmp::getPAKey()
// {
// 	return paKey;
// }

// void qkdtools::PrivAmp::setToeplitzMat(char* toeplitzMat)
// {
// 	for (int i = 0; i < toepMatLen; i++) {
// 		toepMat[i] = toeplitzMat[i];
// 	}
// }

// void qkdtools::PrivAmp::copyPAKey(char* pakey) // updated from bool* to char*
// {
// 	for (int i = 0; i < paLen; i++) {
// 		pakey[i] = (this->paKey[i] == 1) ? 1 : 0;
// 	}
// }



// dynamic key length code

#include "PrivAmp.h"
#include <cstdlib>

qkdtools::PrivAmp::PrivAmp(int paKeyLength) {
toepMat = nullptr;
paKey = nullptr;
paLen = paKeyLength;
}

qkdtools::PrivAmp::~PrivAmp() {
delete[] toepMat;
delete[] paKey;
}

void qkdtools::PrivAmp::setPAKeyLength(int paKeyLength, int keyLen) {
delete[] toepMat;
delete[] paKey;
paLen = paKeyLength;
toepMatLen = keyLen + paKeyLength - 1;
toepMat = new char[toepMatLen];
paKey = new char[paLen];
std::srand(static_cast<unsigned>(time(nullptr)));
}

void qkdtools::PrivAmp::generateToeplitzMat() {
for (int i = 0; i < toepMatLen; i++) {
toepMat[i] = (std::rand() % 2 == 0) ? 0 : 1;
}
}

void qkdtools::PrivAmp::calcPAKey(char* key, int keyLen) {
for (int i = 0; i < paLen; i++) {
paKey[i] = 0;
for (int j = 0; j < keyLen; j++) {
int id = i - j + keyLen - 1;
if (id >= 0 && id < toepMatLen)
paKey[i] += toepMat[id] * key[j];
}
paKey[i] %= 2;
}
}

int qkdtools::PrivAmp::getPAKeyLen() {
return paLen;
}

int qkdtools::PrivAmp::getToeplitzMatLen() {
return toepMatLen;
}

char* qkdtools::PrivAmp::getToeplitzMat() {
return toepMat;
}

char* qkdtools::PrivAmp::getPAKey() {
return paKey;
}

void qkdtools::PrivAmp::setToeplitzMat(char* toeplitzMat) {
for (int i = 0; i < toepMatLen; i++) {
toepMat[i] = toeplitzMat[i];
}
}

void qkdtools::PrivAmp::copyPAKey(char* pakey) {
for (int i = 0; i < paLen; i++) {
pakey[i] = (this->paKey[i] == 1) ? 1 : 0;
}
}