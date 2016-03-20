#ifndef WAVE_BUFFER_H
#define WAVE_BUFFER_H
#include <Windows.h>
#include <mutex>
#include <condition_variable>
#include "ThreadStatus.h"
#define BUFFER_SIZE 2*1024

struct WaveFileFormat
{
	WaveFileFormat();
	
	WAVEFORMATEX wfx;
};
enum class WaveStatus
{
	READY,
	WAITING,
	FINISHED,
	EMTY
};
enum class WaveBufferStatus
{
	EMTY,FULL
};


class WaveBuffer 

{
public:
	WaveBuffer(const char * name,ThreadStatus * ts);
	~WaveBuffer();
	void Run();
	void WaveOutPlay();
	void PrepareWaveHeader(const int size);
	char * getWaveBuffer();
	void loadBuffer(const char * filebuffer,const int size);
	void Play();
	void sendRequest();
	bool IsFinished();
	void ClearBuffer();
	bool IsEmty();
	bool IsAlive();
public :
	HWAVEOUT hWaveOut;
	WaveBuffer * next;
    WaveFileFormat waveformat;
    WaveStatus status;
	WaveBufferStatus bufferStatus; 
	WAVEHDR  wh;
	ThreadStatus  *tStatus;
	std::condition_variable  waveout_write_cond;
	const char*  wavename;

private:
	
	 
	 MMRESULT  mmr;
	 std::condition_variable data_cond;
	 std::mutex wPlaymtx;
	 char*  wavebuffer;

};




#endif