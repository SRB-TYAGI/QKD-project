/* KeyQueue.h - key queue and key queue packet to store key parts, FIFO queue */

/*
Control software for the quantum key distribution experiment, developed and used in the Laser and Quantum Optics group (LQO) within the Technische Universität Darmstadt, Germany. 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Copyright (c) 2012 by Pascal Notz
 * Usage: 
 * create packet with valid bit, push into queue
 * 
 * if there is only one packet in a queue, pop will return NULL
 * and the packet will stay there, so that push only changes 
 * *last and pop only changes *first 
 * -> parallel usage possible and fast (without mutex)
 *
 */

#pragma once

#include "qkdtools.h"

namespace qkdtools
{
	class _declspec(dllexport) KeyQueuePacket
	{
	public:
		//constructor methods
		KeyQueuePacket();
		KeyQueuePacket(bool valid);

		//returns true if valid bit is set
		bool isValid();
		
		//pointer to next packet in queue
		KeyQueuePacket* getNext();
		void setNext(KeyQueuePacket* n);

		//data storage: bool array
		bool* getData();
		void setData(bool *d);
		
		//length of data array
		void setLength(int length);
		int length();

		//calculated quantum bit error rate for packet
		void setQBER(float qber);
		float getQBER();

	private:
		//called by constructor
		void initQueuePacket(bool valid);

		bool valid;
		bool *data;
		int len;
		float qber;
		KeyQueuePacket* next;
	};

	class _declspec(dllexport) KeyQueue
	{
	public:
		KeyQueue();
		
		//push packet into queue, pop (FIFO)
		void pushPacket(qkdtools::KeyQueuePacket* p);
		qkdtools::KeyQueuePacket* popPacket();

		//clear queue
		void clear();

	private:
		qkdtools::KeyQueuePacket* first;
		qkdtools::KeyQueuePacket* last;
	};

}
