#include "WaveControler.h"
#include "WaveFileReader.h"
#include <thread>
#include "ThreadHelper.h"
#include "SafeQueue.h"
#include "WaveBuffer.h"
#include <memory.h>
#include "ThreadStatus.h"
#include <queue>
threadsafe_queue<WaveBuffer*> waveQueue;

WaveControler::WaveControler( WaveFileReader *filereader,ThreadStatus * ts)
{
	active = &bufferA;
	passive =&bufferB;
	bufferA.name = "bufferA";
	bufferB.name = "bufferB";
	this->freader = filereader;
	tStatus = ts;
}

WaveControler::~WaveControler()
{
}

void WaveControler::Run()
{
	printf("Controler (%llu)\n",GetCurrentID());
	FirstLoad();
	while(IsThereData()==true)
	{
		
		LoadBuffer();
		SendBufferData();
	}
	printf("Controler Done");

}


void WaveControler::SendBufferData()
{  
	char * p = active->buffer;

	if(active->status==ControlBufferStatus::READY){	
		
		while(active->remain>0)
		{
			
			if(!waveQueue.empty())
			{
			 std::shared_ptr<WaveBuffer*> b= waveQueue.try_pop();
				//WaveBuffer* b =  ( waveque.front());
			   // waveque.pop();
			 if(active->remain<BUFFER_SIZE)
			 {
				 (*b)->loadBuffer(active->buffer,(active->remain));
				
				 active->buffer += active->remain;
				 active->remain-= active->remain;
			 }
			 else 
			 {
				 (*b)->loadBuffer(active->buffer,BUFFER_SIZE);
				active->buffer += BUFFER_SIZE;
				active->remain -= BUFFER_SIZE;
			 }
			}
			
		}
		active->buffer = p;
		active->status = ControlBufferStatus::EMTY;
		//printf("Data sended %s  size :%d \n",active->name,active->size);	
		SwapBuffers();
	}
	if(passive->status == ControlBufferStatus::DONE)
	{
		active->status= ControlBufferStatus::DONE;
		
	}

}

void WaveControler::SwapBuffers()
{
	
	ControlBuffer* tmp = active;
	active = passive;
	passive = tmp;
	//printf("Buffers Swapped\n");
}
void WaveControler::FirstLoad()
{
	while(1)
	{
	   if(freader->IsDataReady())
		  break;
	}
		
	    memcpy(bufferA.buffer,freader->getBuffer(),freader->getFileInfo().totalfilesize);
		bufferA.size = freader->getFileInfo().totalfilesize;
		freader->SwitchStatus();
		bufferA.remain= bufferA.size;
		bufferA.status = ControlBufferStatus::READY;
	while(1){
	   if(freader->IsDataReady())
		break;
	    }

	 memcpy(bufferB.buffer,freader->getBuffer(),freader->getFileInfo().totalfilesize);
		bufferB.size = freader->getFileInfo().totalfilesize;
		freader->SwitchStatus();
		bufferB.remain= bufferB.size;
		bufferB.status = ControlBufferStatus::READY;

}

void WaveControler::LoadBuffer(){
   if(freader->getFileInfo().lifeStatus==DATA_STATUS::MORE_TO_READ){
	  
	   if(passive->status==ControlBufferStatus::EMTY)
	   {
		if(freader->IsDataReady())
		{
		
		memcpy(passive->buffer,freader->getBuffer(),freader->getFileInfo().totalfilesize);
		passive->size = freader->getFileInfo().totalfilesize;
		freader->SwitchStatus();
		passive->remain= passive->size;
		passive->status = ControlBufferStatus::READY;
		//printf("Data requested from (%llu) size :%d  \n",GetCurrentID(),active->size);	
		}
	 }
   }
   else
	   passive->status=ControlBufferStatus::DONE;
	  
}
ControlBuffer::ControlBuffer()
{

	buffer = new char [FILE_SIZE];
	size = FILE_SIZE;
	remain =size;
	status = ControlBufferStatus::EMTY;

}
bool WaveControler::IsThereData()
{
	 if( passive->status == ControlBufferStatus::DONE &&
		active->status == ControlBufferStatus::DONE)
	 {
			
			this->tStatus->control_thread = CONTROLER_THREAD::DONE;
			return false;
	 }
	 else
		 return true;

}
