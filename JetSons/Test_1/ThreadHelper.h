#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

	#include <Windows.h>
	#include <thread>

	void SetThreadName( std::thread &t0, char* threadName);
	void SetThreadName( DWORD id, char* threadName);
	char *GetThreadName( );
	unsigned long long GetCurrentID();

#endif