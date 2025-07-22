/* DevMgr.cpp - manages FPGA Device to collect data into queue */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 */

#include "DevMgr.h"
#include <string>
#include <fstream>

qkdtools::DevMgr::DevMgr()
{
	//getDataCmd=(unsigned char*)malloc(6 * sizeof(unsigned char));
	//setDelayCmd=(unsigned char*)malloc(6 * sizeof(unsigned char));

	//init command arrays
	for(int i=0;i<6;i++)
	{
		getDataCmd[i]=0;
		setDelayCmd[i]=0;
	}

	//init parameters
	fpgaHandle=NULL;
	skQueue=NULL;

}

void qkdtools::DevMgr::setQueue(qkdtools::KeyQueue *queue)
{
	this->skQueue=queue;
}

//open device connection, receive handle
bool qkdtools::DevMgr::openDev()
{
	fpgaHandle=fpgaIO::FPGA_Open();
	return (fpgaHandle != NULL);
}

//close device connection
bool qkdtools::DevMgr::closeDev()
{
	if(fpgaHandle==NULL)
	{
		return false;
	}
	
	fpgaIO::FPGA_Close(fpgaHandle);
	return true;
}

//write delay parameters and start device
bool qkdtools::DevMgr::setDelayAndStart(int *delays)
{
	//set stop command to stop device
	for(int i=1;i<6;i++)
	{
		setDelayCmd[i]=0;
	}
	setDelayCmd[0]=2; //stop

	//temporary buffer to receive data
	unsigned char buffer[65536];
	int bufferLength;

	//stop device
	fpgaIO::FPGA_Interface(fpgaHandle, setDelayCmd, buffer, bufferLength);

	//set delays
	for(int j=0;j<4;j++)
	{
		setDelayCmd[j]=128 + delays[j]; //128: [7] = 1 -> delay definition
	}
	setDelayCmd[4] = 128 + (delays[4]%128); //lower bits
	setDelayCmd[5] = 128 + (int)(delays[4] / 128); //upper bits

	//write delays to device
	fpgaIO::FPGA_Interface(fpgaHandle, setDelayCmd, buffer, bufferLength);

	//set command to start device
	for(int i=0;i<6;i++)
	{
		setDelayCmd[i]=0;
	}
	setDelayCmd[0]=4; //start

	//start device
	fpgaIO::FPGA_Interface(fpgaHandle, setDelayCmd, buffer, bufferLength);

	return true;
}

//stop device
bool qkdtools::DevMgr::stop()
{
	//set stop command
	for(int i=0;i<6;i++)
	{
		setDelayCmd[i]=0;
	}
	setDelayCmd[0]=2; //stop

	//temporary buffer to receive data
	unsigned char buffer[65536];
	int bufferLength;

	//stop device
	fpgaIO::FPGA_Interface(fpgaHandle, setDelayCmd, buffer, bufferLength);

	return true;
}

//read data from device into buffer
bool qkdtools::DevMgr::getData(unsigned char *buffer, int &bufferLength)
{
	if(fpgaHandle==NULL)
	{
		return false;
	}

	fpgaIO::FPGA_Interface(fpgaHandle, getDataCmd, buffer, bufferLength);

	return true;
}

//reads all data from device, so that it is empty - device should be stopped
bool qkdtools::DevMgr::clearDeviceDataStorage()
{
	if(fpgaHandle==NULL)
	{
		return false;
	}

	//temporary buffer to receive data
	unsigned char buffer[65536];
	int bufferLength;

	for(int i=0;i<4;i++) //four runs
	{
		Sleep(10);
	
		//get data from device
		fpgaIO::FPGA_Interface(fpgaHandle, getDataCmd, buffer, bufferLength);
	}

	//device should be empty now

	return true;
}

//start thread to continously read data from device into queue
bool qkdtools::DevMgr::startDataCollectThread()
{

	if(fpgaHandle==NULL || skQueue==NULL)
	{
		return false;
	}

	unsigned threadID;

	//configure thread communication object (parameters)
	myThreadCom.keepRunning=true;
	myThreadCom.device = this;
	myThreadCom.queue = this->skQueue;

	//start thread
	uintptr_t myDataCollectThread = _beginthreadex(NULL, 0, createSiftedKey, &myThreadCom, 0, &threadID);

	if(myDataCollectThread == NULL)
	{
		return false;
	}

	return true;

}

//stop data collection thread
bool qkdtools::DevMgr::stopDataCollectThread()
{
	//setting this the thread will exit
	myThreadCom.keepRunning=false;

	return true;
}

