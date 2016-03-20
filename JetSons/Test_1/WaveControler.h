#ifndef WAVE_CONTROLER_H
#define WAVE_CONTROLER_H

#include "ThreadStatus.h"
class WaveFileReader;

enum class ControlBufferStatus
{
	EMTY,
	READY,
	WATING,
	DONE
};

struct ControlBuffer
{
	ControlBufferStatus status;
	char* name;
	char* buffer;
	int size;
	int remain;
    ControlBuffer();
};
class WaveControler

{
public:
	WaveControler( WaveFileReader *filereader,ThreadStatus * ts);
	~WaveControler();
	void Run();
	void LoadBuffer();
	void SendBufferData();
	void SwapBuffers();
	void FirstLoad();
	bool IsThereData();
private:
	ControlBuffer bufferA;
	ControlBuffer bufferB;
	ControlBuffer *active;
	ControlBuffer *passive;
	WaveFileReader *freader;
	ThreadStatus*  tStatus;
};




#endif



