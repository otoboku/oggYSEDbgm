#include "stdafx.h"
#include "ogg.h"
#include "oggDlg.h"
//#include <math.h>
//#include <vorbis/codec.h>
//#include <vorbis/vorbisfile.h>
//#include <MMSystem.h>
#include "dsound.h"
//#include "afxmt.h"
//#include "Douga.h"
//#include "itiran.h"
//#include "itiran_FC.h"
//#include "itiran_YSF.h"
//#include "itiran_YS6.h"
//#include "itiran_YSO.h"
//#include "vfw.h"
//#include <direct.h>
//#include "Folder.h"
//#include "dsound.h"

extern int fade1;
extern 	LPDIRECTSOUND m_ds;
extern 	LPDIRECTSOUNDBUFFER m_dsb;
extern 	LPDIRECTSOUND3DBUFFER m_dsb3d;
extern	LPDIRECTSOUNDBUFFER m_p;
extern LPDIRECTSOUND3DBUFFER m_lpDS3DBuffer;

extern int	playf;
extern void ReleaseOggVorbis(char **);
extern char *ogg;
extern DWORD hw;
extern HANDLE hNotifyEvent[2];
extern LPDIRECTSOUNDNOTIFY dsnf1;
extern LPDIRECTSOUNDNOTIFY dsnf2;
extern UINT HandleNotifications(LPVOID lpvoid);
extern ULONG WAVDALen;
extern UINT ttt;
#define BUFSZ			(8192*2)
#define HIGHDIV			4
#define BUFSZH			(BUFSZ/HIGHDIV)
#define SQRT_BUFSZ2		64
#define M_PI			3.1415926535897932384
#define ABS(N)			( (N)<0 ? -(N) : (N) )
#define OUTPUT_BUFFER_SIZE  BUFSZ
#define OUTPUT_BUFFER_NUM   10
extern void playwavds(char*bw);
extern void playwavds2(char*bw,int len);
extern BOOL playwavadpcm(char* bw,int old,int l1,int l2);
extern int mode;
extern save savedata;
LPDIRECTSOUND3DLISTENER m_listener=NULL;

CString COggDlg::init(HWND hwnd,int sm)
{
	DirectSoundCreate(NULL,&m_ds,NULL);
	if(m_ds==NULL) return _T("DirectSoundを生成できません。\nDirectX7が正常にインストールされているか確認してください。");
	if(m_ds->SetCooperativeLevel(hwnd,DSSCL_PRIORITY)!=DS_OK){
		return _T("DirectSoundの強調レベルを設定できません。\nDirectX7が正常にインストールされているか確認してください。");
	}
	hw=0;
//	ZeroMemory(&d,sizeof(d));d.dwSize=sizeof(d);HRESULT r =m_ds->GetCaps(&d);
//	if(r!=DS_OK){
//		return "DirectSoundの情報を獲得出来ません。\nDirectX7が正常にインストールされているか確認してください。";
//	}
//	if(d.dwFlags & (DSCAPS_SECONDARYSTEREO|DSCAPS_PRIMARYSTEREO |DSCAPS_PRIMARY16BIT) && d.dwFreeHwMemBytes!=0){
//		hw=DSBCAPS_LOCHARDWARE;
//	}::timeSetEvent
	m_p=NULL;
	DSBUFFERDESC dss;
	ZeroMemory(&dss,sizeof(dss));
	dss.dwSize=sizeof(dss);
//	dss.dwFlags=DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY|DSBCAPS_PRIMARYBUFFER|hw;
	dss.dwFlags=DSBCAPS_PRIMARYBUFFER;
	dss.lpwfxFormat=NULL;
	dss.dwBufferBytes=0;
	if(m_ds->CreateSoundBuffer(&dss,&m_p,NULL)!=DS_OK){
		return _T("DirectSoundのプライマリバッファを生成できません。\nDirectX7が正常にインストールされているか確認してください。");
	}
	if(m_p!=NULL){
//		//PCMWAVEFORMAT p;
		WAVEFORMATEX p;
		ZeroMemory(&p,sizeof(p));
		p.wFormatTag=WAVE_FORMAT_PCM;
		p.nChannels=2;
		p.nSamplesPerSec=sm;
		p.nBlockAlign=4;
		p.nAvgBytesPerSec=p.nSamplesPerSec*p.nBlockAlign;
		p.wBitsPerSample=16;
		m_p->SetFormat(&p);
	}
	//m_p->QueryInterface(IID_IDirectSound3DListener, (LPVOID*)&m_listener);
	//m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return _T("");
}

extern void DoEvent();
/*
void DoEvent()
{
	MSG msg;
	for(;;){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else return;
	}
}
*/
void COggDlg::Vol(int vol)
{
	m_dsb->SetVolume(vol);
}

void COggDlg::Closeds()
{
//	fade1=1;
	if(m_dsb){
		m_dsb->Stop();
		if(m_dsb3d != NULL){m_dsb3d->Release();m_dsb3d =NULL;}
		if(m_dsb != NULL){m_dsb->Release();m_dsb =NULL;}
	}
}

BOOL COggDlg::ReleaseDXSound(void)
{
	if(m_ds){
		Closeds();
		if(m_dsb3d != NULL){m_dsb3d->Release();m_dsb3d =NULL;}
		if(m_dsb != NULL) {m_dsb->Release();m_dsb=NULL;}
		if(m_lpDS3DBuffer != NULL){m_lpDS3DBuffer->Release();}
		m_dsb =NULL;
		m_lpDS3DBuffer= NULL;
		if(m_p!=NULL){m_p->Release();m_p=NULL;}

		if(m_ds){
			m_ds->Release();
			m_ds = NULL;
		}
	}
	return TRUE;
}

extern void playwavds2(char* bw,int old,int l1,int l2);
extern void playwavkpi(char* bw,int old,int l1,int l2);
extern void playwavmp3(char* bw,int old,int l1,int l2);
extern char bufwav3[OUTPUT_BUFFER_SIZE*OUTPUT_BUFFER_NUM*60];
extern int ps;
extern COggDlg *og;
extern BOOL thn;
extern BOOL thn1;
extern int endf;
extern int lenl;
extern int fade1;
extern BOOL sek;
//スレッド
int syukai=0,syukai2=0;
UINT HandleNotifications(LPVOID)
{
	DWORD hr = DS_OK;
	DWORD hRet = 0;
	thn=FALSE;
	thn1=FALSE;
	char* pdsb1;
	char* pdsb2;
	syukai=0;
//	char bufwav2[OUTPUT_BUFFER_SIZE];
	HANDLE ev[] = {(HANDLE)og->timer};
//	ULONG PlayCursor,WriteCursor=OUTPUT_BUFFER_SIZE*4,oldw=OUTPUT_BUFFER_SIZE*4;
	ULONG PlayCursor,WriteCursor=0,oldw=OUTPUT_BUFFER_SIZE*4;
	m_dsb->SetCurrentPosition(0);
	if(mode==-10){
		oldw=OUTPUT_BUFFER_SIZE*2;
		og->timer.SetEvent();
	}
	fade1=0;
	for(;;){
		DWORD  dwDataLen = WAVDALen/10;
		if(syukai==2) {thn=TRUE;AfxEndThread(0);}
		if(syukai==1) {syukai2=1;continue;}
//		int ik;
//		for(ik=0;ik<60;ik++){
//		if(syukai)
			::WaitForMultipleObjects(1, ev, FALSE, 60);
//		else
			//Sleep(1);
			if(sek==1){
				if(m_dsb)m_dsb->Stop();
				oldw=0;
				if((mode>=10 && mode<=20) || mode<-10)
					playwavadpcm(bufwav3,oldw,OUTPUT_BUFFER_SIZE*4,0);//データ獲得
				else if(mode==-10)
					playwavmp3(bufwav3,oldw,OUTPUT_BUFFER_SIZE*4,0);//データ獲得
				else if(mode==-3)
					playwavkpi(bufwav3,oldw,OUTPUT_BUFFER_SIZE*4,0);//データ獲得
				else
					playwavds2(bufwav3,oldw,OUTPUT_BUFFER_SIZE*4,0);//データ獲得
				oldw=OUTPUT_BUFFER_SIZE*4;WriteCursor=OUTPUT_BUFFER_SIZE*4;
				if(m_dsb)m_dsb->SetCurrentPosition(0);
				if(m_dsb)m_dsb->Play(0,0,DSBPLAY_LOOPING);
				sek=FALSE;
				//break;
			}
			if(thn1) {thn=TRUE;AfxEndThread(0);}
//		}
		if(ps==1) continue;
		if(m_dsb)m_dsb->GetCurrentPosition(&PlayCursor, &WriteCursor);//再生位置取得
		int len1=0,len2=0,len3,len4;
		len1=(int)WriteCursor-(int)oldw;//書き込み範囲取得
		len2=0;
		if(len1<0){
			len1=OUTPUT_BUFFER_SIZE*OUTPUT_BUFFER_NUM-oldw; len2=WriteCursor;}
		if(len2<0)
			len2=0;
		if((mode>=10 && mode<=20) || mode<-10)
			playwavadpcm(bufwav3,oldw,len1,len2);//データ獲得
		else if(mode==-10)
			playwavmp3(bufwav3,oldw,len1,len2);//データ獲得
		else if(mode==-3)
			playwavkpi(bufwav3,oldw,len1,len2);//データ獲得
		else
			playwavds2(bufwav3,oldw,len1,len2);//データ獲得
		if(m_dsb){
			m_dsb->Lock(oldw,len1+len2,(LPVOID *)&pdsb1,(DWORD*)&len3,(LPVOID *)&pdsb2,(DWORD*)&len4,0);
			Sleep(50);
			memcpy(pdsb1,bufwav3+oldw,len3);
			if(len4!=0)memcpy(pdsb2,bufwav3,len4);
			if(m_dsb)m_dsb->Unlock(pdsb1,len3,pdsb2,len4);
			oldw=WriteCursor;
//			oldw+=(len3+len4);
//			oldw%=OUTPUT_BUFFER_SIZE*OUTPUT_BUFFER_NUM;
		}
		if(fade1){
//			if(m_dsb){
//				m_dsb->Lock(0,OUTPUT_BUFFER_SIZE*OUTPUT_BUFFER_NUM,(LPVOID *)&pdsb1,(DWORD*)&len3,(LPVOID *)&pdsb2,(DWORD*)&len4,0);
//				ZeroMemory(pdsb1,len3);
//				if(len4!=0)ZeroMemory(pdsb2,len4);
//				m_dsb->Unlock(pdsb1,len3,pdsb2,len4);
//			}
			Sleep(750);
			og->OnPause();
			playf=0;
			thn=TRUE;
			AfxEndThread(0);
//			for(int y=0;y<11;y++){
//				CloseHandle(hNotifyEvent[y]);hNotifyEvent[y] = (HANDLE)NULL;
//			}
			return 0;
		}
	}


//----以下無効	
/*	for(;;){
		DWORD  dwDataLen = WAVDALen/10;
		Sleep(1);
		if(m_dsb==NULL) return 0;
		if(m_dsb)m_dsb->GetCurrentPosition(&PlayCursor, &WriteCursor);
		if(ttt<WriteCursor){//書き込み
			playwavds(bufwav2);
			m_dsb->Lock(ttt,dwDataLen,(LPVOID *)&pdsb,&dwDataLen,NULL,0,0);
			memcpy(pdsb,bufwav2,dwDataLen);
			m_dsb->Unlock(pdsb,dwDataLen,NULL,0);
			ttt = ttt + (int)dwDataLen;
			if( ttt > WAVDALen-(int)dwDataLen) ttt = 0;
		Sleep(66);//待機時間 リングバッファの空きをここで待つ
		}
		if(fade1){
			playf=0;
			m_dsb->Stop();
			ReleaseOggVorbis(&ogg);
			ogg=NULL;

			for(int y=0;y<11;y++){
				CloseHandle(hNotifyEvent[y]);hNotifyEvent[y] = (HANDLE)NULL;
			}
			return 0;
		}
	}

	while((hRet = WaitForMultipleObjects(11, hNotifyEvent, FALSE, INFINITE))!= WAIT_FAILED)
	{
		// Deal with the event that got signalled.
		switch(hRet-WAIT_OBJECT_0)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			{	// A play notification has been received.
				DWORD  dwDataLen = WAVDALen/10;

//				ttt = ttt + (int)dwDataLen;
//				if( ttt > WAVDALen-(int)dwDataLen) ttt = 0;
				int tttt=(hRet-WAIT_OBJECT_0);tttt+=8;
				if(tttt>9)tttt-=10;
				ttt=tttt*dwDataLen;
				playwavds(bufwav2);
				m_dsb->Lock(ttt,dwDataLen,(LPVOID *)&pdsb,&dwDataLen,NULL,0,0);
				memcpy(pdsb,bufwav2,dwDataLen);
				m_dsb->Unlock(pdsb,dwDataLen,NULL,0);
				if(fade1){
					m_dsb->Stop();
					ReleaseOggVorbis(&ogg);
					ogg=NULL;

					for(int y=0;y<11;y++){
						CloseHandle(hNotifyEvent[y]);hNotifyEvent[y] = (HANDLE)NULL;
					}
					return 0;
				}
				break;
			}
		case 10:
			{

				m_dsb->Stop();
					for(int y=0;y<11;y++){
						CloseHandle(hNotifyEvent[y]);hNotifyEvent[y] = (HANDLE)NULL;
					}
				break;
			}
		default:
			ASSERT(FALSE);	// we should never fall in here.
			break;
		}// end switch
	} // while
	//UpdateProgressBar();
	return 0;
	*/
} //handlenotifications()
