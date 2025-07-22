/* DevMgr.h - manages FPGA Device to collect data into queue */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 * Usage: 
 * One needs to open the device connection: openDev(), then a call to 
 * setDelayAndStart(.) passing an int [5] array (0..3: relative delays, 
 * 4: global delay (max. 2 bytes)) will set the delays and start the
 * device.
 * A single call to getData will collect one set of data.
 *
 * If a key queue is set (setQueue(.)) then a separate thread can be
 * started to collect data and put it into the queue
 * startDataCollectThread()
 * 
 * For cleanup one should stop the thread by stopDataCollectThread(),
 * stop the device by calling stop() and then close the device
 * connection with closeDev()
 * 
 */

#pragma once


#include "qkdtools.h"
#include <Windows.h>
#include <process.h>
#include "KeyQueue.h"

//declare thread functions
unsigned __stdcall createSiftedKey(void * param);

namespace qkdtools
{
	class KeyQueue;
	class DevMgr;

	struct threadComDevMgr 
	{
		qkdtools::KeyQueue *queue;
		bool keepRunning;
		qkdtools::DevMgr* device;
	};

	class _declspec(dllexport) DevMgr
	{
	public:
		DevMgr();

		//connect to device / get Handle
		bool openDev();

		//close device connection
		bool closeDev();

		//write delays to device and start data collection (by device command)
		bool setDelayAndStart(int *delays);

		//stop data collection (by device command)
		bool stop();

		//read data from device into buffer (should be at least 65536 bytes long)
		bool getData(unsigned char *buffer, int &bufferLength);

		//reads all data from device, so that it is empty - device should be stopped
		bool clearDeviceDataStorage();

		//start / stop thread that continously collects data from FPGA
		//and writes the sifted key into a queue
		bool startDataCollectThread();
		bool stopDataCollectThread();

		void setQueue(qkdtools::KeyQueue *queue);

	private:
		HANDLE fpgaHandle;
		unsigned char getDataCmd[6];
		unsigned char setDelayCmd[6];
		threadComDevMgr myThreadCom;
		qkdtools::KeyQueue *skQueue;
		
	};
}
