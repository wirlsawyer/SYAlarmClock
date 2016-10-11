//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "SYAlarmClock.h"
SYAlarmClock *g_alarmCk = new SYAlarmClock();
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
        g_alarmCk->OnDidEvent = AlarmClockTimeout;
	g_alarmCk->Start();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	for (int i = 0; i < 10; i++) {
      	    g_alarmCk->AddWithID(i, i*1000);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AlarmClockTimeout(TObject* sender, int ID)
{
	wchar_t wszBuf[256];
        memset(wszBuf, 0, sizeof(wszBuf));
        wsprintf(wszBuf, L"ID=%d\n", ID);
	OutputDebugString(wszBuf);
}
