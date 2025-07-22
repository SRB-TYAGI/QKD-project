/* printOutKey.h - print out the key packets from a given queue, using a separate thread */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 * Usage:
 * To create a printOutKey object one needs to pass a filename for the output file
 * as well as a key queue to take the key data from.
 * The thread can be started: startPrintOut() and
 * stopped: stopPrintOut() while at the end (cleanup) one should call
 * closeFile() to close the resulting output file.
 *
 */


#pragma once

#include "qkdtools.h"
#include <Windows.h>
#include <process.h>
#include "KeyQueue.h"

//declare thread functions
unsigned __stdcall printOutKeyQueue(void * param);

namespace qkdtools
{
	//structure to manage thread communication
	struct threadComPrintOut
	{
		//thread runs while this is true
		bool keepRunning;

		//key queue and filestream
		qkdtools::KeyQueue *queue;
		std::ofstream fileoutput;
	};

	class KeyQueue;

	class _declspec(dllexport) printOutKey
	{
	public:
		printOutKey(char* filename, qkdtools::KeyQueue *queue);

		//control thread
		bool startPrintOut();
		void stopPrintOut();

		//close file (cleanup)
		void closeFile();

	private:
		qkdtools::KeyQueue *queue;

		threadComPrintOut myThreadCom;
	};

}
