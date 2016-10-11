//---------------------------------------------------------------------------

#ifndef SYAlarmClockH
#define SYAlarmClockH
//---------------------------------------------------------------------------
/*
#include "SYAlarmClock.h"
SYAlarmClock *g_alarmCk = new SYAlarmClock();
g_alarmCk->OnDidEvent = AlarmClockTimeout;
g_alarmCk->Start();

for (int i = 0; i < 10; i++) {
	g_alarmCk->AddWithID(i, i*1000);
}
void __fastcall TForm1::AlarmClockTimeout(TObject* sender, int ID)
{
	wchar_t wszBuf[256];
        memset(wszBug, 0, sizeof(wszBuf));
        wsprintf(wszBuf, L"ID=%d\n", ID);
	OutputDebugString(wszBuf);
}
*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <vector>
//---------------------------------------------------------------------------
class SYAlarmClockItem
{
public:
     __property int ID = { read = m_ID, write = m_ID };
     __property DWORD BeginTime = { read = m_dwBeginTime, write = m_dwBeginTime };
     __property DWORD DelayTime = { read = m_dwDelayTime, write = m_dwDelayTime };
     SYAlarmClockItem()
     {
        m_dwBeginTime = 0;
        m_dwDelayTime = 0;
     }
     bool IsAlermTimeout(void)
     {
     	DWORD dwCurTime = GetTickCount();
    	if((dwCurTime - m_dwBeginTime) > m_dwDelayTime) return true;
        return false;
     }
private:
     int m_ID;
     DWORD m_dwBeginTime;
     DWORD m_dwDelayTime;
};
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *SYAlarmClock_OnDidEvent)(TObject* sender, int ID);

class SYAlarmClock : public TThread
{
public:
     SYAlarmClock();
     __fastcall ~SYAlarmClock(void);
     void Start(void);
     void Stop(void);
     void Clean(void);
     void CleanWithID(int ID);
     void AddWithID(int ID, unsigned int ms);
     //Event
     __property SYAlarmClock_OnDidEvent OnDidEvent = { read = m_onEvent, write = m_onEvent };
private:
     virtual void __fastcall Execute(void);
     void __fastcall Sync(void);
     void __fastcall DelayWithMilliSecond(unsigned int ms);
private:
    std::vector<SYAlarmClockItem*>* m_pVecQueue;
    SYAlarmClockItem *m_pCurAlarmClockItem;
    //event
    SYAlarmClock_OnDidEvent  m_onEvent;
public:
    bool	flag_Start;
    bool	flag_Doing;

};



#endif

