#include "WaveFileReader.h"
#include "WaveBuffer.h"
#include <assert.h>
#include <stdio.h>
#include "ThreadHelper.h"

WaveFileReader::WaveFileReader(ThreadStatus * ts)
{
	buffer = new char [FILE_SIZE];
	fileInfo.remain=0;
	fileInfo.totalfilesize=0;
	fileInfo.lifeStatus = DATA_STATUS::MORE_TO_READ;
	isDataReady = false;
	file_index = 0;
	tStatus = ts;
}

WaveFileReader::~WaveFileReader(){}

void WaveFileReader::ReadWaveFile()
{
   if(file_index<=22)
   {
	FileHandle fh;
	FileError ferror;
	
	sprintf_s(file_name,"wave_%d.wav",file_index);

	ferror = File::open(fh,file_name,FILE_READ);
	
	ferror = File::seek(fh,FileSeek::FILE_SEEK_END,0);
	
	ferror = File::tell(fh,fileInfo.totalfilesize);
	ferror = File::seek(fh,FileSeek::FILE_SEEK_BEGIN,0);
	ferror = File::read(fh,buffer,fileInfo.totalfilesize);

	fileInfo.remain = fileInfo.totalfilesize;

	
	File::close(fh);
	SwitchStatus();

	file_index++;

   }
   else
	   fileInfo.lifeStatus = DATA_STATUS::FINISHED;

}

void WaveFileReader::Run()
{
	printf("File Reader (%llu)\n",GetCurrentID());
	while(IsAlive())
	{  
		if(!isDataReady)
		  ReadWaveFile();
		 std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	printf("File Reader Done");
}
bool WaveFileReader::IsDataReady()
{
	return isDataReady;
}
char *WaveFileReader::getBuffer()
{
	return buffer;
}
WaveFileInfo WaveFileReader::getFileInfo()
{
	return fileInfo;
}

void WaveFileReader::SwitchStatus()
{
	fmtx.lock();
	isDataReady = !isDataReady;
	fmtx.unlock();
}
void WaveFileReader::SendFile(WaveBuffer * wbuffer)
{
	
	while(wbuffer!=0)
	{
		//wbuffer->loadBuffer(buffer);
	   wbuffer=(WaveBuffer*)wbuffer->next;
	   buffer = buffer + BUFFER_SIZE;
	  fileInfo.remain = fileInfo.totalfilesize - BUFFER_SIZE;
	}
}

bool WaveFileReader::IsAlive()
{
	if(fileInfo.lifeStatus == DATA_STATUS::FINISHED)
		return false;
	else
		return true;
}