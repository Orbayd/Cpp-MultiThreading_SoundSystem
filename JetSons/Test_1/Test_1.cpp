// Test_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment (lib,"winmm.lib") 
#include <assert.h>

#include "File.h"
#include "WaveBuffer.h"
#include "WaveFileReader.h"
#include "WavePlayer.h"
#include "WaveControler.h"
#include "ThreadHelper.h"
#include "ThreadStatus.h"
#include <thread>

static void CALLBACK waveOutProc(HWAVEOUT& hWaveOut, UINT uMsg, DWORD dwInstance, DWORD, DWORD)
{
	// even though the waveFreeBlockCount int is global, we still passed it in (it's dwInstance)
	// so let's dereference it
	//int* freeBlockCounter = (int*)dwInstance;
	WavePlayer *c  = (WavePlayer *)dwInstance;
	
	
	hWaveOut;
	// I'm not sure what messages we'd get besides DONE, but if it's anything else, return
	if(uMsg != WOM_DONE)
		return;
	// lock the freeblockcounter with a lightweight mutex
	if(uMsg ==WOM_DONE)
	{
		if(c!=0)
		{
			if(c->tStatus->playback_thread == PLAYBACK_THREAD::WORKING){
			 std::shared_ptr<WaveBuffer*> b= c->busy.try_pop();
		      //printf("Callback  %s \n",(*b)->wavename);
		     (*b)->sendRequest();
			 (*b)->waveout_write_cond.notify_one();
		     c->cond_playback.notify_one();
			}
		
		
		
		}
		
	}

		
	
	// now it's unlocked
}
int main()
{
	printf("MAIN  ID:(%llu) \n",GetCurrentID());
	ThreadStatus  tstatus;
	WaveFileReader waveReader(&tstatus);
	WaveControler  wControler(&waveReader,&tstatus);

	std::thread treader(&WaveFileReader::Run,std::ref(waveReader));
	std::thread tcontroler(&WaveControler::Run,std::ref(wControler));

	
	
	WaveBuffer     wBuffer_0("wb_0",&tstatus); WaveBuffer     wBuffer_1("wb_1",&tstatus); WaveBuffer     wBuffer_2("wb_2",&tstatus);
	WaveBuffer     wBuffer_3("wb_3",&tstatus); WaveBuffer     wBuffer_4("wb_4",&tstatus);  WaveBuffer     wBuffer_5("wb_5",&tstatus);
	WaveBuffer     wBuffer_6("wb_6",&tstatus); WaveBuffer     wBuffer_7("wb_7",&tstatus);  WaveBuffer     wBuffer_8("wb_8",&tstatus);
	WaveBuffer     wBuffer_9("wb_9",&tstatus); WaveBuffer     wBuffer_10("wb_10",&tstatus);WaveBuffer     wBuffer_11("wb_11",&tstatus);
	WaveBuffer     wBuffer_12("wb_12",&tstatus);WaveBuffer     wBuffer_13("wb_13",&tstatus);WaveBuffer     wBuffer_14("wb_14",&tstatus);
	WaveBuffer     wBuffer_15("wb_15",&tstatus);WaveBuffer     wBuffer_16("wb_16",&tstatus);WaveBuffer     wBuffer_17("wb_17",&tstatus);
	WaveBuffer     wBuffer_18("wb_18",&tstatus);WaveBuffer     wBuffer_19("wb_19",&tstatus);

	wBuffer_0.next = &wBuffer_1; wBuffer_1.next = &wBuffer_2; wBuffer_2.next = &wBuffer_3; wBuffer_3.next = &wBuffer_4; wBuffer_4.next = &wBuffer_5;
	wBuffer_5.next = &wBuffer_6; wBuffer_6.next = &wBuffer_7; wBuffer_7.next = &wBuffer_8; wBuffer_8.next = &wBuffer_9; wBuffer_9.next = &wBuffer_10;
	wBuffer_10.next = &wBuffer_11; wBuffer_11.next = &wBuffer_12; wBuffer_12.next = &wBuffer_13; wBuffer_13.next = &wBuffer_14; wBuffer_14.next = &wBuffer_15;
    wBuffer_15.next = &wBuffer_16; wBuffer_16.next = &wBuffer_17; wBuffer_17.next = &wBuffer_18; wBuffer_18.next = &wBuffer_19;wBuffer_19.next = &wBuffer_0;

	HWAVEOUT hWaveOut;
	
	//
	//WavePlayer wPlayer(&wBuffer_0);
	WavePlayer *wPlayer = new WavePlayer(&wBuffer_0,&tstatus);
	wPlayer->tail  = &wBuffer_19;
	//waveOutOpen(&hWaveOut,WAVE_MAPPER,&wBuffer_0.waveformat.wfx,0,0,CALLBACK_NULL);
	
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wBuffer_0.waveformat.wfx, (DWORD_PTR)waveOutProc, (DWORD_PTR)wPlayer, CALLBACK_FUNCTION);
	wBuffer_0.hWaveOut = std::ref(hWaveOut);    wBuffer_6.hWaveOut = std::ref(hWaveOut);	wBuffer_10.hWaveOut = std::ref(hWaveOut);  wBuffer_15.hWaveOut = std::ref(hWaveOut);
	wBuffer_1.hWaveOut = std::ref(hWaveOut);	wBuffer_5.hWaveOut = std::ref(hWaveOut);	wBuffer_11.hWaveOut = std::ref(hWaveOut);  wBuffer_16.hWaveOut = std::ref(hWaveOut);
	wBuffer_2.hWaveOut = std::ref(hWaveOut);	wBuffer_7.hWaveOut = std::ref(hWaveOut);	wBuffer_12.hWaveOut = std::ref(hWaveOut);  wBuffer_17.hWaveOut = std::ref(hWaveOut);
	wBuffer_3.hWaveOut = std::ref(hWaveOut);	wBuffer_8.hWaveOut = std::ref(hWaveOut);	wBuffer_13.hWaveOut = std::ref(hWaveOut);  wBuffer_18.hWaveOut = std::ref(hWaveOut);
	wBuffer_4.hWaveOut = std::ref(hWaveOut);	wBuffer_9.hWaveOut = std::ref(hWaveOut);	wBuffer_14.hWaveOut = std::ref(hWaveOut);  wBuffer_19.hWaveOut = std::ref(hWaveOut);
	   

	
	
     std::thread t0(&WaveBuffer::Run,std::ref(wBuffer_0));
	 std::thread t1(&WaveBuffer::Run,std::ref(wBuffer_1));
	 std::thread t2(&WaveBuffer::Run,std::ref(wBuffer_2));
	 std::thread t3(&WaveBuffer::Run,std::ref(wBuffer_3));
	 std::thread t4(&WaveBuffer::Run,std::ref(wBuffer_4));

	 std::thread t5(&WaveBuffer::Run,std::ref(wBuffer_5));
	 std::thread t6(&WaveBuffer::Run,std::ref(wBuffer_6));
	 std::thread t7(&WaveBuffer::Run,std::ref(wBuffer_7));
	 std::thread t8(&WaveBuffer::Run,std::ref(wBuffer_8));
	 std::thread t9(&WaveBuffer::Run,std::ref(wBuffer_9));

	 std::thread t10(&WaveBuffer::Run,std::ref(wBuffer_10));
	 std::thread t11(&WaveBuffer::Run,std::ref(wBuffer_11));
	 std::thread t12(&WaveBuffer::Run,std::ref(wBuffer_12));
	 std::thread t13(&WaveBuffer::Run,std::ref(wBuffer_13));
	 std::thread t14(&WaveBuffer::Run,std::ref(wBuffer_14));

	 std::thread t15(&WaveBuffer::Run,std::ref(wBuffer_15));
	 std::thread t16(&WaveBuffer::Run,std::ref(wBuffer_16));
	 std::thread t17(&WaveBuffer::Run,std::ref(wBuffer_17));
	 std::thread t18(&WaveBuffer::Run,std::ref(wBuffer_18));
	 std::thread t19(&WaveBuffer::Run,std::ref(wBuffer_19));
	
	  std::thread tplayer(&WavePlayer::Run,std::ref(wPlayer));
	

	  treader.join();
	  tcontroler.join();
	 
	  t0.join(); t1.join();  t2.join();  t3.join();  t4.join();
	  t5.join();  t6.join();  t7.join();  t8.join();  t9.join();
	  t10.join();  t11.join();  t12.join();  t13.join();  t14.join();
	  t15.join();  t16.join();  t17.join();  t18.join();  t19.join();
	 
	  
	 tplayer.join();

	  delete wPlayer;
	  waveOutClose(hWaveOut);
	  printf("MAIN  ID:(%llu) \n",GetCurrentID());
	  system("PAUSE");
	  return 0;
}

