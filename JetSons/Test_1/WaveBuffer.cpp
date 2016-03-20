
#include "WaveBuffer.h"
#include <mutex>
#include <condition_variable>
#include "ThreadHelper.h"
#include "SafeQueue.h"



extern threadsafe_queue<WaveBuffer*> waveQueue;

WaveBuffer::WaveBuffer(const char* name,ThreadStatus * ts):wavename(name)
{ 
	//memset(this,0,sizeof(*this));
	wavebuffer = new char [BUFFER_SIZE];
	memset(wavebuffer,0,BUFFER_SIZE);
	
	status = WaveStatus::READY;
	bufferStatus = WaveBufferStatus::EMTY;
	next = 0;
	this->tStatus = ts;
	
	 
}

WaveBuffer::~WaveBuffer(){}

void WaveBuffer::WaveOutPlay()
{
	
	

	waveOutPrepareHeader(hWaveOut,&wh,sizeof(wh));
	 
	waveOutWrite(hWaveOut,&wh,sizeof(wh));
	
	
	do {}while (!(wh.dwFlags & WHDR_DONE));

	waveOutUnprepareHeader(hWaveOut,&wh,sizeof(wh));
	
	
	ClearBuffer();      
	
	
}

void WaveBuffer::Run()
{
	printf(" %s : (%llu) \n",wavename,GetCurrentID());
	while(1)
	{
	     
		
		 std::unique_lock<std::mutex> lc(wPlaymtx);
	     data_cond.wait(lc); 
		if(IsAlive())	 
		 WaveOutPlay();
		else
			break;
		
		
	   
	  
	}
	
}
void WaveBuffer::PrepareWaveHeader(const int size)
{
	wh.lpData = wavebuffer;
	wh.dwFlags = 0;
    wh.dwLoops = 0;
	wh.dwBufferLength = size;
}
char * WaveBuffer::getWaveBuffer()
{
	return wavebuffer;
}
void WaveBuffer::loadBuffer(const char* filebuffer,const int size)
{
	
	 memcpy(wavebuffer,filebuffer,size);
	 
	 bufferStatus =WaveBufferStatus::FULL;
	 PrepareWaveHeader(size);
	
	 
}
void WaveBuffer::Play()
{
	
	data_cond.notify_one();

}
bool WaveBuffer::IsFinished()
{
	if(status==WaveStatus::FINISHED)
		return true;
	else
		return false;
}
void WaveBuffer::sendRequest()
{
	if(tStatus->control_thread==CONTROLER_THREAD::WORKING)
	{
	waveQueue.push(this);
	
	bufferStatus =WaveBufferStatus::FULL;
	}
	//printf("WaveBUFFER Requested (%llu)\n",GetCurrentID());	
}
void WaveBuffer::ClearBuffer()
{
	
	
		bufferStatus =WaveBufferStatus::EMTY;
	

}
bool WaveBuffer::IsEmty()
{
	if(bufferStatus==WaveBufferStatus::EMTY)
	{
		return true;
	}
	else
		return false;
}

bool WaveBuffer::IsAlive()
{
	if(tStatus->playback_thread==PLAYBACK_THREAD::DONE && IsFinished())
	{
		printf("I realiesed %s \n",this->wavename);
		tStatus->releasebufferthreads();
	
		return false;
	}
	return true;
}

WaveFileFormat::WaveFileFormat()
{
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 22050;
	wfx.wBitsPerSample = 16;
	wfx.cbSize = 0;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
}
