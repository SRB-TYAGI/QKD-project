/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 


This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// fpgaIO.cpp : FPGA-based Fast Multisource Pulse Registration System 
// Simple Labview Driver
// Copyright by Sergey Polyakov
//
// Modified by Pascal Notz, 2012
// 
// Simple FPGA-USB-Interface
//


#include "fpgaIO.h"
using namespace std;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
// Defines the entry point for the DLL application.					 
{
    return TRUE;
}


///////////////////////////////////////////////////
// Opens and closes the FPGA/USB driver
///////////////////////////////////////////////////
_declspec(dllexport) HANDLE fpgaIO::FPGA_Open()
{
    HANDLE XyloDeviceHandle;
	XyloDeviceHandle = CreateFile("\\\\.\\EzUSB-0", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (XyloDeviceHandle==INVALID_HANDLE_VALUE) return (HANDLE)-1;
	return XyloDeviceHandle;
}

_declspec(dllexport) void fpgaIO::FPGA_Close(HANDLE XyloDeviceHandle)
{
	CloseHandle(XyloDeviceHandle);
}
///////////////////////////////////////////////////
// FPGA/USB interface (raw functions)
///////////////////////////////////////////////////
int USB_BulkWrite(ULONG pipe, void* buffer, ULONG buffersize, HANDLE XyloDeviceHandle)
{
	DWORD nBytes;
	int result; 
    result = 
           DeviceIoControl(XyloDeviceHandle, 0x222051, &pipe, sizeof(pipe), buffer, buffersize, &nBytes, NULL);
    if (nBytes== buffersize && result==1)
    return 1; //ok
    else
    return -1; //error
}

DWORD USB_BulkRead(ULONG pipe, void* buffer, ULONG buffersize, HANDLE XyloDeviceHandle)
{
	DWORD nBytes;
	int result;
	result = DeviceIoControl(XyloDeviceHandle, 0x22204E, &pipe, sizeof(pipe), buffer, buffersize, &nBytes, NULL);
//	bool DIC (...) If the operation completes successfully, the return value is nonzero.
//  If the operation fails or is pending, the return value is zero. To get extended error information, call GetLastError.

	if (result !=1 ) return -1; //error
	return nBytes;
}


///////////////////////////////////////////////////
// Communicates with FPGA
// new: 8 Bit Events
///////////////////////////////////////////////////
_declspec(dllexport) int fpgaIO::FPGA_Interface (HANDLE XyloDeviceHandle, unsigned char * fpga_command, unsigned char *buffer, int &bufferLength)
{

#ifdef PRINTOUT_DEBUG_FILE
	char * filename=PRINTOUT_DEBUG_FILENAME;
	fstream myOutputfile(filename,fstream::app|fstream::out|fstream::binary);
#endif

    unsigned char *nbuffer;
    unsigned char command[6];

	if(fpga_command == NULL)
	{
		return 0;
	}

	command[0]=fpga_command[0];
	command[1]=fpga_command[1];
	command[2]=fpga_command[2];
	command[3]=fpga_command[3];
	command[4]=fpga_command[4];
	command[5]=fpga_command[5];

	static unsigned int bufferleft=0;
	int shortpacketflag;

	nbuffer = buffer;

    int nb_bytes_received;
		
	//step 1: write a command into FPGA and request data
	if ( USB_BulkWrite(2, &command, 6, XyloDeviceHandle)== -1 ) 
	{
			return 0; //error
	}

	//step 2: recieve 2 bytes defining the length of transmission
	if ( (nb_bytes_received = USB_BulkRead(5, nbuffer, 512, XyloDeviceHandle) )== -1)
	{
		return 0; //error
	}

	//calculate the length of data availible for download
	unsigned int size = (int)nbuffer[1]+256*(int)nbuffer[0];

	//because a full USB frame is 512 bytes, download an integer number of frames, and leave everthing else for a next session
	size += bufferleft;
	bufferleft = size%512;
	size -=bufferleft;	

	//however, if there was too little data availible (i.e. less than <512 bytes), the FPGA will force sending a short packet to keep us informed
	shortpacketflag = 0;
	if (!size) 
	{
		shortpacketflag = 1;
		size = 1024;
	}
		
	//step 3: recieve a bulk of data
    if ( (nb_bytes_received = USB_BulkRead(4, nbuffer, size, XyloDeviceHandle) )== -1) 
	{
		    return 0; //error
	}  

	if ( shortpacketflag )
	{
		shortpacketflag = 0;
		bufferleft -= nb_bytes_received;
	}		
	bufferLength=nb_bytes_received;

			
#ifdef PRINTOUT_DEBUG_FILE
	myOutputfile << "received " << nb_bytes_received << " Bytes!" << std::endl;

	for (int j=0; j<nb_bytes_received; j++)
	{
		/*
		if( (unsigned short)(buffer[j]) + 1 != (unsigned short)(buffer[j+1]) && ( (unsigned short)(buffer[j])!=255 || (unsigned short)(buffer[j+1])!=0 ) )
		{
			datei << "ERROR in byte " << j << std::endl;
		}*/
		myOutputfile << (unsigned short)(buffer[j]) << std::endl;
	}
#endif

	
#ifdef PRINTOUT_DEBUG_FILE
	myOutputfile.close();
#endif

    return 1; //no error
}
