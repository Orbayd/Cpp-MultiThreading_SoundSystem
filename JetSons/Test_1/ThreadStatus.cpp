#include "ThreadStatus.h"
std::mutex mtx;
ThreadStatus::ThreadStatus()
{
	file_thread = FILEREADER_THREAD::WORKING;
	control_thread= CONTROLER_THREAD::WORKING;
	playback_thread=PLAYBACK_THREAD::WORKING;
	buffer_threads= BUFFER_THREADS::WORKING;
	numbufferthreads = 20;

}

void ThreadStatus::releasebufferthreads()
{
	 std::lock_guard<std::mutex> lock(mtx);
	numbufferthreads--;
	printf("Number of Working Threads %d\n",numbufferthreads);



}