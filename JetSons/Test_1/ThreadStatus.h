#ifndef THREAD_STATUS_H
#define THREAD_STATUS_H
#include <mutex>
#include <condition_variable>
enum class FILEREADER_THREAD
{
	DONE,
	WORKING
};
enum class CONTROLER_THREAD
{
	DONE,
	WORKING
};
enum class PLAYBACK_THREAD
{
	DONE,
	WORKING
};
enum class BUFFER_THREADS
{
	DONE,
	WORKING
};

struct ThreadStatus
{
	ThreadStatus();
	FILEREADER_THREAD file_thread;
	CONTROLER_THREAD  control_thread;
	PLAYBACK_THREAD	  playback_thread;
	BUFFER_THREADS     buffer_threads;
	int numbufferthreads;
	void  releasebufferthreads();
	std::mutex tstatusmtx;
	std::condition_variable  tstatus_cond;
};

#endif