/* QkdMgr.h - manages all steps needed from FPGA to final key (text file) */


/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 *
 * Usage:
 * To configure the QkdMgr, one can either read in a config file using
 * readConfigFromFile(.) or provide the needed parameters manually:
 * - the delays for the FPGA device as int [5] array: setDelays(.)
 * - if this is Alice: setActAsAlice(.)
 * - the filename for the final key output: setPrintOutFilename(.)
 * - the network data (IP, Ports): setNetworkData(.)
 * - the length of the privacy amplified key: setPAKeyLength(.)
 * - LDPC matrices for error correction: setLDPCMatFiles(.)
 * If one specifies these parameters manually, one has to call 
 * initQkd() at the end.
 *
 * The QkdMgr can then be controlled by these commands:
 * bool start() - starting data collection and key post processing
 * bool stop() - stopping
 * bool reset() - resetting clears the queues and the device, use only when stopped!
 * bool close() - close device connection / print out file at the end
 *
 *
 */


#pragma once

#include "qkdtools.h"
#include "ThreadMgr.h"
#include <time.h>

//thread for bob to receive commands by Alice
unsigned __stdcall receiveCmds(void * param);

namespace qkdtools
{
	class ThreadMgr;
	class printOutKey;
	class DevMgr;
	class QkdMgr;
	class NetworkMgr;

	struct threadComBobCmd
	{
		qkdtools::QkdMgr* myQkdMgr;
		bool keepRunning;
		qkdtools::NetworkMgr* netdevice;
	};

	struct qkdConfigData
	{
		char* connectToIP;
		int Ports[4];

		int delays[5]; //four local delays, one global delay

		int paKeyLen;

		bool autoMode;

		bool actAsAlice;

		char* printOutFilename;

		char* pchk_filename;
		char* gen_filename;

	};

	class _declspec(dllexport) QkdMgr
	{
	public:
		QkdMgr();
		~QkdMgr();

		//set parameters
		void setDelays(int delays[5]);
		void setActAsAlice(bool actAsAlice);
		void setPrintOutFilename(char* printOutFilename);
		void setNetworkData(char* connectToIP, int Port1, int Port2, int Port3);
		void setPAKeyLength(int paKeyLen);
		void setLDPCMatFiles(char* pchk_filename, char* gen_filename);
		void initQkd(); //set parameters to sub classes

		void readConfigFromFile(char* filename);

		//commands
		bool start();
		bool stop();
		bool reset();
		bool close();

		qkdConfigData myCurConfig;
		qkdtools::KeyQueue *skQueue;

	private:

		bool checkReady();

		qkdtools::KeyQueue *finalKeyQueue;

		bool hasParam[6]; //0: actAsAlice, 1: delays 2: network param, 3: PALength, 4: LDPCMatFiles, 5: printOutFile
		
		bool initDone;
				
		qkdtools::DevMgr *myDevice;
		qkdtools::ThreadMgr *myThreadMgr;
		qkdtools::printOutKey *myPrintOut;
		qkdtools::NetworkMgr *myNetCmd;
		threadComBobCmd myCmdThreadCom;
		
	};
}

