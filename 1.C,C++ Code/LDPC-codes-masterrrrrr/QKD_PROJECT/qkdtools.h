/* qkdtools.h - provide functions for FPGA delay optimization */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 *
 * Usage:
 * call optimizeGlobalDelay(.) with handle if device connection is already
 * open, with int [5] array (0..3 local delays, have to be set!)
 * and a delay guess.
 * It will test [guess-5, guess+5] to find optimal delay configuration
 * The global delay which results in the highest ratio of ok events to total 
 * events recorded by the device will be returned.
 *
 */


#pragma once

// #include "stdafx.h"
// #include "../../fpgaIO/include/fpgaIO.h"
// // #include "DevMgr.h"
// #include "NetworkMgr.h"
#include "ThreadMgr.h"
#include "QkdMgr.h"
#include "KeyQueue.h"
#include "PrivAmp.h"
// #include "printOutKey.h"
// #include "ldpcMgr.h"
// #include "../../ldpclib/include/ldpcMgr.h"
// #include "C:/Users/soura/Desktop/New folder/ldpclib/include/ldpcMgr.h"
#include "E:\QKD_PROJECT\ldpclib\include\ldpcMgr.h"




#define BLOCKLENGTH 100 //length of bit blocks separated by special signal

//set this switch to write out a debug file containing the data we get from the FPGA
//#define PRINTOUT_DEBUG_FILE
//#define PRINTOUT_DEBUG_FILENAME "e:\\fpgaout2.txt"


namespace qkdtools
{

_declspec(dllexport) int optimizeGlobalDelay(int *localDelays, int DelayGuess);
_declspec(dllexport) int optimizeGlobalDelay(HANDLE XyloDeviceHandle, int *localDelays, int DelayGuess);
_declspec(dllexport) bool runDataCollectOnce(int *delays);

}
