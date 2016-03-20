#include "WavePlayer.h"
#include "WaveBuffer.h"
#include "ThreadHelper.h"
#include <mutex>
#include <condition_variable>
#include <thread>


WavePlayer::WavePlayer(){}

WavePlayer::WavePlayer(WaveBuffer * vbuffer,ThreadStatus * ts):wheader(vbuffer),wcurrent(vbuffer)
{
	this->tStatus = ts;
}

WavePlayer::~WavePlayer(){}

void WavePlayer::Play()
{


	while(wcurrent->next != wheader)
	{
		
		wcurrent->sendRequest();
		wcurrent= wcurrent->next;
	}
	wcurrent->sendRequest();
	wcurrent= wcurrent->next;
	
	printf("Playback (%llu)\n",GetCurrentID());
	while(1)
	{
		if(!tail->IsEmty())
			break;
	} 
  
	
	 

	   wcurrent->Play();
	   busy.push(wcurrent);
	   wcurrent=wcurrent->next;

     	wcurrent->Play();
		busy.push(wcurrent);
	    wcurrent=wcurrent->next;

	    wcurrent->Play();
	    busy.push(wcurrent);
		wcurrent=wcurrent->next;

	while(IsAlive())
	{
		
		  wcurrent->Play();
		  busy.push(wcurrent);
		  std::unique_lock<std::mutex> lk(cond_playback_mtx);
		  cond_playback.wait(lk);

		wcurrent =(WaveBuffer*)wcurrent->next;
		
		
	}
	printf("PLAYBACK Released\n");

}

void WavePlayer::Run() 
{
	Play();
}

bool WavePlayer::IsAlive()
{
	if(tStatus->control_thread==CONTROLER_THREAD::DONE)
	{
		tStatus->playback_thread=PLAYBACK_THREAD::DONE;
		printf("Current %s \n",wcurrent->wavename);
		
		while(1){
			
			
				wcurrent->status= WaveStatus::FINISHED;
				wcurrent->Play();
			    wcurrent =wcurrent->next;
				if(tStatus->numbufferthreads==0)
					break;
		}
		
		
		
			/*std::unique_lock<std::mutex> lk(tStatus->tstatusmtx);
			tStatus->tstatus_cond.wait(lk);*/


		return false;
	}
	else
		return true;

}