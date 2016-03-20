#ifndef WAVE_FILE_READER_H
#define WAVE_FILE_READER_H
#include "File.h"
#include <mutex>
#include "ThreadStatus.h"
#define FILE_SIZE 512*1024

class WaveBuffer;


 enum class DATA_STATUS
{
	MORE_TO_READ,
	FINISHED

};
struct WaveFileInfo
{
	 int totalfilesize;
	 int remain;
	 DATA_STATUS lifeStatus;
};
class WaveFileReader
{
public:
	WaveFileReader(ThreadStatus * ts);
	~WaveFileReader();
	
	void Run();
	void ReadWaveFile();
	void SendFile(WaveBuffer * wbuffer );
	void SwitchStatus();
	bool IsDataReady();
	bool IsAlive();
	char * getBuffer();
	WaveFileInfo getFileInfo();
	
private:
	WaveFileInfo fileInfo;
	int  file_index;
	char file_name[12];
	ThreadStatus * tStatus;
	std::mutex fmtx;
	char* buffer;
	bool isDataReady;
	
};




#endif