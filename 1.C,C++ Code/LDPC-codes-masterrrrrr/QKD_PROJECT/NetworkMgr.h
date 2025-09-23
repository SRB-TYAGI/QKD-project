/* NetworkMgr.h - manages network connections and sending/receiving data */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 * Usage:
 * create NetworkMgr, with socketType: 0 undef, 1 client, 2 server
 * Then call setNetworkParam(.) to set parameters.
 * For Server:
 * call bindSocket() to wait for and accept incoming connections
 * 
 * For Client:
 * call connectSocket() to connect to a server
 * 
 * sendData(.) with char array of data, and len=len of array (data)
 * after the call, len will contain the number of sent bytes
 *
 * receiveData(.) with char array as buffer, len must be the buffer length
 * after the call, len will contain the number of received bytes
 *
 * receiveDataOfLen(.) similar to receiveData, but it will keep on calling
 * receiveData until it has received the defined number of bytes
 *
 * Cleanup: closeSocket()
 *
 */

#pragma once

#include "qkdtools.h"
#include <Windows.h>
#include <winsock2.h>
#include <fstream>
#include <string>
#include <iostream>

namespace qkdtools
{

	class _declspec(dllexport) NetworkMgr
	{
	public:
		NetworkMgr();
		NetworkMgr(int socketType);
		~NetworkMgr();

		//client: connect to server
		bool connectSocket();

		//server: bind to port and accept connections
		bool bindSocket();

		//close socket
		void closeSocket();

		bool sendData(char *data, int &len);
		bool receiveData(char *data, int &len);
		bool receiveDataOfLen(char *data, int len);

		//splits data into packets and transmits them / DEPR
		void sendDataInPackets(char *data, int len, char &retVal);
		void receiveDataInPackets(char *data, int len, char &retVal);

		//set parameters for network connection
		void setNetworkParam(char* connectToIP, int connectToPort, int bindToPort);

	private:
		//called by constructor
		void initNetworkMgr(int socketType);

		short socketType; //0 undef, 1 client, 2 server
		bool wsockInitOk;
		bool wsockConnected;

		WSADATA wsa;
		SOCKET wSocket;
		SOCKET svrSocket;
		SOCKADDR_IN wAddress; 

		char* connectToIP;
		int connectToPort;
		int bindToPort;
		
	};

}
