/* ThreadMgr.h - manages threads and queues for post processing */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 *
 * Thread 0:
 * reads skQueue, delete invalid packets, put valid packets into validQueue
 *
 * Thread 1:
 * read validQueue, do error correction, put packets into ecQueue
 * 
 * Thread 2:
 * read ecQueue, do privacy amplification, put packets into finalQueue
 *
 * Usage:
 * First, one needs to set the parameters of the post processing:
 * - if this is Alice: setActAsAlice(.)
 * - the network data (IP, Ports): setNetworkData(.)
 * - the length of the privacy amplified key: setPAKeyLength(.)
 * - LDPC matrices for error correction: setLDPCMatFiles(.)
 * - input key queue of sifted key: setKeyQueue(.)
 * - output key queue, final key: setFinalKeyQueue(.)
 *
 * The three threads can then be started by calling
 * startThread(.) or stopped by calling stopThread(.)
 *
 */

#pragma once

#include "qkdtools.h"
#include <Windows.h>
#include <process.h>
#include "KeyQueue.h"
#include "NetworkMgr.h"
#include "../../ldpclib/include/ldpcMgr.h"

//declare thread functions
unsigned __stdcall deleteInvalidPackets(void * param);
unsigned __stdcall doErrorCorrection(void * param);
unsigned __stdcall doPrivacyAmplification(void * param);

namespace qkdtools
{
	class KeyQueue;
	class NetworkMgr;

	struct threadCom
	{
		qkdtools::KeyQueue *inQueue;
		qkdtools::KeyQueue *outQueue;
		bool keepRunning;
		qkdtools::NetworkMgr* netdevice;
		bool actAsAlice;
		qkdtools::ldpcMgr* ldpcmgr;
		int paKeyLength;
	};

	class _declspec(dllexport) ThreadMgr
	{
	public:
		ThreadMgr(bool actAsAlice);
		ThreadMgr();
		void initThreadMgr(bool actAsAlice);

		bool startThread(int id); //id=0,1,2
		bool stopThread(int id);

		//set queues
		void setKeyQueue(qkdtools::KeyQueue *queue);
		void setFinalKeyQueue(qkdtools::KeyQueue *queue);

		void setActAsAlice(bool actAsAlice);
		
		//we need a connection partner and three ports for the three threads to communicate parallel
		void setNetworkData(char* connectToIP, int Port1, int Port2, int Port3);
		void setPAKeyLength(int paKeyLen);
		void setLDPCMatFiles(char* pchk_filename, char* gen_filename);

		//empty queues: should only be called when the threads are stopped
		void clearAllQueues();
		void clearInternalQueues();

	private:

		bool startValidThread();
		bool startECThread();
		bool startPAThread();

		//helper functions
		bool readyToStart();
		bool startNetworkConnection(int id);
		void initThreadCom();

		bool actAsAlice;
		bool hasNetworkParam;

		int paKeyLength;
		char *pchk_file, *gen_file;

		qkdtools::NetworkMgr *netmgr[3]; //network mgrs
		threadCom myThreadCom[3]; //objects for thread communication

		qkdtools::KeyQueue *skQueue; //sifted key, IN for thread1
		qkdtools::KeyQueue *validQueue; //valid key, OUT of thread1, IN for thread2
		qkdtools::KeyQueue *ecQueue; //error corrected key, OUT of thread2, IN for thread3
		qkdtools::KeyQueue *finalQueue; //final key, with privacy amplification, OUT of thread3
		
	};

}
