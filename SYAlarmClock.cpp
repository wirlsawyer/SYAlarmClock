//---------------------------------------------------------------------------


#pragma hdrstop

#include "SYAlarmClock.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

SYAlarmClock::SYAlarmClock()
: TThread(true)
{
    m_onEvent     = NULL;
    m_pCurAlarmClockItem = NULL;
    m_pVecQueue   = new std::vector<SYAlarmClockItem*>;
    flag_Start    = false;
    flag_Doing    = false;
    this->Resume();
}
__fastcall SYAlarmClock::~SYAlarmClock(void)
{
 	this->Stop();
        this->Terminate();

	if (m_pVecQueue)
	{
		//clear
		while(!m_pVecQueue->empty())
		{
			delete m_pVecQueue->back();
			m_pVecQueue->pop_back();
		}
		delete m_pVecQueue;
	}
}
void SYAlarmClock::Start(void)
{
 	this->flag_Start = true;
}

void SYAlarmClock::Stop(void)
{
	flag_Start    = false;
}

void SYAlarmClock::Clean(void)
{
 	flag_Start = false;
        while(flag_Doing)
        {
        	DelayWithMilliSecond(10);
        }
        //clear
   	while(!m_pVecQueue->empty())
   	{
   		delete m_pVecQueue->back();
   		m_pVecQueue->pop_back();
   	}

        this->flag_Start = true;
}

void SYAlarmClock::CleanWithID(int ID)
{
	flag_Start = false;
        while(flag_Doing)
        {
        	DelayWithMilliSecond(10);
        }

        //clear
        std::vector<SYAlarmClockItem *>::iterator iter = m_pVecQueue->begin();
    	while (iter != m_pVecQueue->end())
        {
        	SYAlarmClockItem *item = *iter;
                if (item->ID == ID) {
                   m_pVecQueue->erase(iter);
                } else {
                   ++iter;
                }
        }
        this->flag_Start = true;
}

void SYAlarmClock::AddWithID(int ID, unsigned int ms)
{
	SYAlarmClockItem *item = new SYAlarmClockItem();
        item->BeginTime = GetTickCount();
        item->DelayTime = ms;
        item->ID = ID;
	m_pVecQueue->push_back(item);
}

void __fastcall SYAlarmClock::Execute(void)
{
	while (true)
	{
                if (this->Terminated) {
                    break;
                }
    	        if (this->flag_Start && this->flag_Doing == false)
                {
                        flag_Doing = true;
    			if (m_pVecQueue->empty())
        		{
                             flag_Doing = false;
    			}else{
                           std::vector<SYAlarmClockItem *>::iterator iter = m_pVecQueue->begin();
			   while (iter != m_pVecQueue->end())
        		   {
        			m_pCurAlarmClockItem = *iter;
                		if (m_pCurAlarmClockItem->IsAlermTimeout()) {
                                    Synchronize(Sync);
                                    m_pVecQueue->erase(iter);
                                    delete m_pCurAlarmClockItem;
                                    m_pCurAlarmClockItem = NULL;
                		}else{
                                    ++iter;
                                }
                           }
        		}
                }
                flag_Doing = false;
                DelayWithMilliSecond(10);
    	}//end while
}

void __fastcall SYAlarmClock::Sync(void)
{
        if (m_onEvent) {
            m_onEvent(this, m_pCurAlarmClockItem->ID);
        }
}

void __fastcall SYAlarmClock::DelayWithMilliSecond(unsigned int ms)
{
	DWORD dwBeginTime = GetTickCount();
    while(true)
    {
    	DWORD dwCurTime = GetTickCount();
    	if((dwCurTime - dwBeginTime) > ms)
    	{
    		break;
    	}
    	//Dovenets
    	MSG msg;
    	while (PeekMessage( & msg, NULL,  0 ,  0 , PM_REMOVE))
    	{
    		TranslateMessage( & msg);
    		DispatchMessage( & msg);
    	}
    }
}

