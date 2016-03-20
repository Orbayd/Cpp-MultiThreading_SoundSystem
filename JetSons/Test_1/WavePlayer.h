#ifndef WAVE_PLAYER_H
#define WAVE_PLAYER_H
#include <mutex>
#include <condition_variable>
#include "ThreadStatus.h"
#include "SafeQueue.h"
#include <queue>
class WaveBuffer;
class WavePlayer

{
public:
	WavePlayer();
	~WavePlayer();
	WavePlayer(WaveBuffer * wBuffer,ThreadStatus* ts);
	void Run();
	void Play();
	bool IsAlive();
public:
	WaveBuffer * tail;
	WaveBuffer * wheader;
	WaveBuffer * wcurrent;
	std::condition_variable cond_playback;
	threadsafe_queue<WaveBuffer*>  busy;
	ThreadStatus * tStatus;

	
private:
	std::mutex  cond_playback_mtx;
	
};



#endif