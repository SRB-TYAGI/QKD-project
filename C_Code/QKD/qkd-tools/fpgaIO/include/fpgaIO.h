/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 


This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/



// fpgaIO.h : FPGA-based Fast Multisource Pulse Registration System 
// Simple Labview Driver
// Copyright by Sergey Polyakov
//
// Modified by Pascal Notz, 2012
// 
// Simple FPGA-USB-Interface, defining:
//
// HANDLE fpgaIO::FPGA_Open()
//		returns an integer "handle" to the device. -1 if not successful.
//
// void fpgaIO::FPGA_Close(HANDLE XyloDeviceHandle)
//		Closes communication session.
//
// int fpgaIO::FPGA_Interface (HANDLE XyloDeviceHandle, unsigned char * fpga_command, unsigned char *buffer, int &bufferLength)
//		establishes the communication with the FPGA and returns data.
//				HANDLE XyloDeviceHandle: handle to FPGA device
//              char fpga_command: array of up to 6 bytes to send to FPGA. 
//					Possible commands: 00000000 no command
//									   00000001 clear timestamping counter (same as start)
//									   00000010 stop aquisition
//									   00000100 start aquisition
//									   10000111 set delay, 7 in this case (need to have 6 bytes in a row to set all delays)
//				unsigned char* buffer: needs to be an array of 65536 bytes length
//				int &bufferLength: will contain the number of Bytes received from FPGA
//		returns 1 if successful, 0 upon an error
//
// The computer communicates with the FPGA via a USB 2 port and using a standard ezusb driver.
// The FPGA is a slave USB device. The protocol consists of 
// 1) a request sent by a computer (sends one byte via FIFO2, which is used for a primitive control of FPGA, see documentation)
// 2) a response of FPGA (replies with 2 bytes via FIFO5, reports the length of collected information in bytes)
// 3) data of a total length reported in step 2 is then availible for download via FIFO4
//


#pragma once

#include "stdafx.h"

//set this switch to write out a debug file containing the data we get from the FPGA
//#define PRINTOUT_DEBUG_FILE
//#define PRINTOUT_DEBUG_FILENAME "e:\\fpgaout.txt"


namespace fpgaIO
{
_declspec(dllexport) HANDLE FPGA_Open();
_declspec(dllexport) void FPGA_Close(HANDLE XyloDeviceHandle);
_declspec(dllexport) int FPGA_Interface (HANDLE XyloDeviceHandle, unsigned char * fpga_command, unsigned char *buffer, int &bufferLength);
}
