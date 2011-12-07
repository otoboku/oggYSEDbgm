// PlayList.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "direct.h"
#include "dshow.h"
#include "ogg.h"
#include "oggDlg.h"
#include "ListCtrlA.h"
#include "PlayList.h"
#include "ListSyosai.h"
#include "Filename.h"
#include "Douga.h"
// CPlayList �_�C�A���O

IMPLEMENT_DYNAMIC(CPlayList, CDialog)

extern 	CString ext[10000][100];
extern 	CString kpif[10000];
extern 	int kpicnt;

extern BOOL plw;


CPlayList::CPlayList(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayList::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_PL);
	pc=NULL;
	plw=0;
//	pc = new playlistdata0[60000];
}

CPlayList::~CPlayList()
{
	if(pc)
		free(pc);
}

void CPlayList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_lsup);
	DDX_Control(pDX, IDC_BUTTON5, m_lup);
	DDX_Control(pDX, IDC_BUTTON10, m_lsdown);
	DDX_Control(pDX, IDC_BUTTON11, m_ldown);
	DDX_Control(pDX, IDC_LIST1, m_lc);
	DDX_Control(pDX, IDC_EDIT1, m_e);
	DDX_Control(pDX, IDC_CHECK1, m_renzoku);
	DDX_Control(pDX, IDC_CHECK4, m_loop);
	DDX_Control(pDX, IDC_CHECK28, m_tool);
	DDX_Control(pDX, IDC_CHECK29, m_saisyo);
	DDX_Control(pDX, IDC_EDIT2, m_find);
	DDX_Control(pDX, IDC_BUTTON16, m_findup);
	DDX_Control(pDX, IDC_BUTTON20, m_finddown);
}


BEGIN_MESSAGE_MAP(CPlayList, CDialog)
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CPlayList::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CPlayList::OnUP)
	ON_BN_CLICKED(IDC_BUTTON5, &CPlayList::OnSUP)
	ON_BN_CLICKED(IDC_BUTTON10, &CPlayList::OnSDOWN)
	ON_BN_CLICKED(IDC_BUTTON11, &CPlayList::OnDOWN)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CPlayList::OnLvnKeydownList1)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CPlayList::OnNMDblclkList1)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CHECK4, &CPlayList::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK1, &CPlayList::OnBnClickedCheck1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, &CPlayList::OnLvnBegindragList1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CPlayList::OnLvnGetdispinfoList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CPlayList::OnNMRclickList1)
	ON_COMMAND(ID_POP_32776, OnList)
	ON_COMMAND(ID_POP_32777,Del)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_POP_32787, &CPlayList::OnPop32787)
	ON_BN_CLICKED(IDC_BUTTON16, &CPlayList::OnFindUp)
	ON_BN_CLICKED(IDC_BUTTON20, &CPlayList::OnFindDown)
END_MESSAGE_MAP()

#include <eh.h>
class SE_Exception1
{
private:
    unsigned int nSE;
public:
    SE_Exception1() {}
    SE_Exception1( unsigned int n ) : nSE( n ) {}
    ~SE_Exception1() {}
    unsigned int getSeNumber() { return nSE; }
};
void trans_func1( unsigned int, EXCEPTION_POINTERS* );
void trans_func1( unsigned int u, EXCEPTION_POINTERS* pExp )
{
    throw SE_Exception1();
}


int playcnt=0;

int syo;
CString syos;
extern TCHAR karento2[1024];
extern int fade1;
extern IMediaPosition *pMediaPosition;
extern int mode,videoonly,playf;
extern int plcnt;
BOOL CPlayList::OnInitDialog()
{
	CDialog::OnInitDialog();
	playcnt=0;
	w_flg=TRUE;
	pnt=0;
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	// TODO:  �����ɏ�������ǉ����Ă�������
	m_lsup.SetIcon(IDR_SUP);
	m_lsup.SetFlat(TRUE);
	m_lup.SetIcon(IDR_UP);
	m_lup.SetFlat(TRUE);
	m_lsdown.SetIcon(IDR_SDOWN);
	m_lsdown.SetFlat(TRUE);
	m_ldown.SetIcon(IDR_DOWN);
	m_ldown.SetFlat(TRUE);

	m_findup.SetIcon(IDR_DOWN);
	m_findup.SetFlat(TRUE);
	m_finddown.SetIcon(IDR_UP);
	m_finddown.SetFlat(TRUE);

	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDOK), _T("�v���C���X�g����܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON1), _T("�I�����ڂ���ԏ�Ɏ����čs���܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON5), _T("�I�����ڂ���Ɏ����čs���܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON10), _T("�I�����ڂ���ԉ��Ɏ����čs���܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON11), _T("�I�����ڂ����Ɏ����čs���܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK1), _T("�v���C���X�g�̏��ԂɍĐ����s���܂��B\n�Đ����Ƀt�@�C���h���b�v���Ēǉ����Ă����t���̋Ȃ͂��̂܂ܖ葱���܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK4), _T("�I�������Ȃ����[�v�����܂��B\n�Đ�����O�Ƀ`�F�b�N������K�v������܂��B\n�����łȂ��ƃ��[�v�͂�����܂���B\n���[�v�|�C���g��0�̂���(mp3�⃋�[�v���Ȃ���)���Ώۂł��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK28), _T("�c�[���`�b�v��\�����܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK29), _T("�ŏ����A�ŏ�������̕��A���A���C����ʂƃv���C���X�g��ʂ������ɍŏ����A�ŏ�������̕��A���s���܂��B"));
	m_tooltip.SetDelayTime( TTDT_AUTOPOP, 10000 );
	m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 512);

	DWORD dwExStyle = m_lc.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;//|LVS_EX_INFOTIP;
	m_lc.SetExtendedStyle(dwExStyle);
	il.Create(16, 16, ILC_COLOR, 0, 1);
	il.Add(::AfxGetApp()->LoadIcon(IDI_ICON1)); 
	il.Add(::AfxGetApp()->LoadIcon(IDI_ICON2)); 
	il.Add(::AfxGetApp()->LoadIcon(IDI_ICON3)); 
	m_lc.SetImageList(&il,LVSIL_SMALL);
	m_lc.ModifyStyle ( 0, LVS_REPORT );
	m_lc.InsertColumn ( 0, _T("���O"), LVCFMT_LEFT, 200, 0 );
	m_lc.InsertColumn ( 1, _T("�Q�[��"), LVCFMT_LEFT, 50, 0 );
	m_lc.InsertColumn ( 2, _T("����"), LVCFMT_RIGHT, 50, 0 );
	m_lc.InsertColumn ( 3, _T("�A�[�e�B�X�g"), LVCFMT_LEFT, 200, 0 );
	m_lc.InsertColumn ( 4, _T("�A���o��/�R�����g"), LVCFMT_LEFT, 200, 0 );
	m_lc.InsertColumn ( 5, _T("�t�H���_"), LVCFMT_LEFT, 50, 0 );
//	pc=NULL;
//	pc = (playlistdata0*)malloc(sizeof(playlistdata0)*50000);
//	if(pc==NULL)
//		EndDialog(0);
	m_lc.SetFocus();
	pnt=pnt1=-1;
	nnn=1;
	pc=NULL;
	Load();
	if(pc==NULL){
		pc = (playlistdata0*)malloc(sizeof(playlistdata0));
	}
	SetTimer(20,20,NULL);
	SetTimer(30,1200,NULL);
	SetTimer(40,500,NULL);
	SetTimer(50,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
extern int killw1;
extern CPlayList *pl;
void CPlayList::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	killw1=1;
}

BOOL CPlayList::DestroyWindow()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	Save();
//	free(pc);
//	pc=NULL;
	KillTimer(20);
	KillTimer(30);
	BOOL rr=CDialog::DestroyWindow();
	pl=NULL;
	if(nnn)
		delete this;
	plw=0;
	return rr;
}

int CPlayList::Create(CWnd *pWnd)
{
	 m_pParent = NULL;
	BOOL bret = CDialog::Create( CPlayList::IDD, this);
    if( bret == TRUE)
        ShowWindow( SW_SHOW);
    return bret;
}

void CPlayList::OnClose()
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	nnn=0;
	DestroyWindow();

	CDialog::OnClose();
}

void CPlayList::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
//	DestroyWindow();
}

BOOL CPlayList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
		m_tooltip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

int pnt1=-1;

int CPlayList::chk(CString name,int sub,CString art,CString fol,int ret)
{
	int i=m_lc.GetItemCount(),c=0;
	pnt1=-1;
	CString s,s1;
	for(int j=0;j<i;j++){
		c=0;
		if((pc[j].sub==-10)||(pc[j].sub==-2)||(pc[j].sub==-3)){
			if(_tcscmp(pc[j].fol,fol)==0 && pc[j].sub==sub)
				return j;
		}else{
			if(_tcscmp(pc[j].fol,fol)==0 && pc[j].sub==sub && (pc[j].ret2==ret))
				return j;
		}
	}
	return -1;
}

CString CPlayList::UTF8toSJIS(const char* a)
{
	WCHAR f[1024];
	char ff[1024];
	int rr=MultiByteToWideChar(CP_UTF8,0,a,-1,f,1024);
	int rr2=WideCharToMultiByte(CP_ACP,0,f,rr,ff,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,f,rr,ff,rr2,NULL,NULL);
	CString s; s=f;
	return s;
//	return _T("");
}

CString CPlayList::UTF8toUNI(const TCHAR* a)
{
//	WCHAR f[1024];
//	char ff[1024];
//	int rr2=WideCharToMultiByte(CP_ACP, 0, a,1024,ff,1024,NULL,NULL);
//	int rr= MultiByteToWideChar(CP_UTF8,0,ff,-1,f ,1024);
//	WideCharToMultiByte(CP_ACP,0,f,rr,ff,rr2,NULL,NULL);
	CString s; s=a;
	return s;
//	return _T("");
}

int CPlayList::Add(CString name,int sub,int loop1,int loop2,CString art,CString alb,CString fol,int ret,int time,BOOL f,BOOL ff)
{
	int cnt1;
	CString s,ss;
	switch(sub){
		case 1:s="��̋O��SC";break;
		case 2:s="��̋O��FC";break;
		case 3:s="�C�[�X �t�F���K�i�̐���";break;
		case 4:s="Ys6 �i�s�V���e���̙�";break;
		case 5:s="�C�[�X �I���W��";break;
		case 6:s="��̋O�� The3rd";break;
		case 7:s="ZWEI II";break;
		case 8:s="Ys I&II Chronicles 1";break;
		case 9:s="Ys I&II Chronicles 2";break;
		case 10:s="XANADU NEXT";break;
		case 11:s="Ys I&II ���S�� 1";break;
		case 12:s="Ys I&II ���S�� 2";break;
		case 13:s="Sorcerian Original";break;
		case 14:s="Zwei!!";break;
		case 15:s="����݂� -GURUMIN-";break;
		case 16:s="�_�C�i�\�A ���U���N�V����";break;
		case 17:s="Brandish4 �����_�̓�";break;
		case 18:s="��������";break;
		case 19:s="��g����";break;
		case 20:s="�C�̟B��";break;
		case -11:s="���e�̃��v�\�f�B�[";break;
		case -12:s="�����̋�����";break;
		case -13:s="�A�[�N�g�D���X";break;
		case -14:s="���z�O���u1";break;
		case -15:s="���z�O���u2";break;
		case -3:
			ss=fol.Right(fol.GetLength()-fol.ReverseFind('.')-1);
			s.Format(_T("%s�t�@�C��"),ss);break;
		case -2:
			ss=fol.Right(fol.GetLength()-fol.ReverseFind('.')-1);
			s.Format(_T("%s�t�@�C��"),ss);break;
		case -1:s="ogg�t�@�C��";break;
		case -10:
			s=fol;s.MakeLower();
			if(s.Right(3)=="mp3"){ s="mp3�t�@�C��";break;}
			if(s.Right(3)=="mp2"){ s="mp2�t�@�C��";break;}
			if(s.Right(3)=="mp1"){ s="mp1�t�@�C��";break;}
			if(s.Right(3)=="rmp"){ s="rmp�t�@�C��";break;}
	}

	if(f)
		if((cnt1=chk(name,sub,art,fol,ret))!=-1){
			pc[cnt1].time=time;
			RECT r;
			m_lc.GetItemRect(cnt1,&r,LVIR_BOUNDS);
			m_lc.RedrawWindow(&r);	
			return cnt1;
		}
//	if(playcnt<60000){
		if(ff){
			playlistdata0 *tmp;tmp=pc;
			size_t size=_msize(pc);
			pc=(playlistdata0*)realloc(tmp,size+sizeof(playlistdata0));
			if(pc==NULL){
				return -1;
			}
			m_lc.SetItemCount(playcnt+1);
		}
		_tcscpy(pc[playcnt].name,name);
		_tcscpy(pc[playcnt].art,art);
		_tcscpy(pc[playcnt].alb,alb);
		_tcscpy(pc[playcnt].fol,fol);
		_tcscpy(pc[playcnt].game,s);
		pc[playcnt].loop1=loop1;
		pc[playcnt].loop2=loop2;
		pc[playcnt].sub=sub;
		pc[playcnt].ret2=ret;
		pc[playcnt].icon=1;
		pc[playcnt].time=time;
//		RECT r;
//		m_lc.GetItemRect(playcnt,&r,LVIR_BOUNDS);
//		m_lc.RedrawWindow(&r);	
		playcnt++;
//	}		
		
	return -1;
}

void CPlayList::Del()
{
	int Lindex=-1,j=0;
	for(;;){//�I������Ă�����̂��s�b�N�A�b�v
		Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
		if(Lindex==-1) break;
		m_lc.SetItemState(Lindex,m_lc.GetItemState(Lindex,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
		for(int i=Lindex+1+j;i<playcnt;i++){
			memcpy(&pc[i-1],&pc[i],sizeof(playlistdata0));
		}
		playcnt--;j--;
	}
	playlistdata0 *tmp;tmp=pc;
	pc=(playlistdata0*)realloc(tmp,(size_t)sizeof(playlistdata0)*(playcnt+2));//�]�T�������ĉ��
	m_lc.SetItemCount(playcnt);
	for(j=0;j<playcnt;j++) pc[j].icon=1;
	m_lc.RedrawWindow();
	Save();
}

void CPlayList::OnSUP()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int i=m_lc.GetItemCount();
	CString s,s1;
	for(int j=0;j<i-1;j++){
		if((m_lc.GetItemState(j+1,LVIS_SELECTED)&LVIS_SELECTED)&&!(m_lc.GetItemState(j,LVIS_SELECTED)&LVIS_SELECTED)){
			playlistdata0 ppp;
			pc[j].icon=pc[j+1].icon=1;
			memcpy(&ppp,&pc[j+1],sizeof(playlistdata0));
			memcpy(&pc[j+1],&pc[j],sizeof(playlistdata0));
			memcpy(&pc[j],&ppp,sizeof(playlistdata0));
			m_lc.RedrawItems(j,j+1);
			m_lc.SetItemState(j  ,m_lc.GetItemState(j  ,LVIS_SELECTED)|LVIS_SELECTED,LVIS_SELECTED);
			m_lc.SetItemState(j+1,m_lc.GetItemState(j+1,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
		}
	}
//	m_lc.RedrawWindow();
	Save();
}

void CPlayList::OnUP()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int i=m_lc.GetItemCount(),i2=0;
	CString s,s1;
	for(;;){i2=0;
		for(int j=0;j<i-1;j++){
			if((m_lc.GetItemState(j+1,LVIS_SELECTED)&LVIS_SELECTED)&&!(m_lc.GetItemState(j,LVIS_SELECTED)&LVIS_SELECTED)){
			playlistdata0 ppp;
			pc[j].icon=pc[j+1].icon=1;
			memcpy(&ppp,&pc[j+1],sizeof(playlistdata0));
			memcpy(&pc[j+1],&pc[j],sizeof(playlistdata0));
			memcpy(&pc[j],&ppp,sizeof(playlistdata0));
			m_lc.RedrawItems(j,j+1);
				m_lc.SetItemState(j  ,m_lc.GetItemState(j  ,LVIS_SELECTED)|LVIS_SELECTED,LVIS_SELECTED);
				m_lc.SetItemState(j+1,m_lc.GetItemState(j+1,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
				i2=1;
			}
		}
		if(i2==0) break;
	}	
//	m_lc.RedrawWindow();
	Save();
}

void CPlayList::OnSDOWN()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int i=m_lc.GetItemCount(),i2=0;
	CString s,s1;
	for(;;){i2=0;
		for(int j=i-1;j>0;j--){
			if((m_lc.GetItemState(j-1,LVIS_SELECTED)&LVIS_SELECTED)&&!(m_lc.GetItemState(j,LVIS_SELECTED)&LVIS_SELECTED)){
			playlistdata0 ppp;
			pc[j].icon=pc[j+1].icon=1;
			memcpy(&ppp,&pc[j-1],sizeof(playlistdata0));
			memcpy(&pc[j-1],&pc[j],sizeof(playlistdata0));
			memcpy(&pc[j],&ppp,sizeof(playlistdata0));
			m_lc.RedrawItems(j-1,j);
				m_lc.SetItemState(j  ,m_lc.GetItemState(j  ,LVIS_SELECTED)|LVIS_SELECTED,LVIS_SELECTED);
				m_lc.SetItemState(j-1,m_lc.GetItemState(j-1,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
				i2=1;
			}
		}
		if(i2==0) break;
	}	
//	m_lc.RedrawWindow();
	Save();
}

void CPlayList::OnDOWN()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int i=m_lc.GetItemCount();
	CString s,s1;
	for(int j=i-1;j>0;j--){
		if((m_lc.GetItemState(j-1,LVIS_SELECTED)&LVIS_SELECTED)&&!(m_lc.GetItemState(j,LVIS_SELECTED)&LVIS_SELECTED)){
			playlistdata0 ppp;
			pc[j].icon=pc[j-1].icon=1;
			memcpy(&ppp,&pc[j-1],sizeof(playlistdata0));
			memcpy(&pc[j-1],&pc[j],sizeof(playlistdata0));
			memcpy(&pc[j],&ppp,sizeof(playlistdata0));
			m_lc.RedrawItems(j-1,j);

			m_lc.SetItemState(j  ,m_lc.GetItemState(j  ,LVIS_SELECTED)|LVIS_SELECTED,LVIS_SELECTED);
			m_lc.SetItemState(j-1,m_lc.GetItemState(j-1,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
		}
	}
//	m_lc.RedrawWindow();
	Save();
}

void CPlayList::OnXCHG(int i,int j)
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
			playlistdata0 ppp;
			pc[j].icon=pc[j-1].icon=1;
			memcpy(&ppp,&pc[i],sizeof(playlistdata0));
			memcpy(&pc[i],&pc[j],sizeof(playlistdata0));
			memcpy(&pc[j],&ppp,sizeof(playlistdata0));
}

extern COggDlg *og;
extern CString filen,fnn;

extern int modesub,loop1,loop2,ret2;

void CPlayList::Get(int i)
{
		fnn=pc[i].name; filen=pc[i].fol; modesub=pc[i].sub; loop1=pc[i].loop1; loop2=pc[i].loop2; ret2=pc[i].ret2;
		for(int k=0;k<playcnt;k++){
			m_lc.SetItemState(k,m_lc.GetItemState(k,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
		}
		m_lc.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		SIcon(i);
}

void CPlayList::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(pLVKeyDow->wVKey == VK_DELETE){
		Del();
	}
	*pResult = 0;
}


void CPlayList::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	TCHAR filen_c[1024];
	syo=0;syos="";
	int ii=m_lc.GetItemCount();
	UINT cnt = DragQueryFile(hDropInfo,(UINT)-1,filen_c,sizeof(filen_c));
	TCHAR tmp[1024];
	_tgetcwd(tmp,1000);
		for(UINT i=0;i<cnt;i++){
			DragQueryFile(hDropInfo,(UINT)i,filen_c,sizeof(filen_c));
			Fol(filen_c);
		}
	_tchdir(tmp);
	if(syo==1 && (fade1==1 || playf==0) && !pMediaPosition){
		plcnt=ii;
		SIcon(ii);
	}
	if(syo==1 && m_renzoku.GetCheck()==FALSE){
		plcnt=ii;
		SIcon(ii);
		filen = syos;
		og->dp(filen);
	}
	if(syo==1 && pMediaPosition){
		if(mode==-2 || videoonly==TRUE){
			REFTIME aa,bb;
			pMediaPosition->get_CurrentPosition(&aa);
			pMediaPosition->get_Duration(&bb);
			if(aa>=bb){
				filen = syos;
				og->dp(filen);
			}
		}
	}
	if(syo==1 && (fade1==1 || playf==0) && !pMediaPosition){
		filen = syos;
		og->dp(filen);
	}
	Save();
	CDialog::OnDropFiles(hDropInfo);
}

#include "Id3tagv1.h"
#include "Id3tagv2.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
OggVorbis_File vf1;

void CPlayList::Fol(CString fname)
{
	CString ft;ft="*.*";
	CString s,ss;
	playlistdata p;ZeroMemory(&p,sizeof(p));
	if(_tchdir(fname)==-1){
		CString ff=fname.Left(fname.ReverseFind('\\')-1);
		_tchdir(ff);
		ft=fname.Right(fname.GetLength()-fname.ReverseFind('\\')-1);
		if(ft.Right(4)==".ogg" || ft.Right(4)==".OGG"){
			p.sub=-1;
			FILE *fp;
			fp = _tfopen(fname, _T("rb"));
			if(fp == NULL){
				return;
			}
			if(ov_open(fp, &vf1, NULL, 0) < 0) {
				fclose(fp);
				return;
			}
			CString cc;
			_tcscpy(p.name,ft);
			p.alb[0]=p.art[0]=NULL;
			for(int iii=0;iii<vf1.vc->comments;iii++){
#if _UNICODE
				WCHAR f[1024];
				MultiByteToWideChar(CP_UTF8,0,vf1.vc->user_comments[iii],-1,f,1024);
				cc=f;
#else
				cc=vf1.vc->user_comments[iii];
#endif
				if(cc.Left(6)=="TITLE=")
				{
#if _UNICODE
					ss=UTF8toUNI(cc.Mid(6));
#else
					ss=UTF8toSJIS(cc.Mid(6));
#endif
					_tcscpy(p.name,ss);
				}
				if(cc.Left(7)=="ARTIST=")
				{
#if _UNICODE
					ss=UTF8toUNI(cc.Mid(7));
#else
					ss=UTF8toSJIS(cc.Mid(7));
#endif
					_tcscpy(p.art,ss);
				}
				if(cc.Left(6)=="ALBUM=")
				{
#if _UNICODE
					ss=UTF8toUNI(cc.Mid(6));
#else
					ss=UTF8toSJIS(cc.Mid(6));
#endif
					_tcscpy(p.alb,ss);
				}
			}
			ov_clear(&vf1);
			fclose(fp);
			_tcscpy(p.fol,fname);
			p.loop1=p.loop2=0;
		}else if(ft.Right(4)==".mp3" || ft.Right(4)==".MP3" || ft.Right(4)==".mp2" || ft.Right(4)==".MP2" ||
			ft.Right(4)==".mp1" || ft.Right(4)==".MP1" || ft.Right(4)==".rmp" || ft.Right(4)==".RMP"){
			p.sub=-10;p.loop1=p.loop2=0;
			_tcscpy(p.fol,fname);
			CId3tagv1 ta1p;
			CId3tagv2 ta2p;
			int b=ta2p.Load(fname);
			ss=ta2p.GetArtist();if(b==-1){ta1p.Load(fname); ss=ta1p.GetArtist();} _tcscpy(p.art,ss);
			ss=ta2p.GetTitle(); if(b==-1) ss=ta1p.GetTitle();if(ss=="")ss=ft; _tcscpy(p.name,ss);
			ss=ta2p.GetAlbum(); if(b==-1) ss=ta1p.GetAlbum(); _tcscpy(p.alb,ss);
		}else {
			p.sub=-2;
			_tcscpy(p.name,ft);
			_tcscpy(p.fol,fname);
			p.alb[0]=p.art[0]=NULL;p.loop1=p.loop2=0;
			TCHAR kpi[512];kpi[0]=0;
			plugs(fname,&p,kpi);
			if(kpi[0]){
				ft=fname.Left(fname.ReverseFind('.'));ft+=".m3u";
				ss=fname.Right(4);ss.MakeLower();
				if(ss==".kss"){
					CStdioFile f; if(f.Open(ft,CFile::modeRead|CFile::typeText,NULL)){
						TCHAR buf[256];int st,ed,tmp;
						for(;;){
							if(f.ReadString(buf,256)==FALSE) break;
							if(buf[0]==_T('#') || buf[0]==_T('\r') || buf[0]==_T('\n')) continue;
							ss=buf;
							st=ss.Find(',',0);ed=ss.Find(',',st+1); s=ss.Mid(st+1,(ed-1)-st);
							if(s.Left(1)==_T("$")){
								int num=0;
								CString s3=s.Mid(1,1);
								if(_T("0")<=s3 && _T("9")>=s3) num=s3.GetAt(0)-_T('0');
								if(_T("a")<=s3 && _T("f")>=s3) num=s3.GetAt(0)-_T('a')+10;
								if(_T("A")<=s3 && _T("F")>=s3) num=s3.GetAt(0)-_T('A')+10;
								s3=s.Mid(2,1);num*=10;
								if(_T("0")<=s3 && _T("9")>=s3) num+=s3.GetAt(0)-_T('0');
								if(_T("a")<=s3 && _T("f")>=s3) num+=s3.GetAt(0)-_T('a')+10;
								if(_T("A")<=s3 && _T("F")>=s3) num+=s3.GetAt(0)-_T('A')+10;
								ft.Format(_T("%s::%04d"),fname,num+1);
							}else
								ft.Format(_T("%s::%04d"),fname,_tstoi(s)+1);
							_tcscpy(p.fol,ft);						
							st=ss.Find(',',ed);ed=ss.Find(',',st+1); s=ss.Mid(st+1,(ed-1)-st);
							_tcscpy(p.name,s);
							if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;	fnn = p.name;}
							Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
						}
						f.Close();
						return;
					}
				}
				ft=fname.Left(fname.ReverseFind('.'));ft+=".frm";
				if(ss==".nsf"){
					CStdioFile f; if(f.Open(ft,CFile::modeRead|CFile::typeText,NULL)){
						TCHAR buf[256];int st,ed,tmp;
						f.ReadString(buf,256);
						f.ReadString(buf,256);
						_tcscpy(p.alb,buf);
						f.ReadString(buf,256);
						s=buf;int j=s.Find(_T("songs"));if(j>=0){
							int k=s.Find(_T("S.E."));
							int l=s.ReverseFind('(');ss=s.Mid(l+1,3);j=_tstoi(ss);
							if(k>=0){l=s.ReverseFind('&');ss=s.Mid(l+1,3);j+=_tstoi(ss);}
							for(l=0;l<j;l++){
								s=fname.Right(fname.GetLength()-fname.ReverseFind('\\')-1);
								ss.Format(_T("%s::%04d"),s,l+1);
								_tcscpy(p.name,ss);
								ss.Format(_T("%s::%04d"),fname,l+1);
								_tcscpy(p.fol,ss);
								if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;	fnn = p.name;}
								Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
							}
						}
						f.Close();
						return;
					}
				}					
				if(ss==".gbs"){
					CFile f; if(f.Open(fname,CFile::modeRead,NULL)){
						char buf[32];
						f.Read(buf,16);int i=buf[4];
						f.Read(buf,32);
#if UNICODE
						TCHAR ss1[512];
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
						_tcscpy(p.name,ss1);
#else
						_tcscpy(p.name,buf);
#endif
						f.Read(buf,32);
#if UNICODE
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
						_tcscpy(p.alb,ss1);
#else
						_tcscpy(p.alb,buf);
#endif
						f.Read(buf,32);
#if UNICODE
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
						_tcscpy(p.art,ss1);
#else
						_tcscpy(p.art,buf);
#endif
						f.Close();
						for(int j=0;j<i;j++){
							ss.Format(_T("%s::%04d"),fname,j+1);_tcscpy(p.fol,ss);
							if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;	fnn = p.name;}
							Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
						}
						return;
					}
				}
				if(ss==".hes"){
					for(int i=1;i<257;i++){
						ft.Format(_T("%s::%04d"),fname,i);
						_tcscpy(p.fol,ft);
						ss=fname.Right(fname.GetLength()-fname.ReverseFind('\\')-1);
						ft.Format(_T("%s::%04d"),ss,i);
						_tcscpy(p.name,ft);						
						if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
						Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
					}
					return;
				}
				if(ss==".ovi"||ss==".opi"||ss==".ozi"){
					CFile f;char buf[512],*buf2;
					f.Open(fname,CFile::modeRead|CFile::shareDenyRead,NULL);
					if(f.GetLength()>512)
						f.Seek(-512,CFile::end);
					else
						f.SeekToBegin();
					f.Read(buf,512);
					f.Close();
					for(int i=0;i<500;i++){
						if(buf[i]=='F'&&buf[i+1]=='M'&&buf[i+2]=='C') break;
					}
					if(i!=500){
						buf2=buf+i+4; ss=buf2;
						int st=ss.Find(0x0d,0);
						ft=ss.Left(st);_tcscpy(p.name,ft);
						int ed=ss.Find(0x0d,st+2);
						ft=ss.Mid(st+1,ed-st-1);_tcscpy(p.art,ft);
						st=ss.Find(0x0d,ed+2);
						ft=ss.Mid(ed+1,st-ed-1);_tcscpy(p.alb,ft);
						if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
						Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
						return;
					}
				}
				ss=fname.Right(2);ss.MakeLower();
				ft=fname.Right(3);ft.MakeLower();
				if(ss==".m" || ft==".mz"){
					CFile ff;char buf[512],*buf2;
					ff.Open(fname,CFile::modeRead|CFile::shareDenyNone,NULL);
					if(ff.GetLength()>512)
						ff.Seek(-512,CFile::end);
					else
						ff.SeekToBegin();
					ff.Read(buf,512);
					int jj=ff.GetLength();if(jj>510) jj=510;
					jj-=3;
					for(int i=jj;i>0;i--){
						if(buf[i]==0&&(buf[i+1]==0||(BYTE)buf[i+1]==255)&&buf[i+2]==0)break;
					}
					ff.Close();
					if(i!=0){
						buf2=buf+i+3;
						for(int j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
						TCHAR ss1[512];
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
						_tcscpy(p.name,ss1);buf2+=j;
#else
						_tcscpy(p.name,buf2);buf2+=j;
#endif
						for(j=0;;j++)if(buf2[j]!=0)break;
						buf2+=j;
						for(j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
						_tcscpy(p.art,ss1);buf2+=j;
#else
						_tcscpy(p.art,buf2);buf2+=j;
#endif
						for(j=0;;j++)if(buf2[j]!=0)break;
						buf2+=j;
						for(j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
						_tcscpy(p.alb,ss1);buf2+=j;
#else
						_tcscpy(p.alb,buf2);buf2+=j;
#endif
						if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
						Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
					}
					return;
				}
				ft=fname.Right(4);ft.MakeLower();
				if(ft==".tta"){
					CFile ff;char buf[512],*buf2;
					ff.Open(fname,CFile::modeRead|CFile::shareDenyNone,NULL);
					if(ff.GetLength()>0x80)
						ff.Seek(-0x80,CFile::end);
					else
						ff.SeekToBegin();
					ff.Read(buf,0x80);
					for(int i=0;i<0x80;i++){
						if(buf[i+0]=='T'&&buf[i+1]=='A'&&buf[i+2]=='G')break;
					}
					ff.Close();
					if(i!=0x80){
						buf2=buf+i+3;
#if UNICODE
						TCHAR ss1[512];
						TCHAR buf3=buf2[30]; buf2[30]=0;
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
						buf2[30]=buf3;
						_tcscpy(p.name,ss1);buf2+=30;
#else
						_tcscpy(p.name,buf2);buf2+=30;
#endif
#if UNICODE
						buf3=buf2[30]; buf2[30]=0;
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
						buf2[30]=buf3;
						_tcscpy(p.art,ss1);buf2+=30;
#else
						_tcscpy(p.art,buf2);buf2+=30;
#endif
#if UNICODE
						buf3=buf2[30]; buf2[30]=0;
						MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
						buf2[30]=buf3;
						_tcscpy(p.alb,ss1);buf2+=30;
#else
						_tcscpy(p.alb,buf2);buf2+=30;
#endif
						if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
						Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
					}
					return;
				}
			}

		}
		s=fname;s.MakeLower();
		if(s.Right(4)==".png" || s.Right(4)==".url" || s.Right(4)==".jpg" || s.Right(4)==".bmp" || s.Right(4)==".cue" || s.Right(4)==".iso" || s.Right(4)==".bin" || s.Right(4)==".img" || s.Right(4)==".mds" || s.Right(4)==".mdf" || s.Right(4)==".ccd" || s.Right(4)==".sub" || s.Right(4)==".pdf" || s.Right(4)==".com" || s.Right(4)==".exe" || s.Right(4)==".dll" || s.Right(4)==".bat" || s.Right(4)==".reg" || s.Right(4)==".msi" || s.Right(4)==".nfo" || s.Right(4)==".diz" || s.Right(4)==".gif" || s.Right(4)==".ico" ||
		   s.Right(4)==".zip" || s.Right(4)==".lzh" || s.Right(4)==".cab" || s.Right(4)==".rar" || s.Right(4)==".txt" || s.Right(4)==".doc" || s.Right(4)=="html" || s.Right(4)==".htm" || s.Right(4)==".ini" || s.Right(4)==".xml" || s.Right(4)==".kar" || s.Right(4)==".hed" || s.Right(4)==".mzi" || s.Right(4)==".mag" || s.Right(4)==".mvi" || s.Right(4)==".lvi" || s.Right(4)==".mpi" || s.Right(4)==".pvi" || s.Right(4)==".pzi" || s.Right(4)==".p86" || s.Right(4)==".mml" || s.Right(4)==".m3u" || s.Right(4)==".frm" || s.Right(7)==".psflib" || s.Right(8)==".psf2lib" || s.Right(7)==".usflib" || s.Right(7)==".2sflib" || s.Right(3)==".gb" || s.Right(7)==".gsflib" || s.Right(4)==".pdx"){}else{
			if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
			Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
		}
		return;
	}
	CFileFind f;
	if(f.FindFile(ft)){
		int b=1;
		for(;b;){
			b=f.FindNextFile();
			s=f.GetFileName();
			if(f.IsDirectory()==0){
				if(s.Right(4)==".ogg" || s.Right(4)==".OGG"){
					p.sub=-1;
					FILE *fp;
					fp = _tfopen(f.GetFilePath(), _T("rb"));
					if(fp == NULL){
						continue;
					}
					if(ov_open(fp, &vf1, NULL, 0) < 0) {
						fclose(fp);
						continue;
					}
					CString cc;
					_tcscpy(p.name,s);
					for(int iii=0;iii<vf1.vc->comments;iii++){
						cc=vf1.vc->user_comments[iii];
						if(cc.Left(6)=="TITLE=")
						{
#if _UNICODE
							ss=UTF8toUNI(cc.Mid(6));
#else
							ss=UTF8toSJIS(cc.Mid(6));
#endif
							_tcscpy(p.name,ss);
						}
						if(cc.Left(7)=="ARTIST=")
						{
#if _UNICODE
							ss=UTF8toUNI(cc.Mid(7));
#else
							ss=UTF8toSJIS(cc.Mid(7));
#endif
							_tcscpy(p.art,ss);
						}
						if(cc.Left(6)=="ALBUM=")
						{
#if _UNICODE
							ss=UTF8toUNI(cc.Mid(6));
#else
							ss=UTF8toSJIS(cc.Mid(6));
#endif
							_tcscpy(p.alb,ss);
						}
					}
					ov_clear(&vf1);
					fclose(fp);
					s=f.GetFilePath();
					_tcscpy(p.fol,s);
					p.alb[0]=p.art[0]=NULL;p.loop1=p.loop2=0;
				}else if((s.Right(4)==".mp3" || s.Right(4)==".MP3" || s.Right(4)==".mp2" || s.Right(4)==".MP2" ||
					s.Right(4)==".mp1" || s.Right(4)==".MP1" || s.Right(4)==".rmp" || s.Right(4)==".RMP")){
					p.sub=-10;p.loop1=p.loop2=0;
					s=f.GetFilePath();
					_tcscpy(p.fol,s);
					CId3tagv1 ta1p;
					CId3tagv2 ta2p;
					int b=ta2p.Load(s);
					ss=ta2p.GetArtist();if(b==-1){ta1p.Load(s); ss=ta1p.GetArtist();} _tcscpy(p.art,ss);
					ss=ta2p.GetTitle(); if(b==-1) ss=ta1p.GetTitle();if(ss=="")ss=f.GetFileName(); _tcscpy(p.name,ss);
					ss=ta2p.GetAlbum(); if(b==-1) ss=ta1p.GetAlbum(); _tcscpy(p.alb,ss);
				}else{
					p.sub=-2;
					_tcscpy(p.name,s);
					s=f.GetFilePath();
					_tcscpy(p.fol,s);
					p.alb[0]=p.art[0]=NULL;p.loop1=p.loop2=0;
					HKMP kmp=NULL;
					KMPMODULE *mod=NULL;
					TCHAR kpi[512]={0};
					plugs(s,&p,kpi);
					if(kpi[0]){
						ss=f.GetFilePath();
						ft=ss.Left(ss.ReverseFind('.'));ft+=".m3u";
						ss=f.GetFileName().Right(4);ss.MakeLower();
						if(ss==".kss"){
							CStdioFile f; if(f.Open(ft,CFile::modeRead|CFile::typeText,NULL)){
								TCHAR buf[256];int st,ed,tmp;
								for(;;){
									if(f.ReadString(buf,256)==FALSE) break;
									if(buf[0]=='#' || buf[0]==10) continue;
									ss=buf;
									st=ss.Find(',',0);ed=ss.Find(',',st+1); s=ss.Mid(st+1,(ed-1)-st);
									if(s.Left(1)==_T("$")){
										int num=0;
										CString s3=s.Mid(1,1);
										if(_T("0")<=s3 && _T("9")>=s3) num=s3.GetAt(0)-_T('0');
										if(_T("a")<=s3 && _T("f")>=s3) num=s3.GetAt(0)-_T('a')+10;
										if(_T("A")<=s3 && _T("F")>=s3) num=s3.GetAt(0)-_T('A')+10;
										s3=s.Mid(2,1);num*=10;
										if(_T("0")<=s3 && _T("9")>=s3) num+=s3.GetAt(0)-_T('0');
										if(_T("a")<=s3 && _T("f")>=s3) num+=s3.GetAt(0)-_T('a')+10;
										if(_T("A")<=s3 && _T("F")>=s3) num+=s3.GetAt(0)-_T('A')+10;
										ft.Format(_T("%s::%04d"),fname,num+1);
									}else
										ft.Format(_T("%s::%04d"),fname,_tstoi(s)+1);
									_tcscpy(p.fol,ft);						
									st=ss.Find(',',ed);ed=ss.Find(',',st+1); s=ss.Mid(st+1,(ed-1)-st);
									_tcscpy(p.name,s);						
									if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;	fnn = p.name;}
									Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
								}
								f.Close();
								continue;
							}
						}
						ss=f.GetFilePath();
						ft=ss.Left(ss.ReverseFind('.'));ft+=".frm";
						ss=f.GetFileName().Right(4);ss.MakeLower();
						if(ss==".nsf"){
							CStdioFile ff; if(ff.Open(ft,CFile::modeRead|CFile::typeText,NULL)){
								TCHAR buf[256];int st,ed,tmp;
								ff.ReadString(buf,256);
								ff.ReadString(buf,256);
//#if UNICODE
//								TCHAR ss1[512];
//								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
//								_tcscpy(p.alb,ss1);
//#else
								_tcscpy(p.alb,buf);
//#endif
								ff.ReadString(buf,256);
								s=buf;int j=s.Find(_T("songs"));if(j>=0){
									int k=s.Find(_T("S.E."));
									int l=s.ReverseFind('(');ss=s.Mid(l+1,3);j=_tstoi(ss);
									if(k>=0){l=s.ReverseFind('&');ss=s.Mid(l+1,3);j+=_tstoi(ss);}
									for(l=0;l<j;l++){
										ss=f.GetFilePath();
										s=ss.Right(ss.GetLength()-ss.ReverseFind('\\')-1);
										ss.Format(_T("%s::%04d"),s,l+1);
										_tcscpy(p.name,ss);
										ss.Format(_T("%s::%04d"),f.GetFilePath(),l+1);
										_tcscpy(p.fol,ss);
										if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;	fnn = p.name;}
										Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
									}
								}
								ff.Close();
								continue;
							}
						}					
						if(ss==".gbs"){
							CFile ff; if(ff.Open(f.GetFilePath(),CFile::modeRead,NULL)){
								char buf[32];
								ff.Read(buf,16);int i=buf[4];
								ff.Read(buf,32);
#if UNICODE
								TCHAR ss1[512];
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
								_tcscpy(p.name,ss1);
#else
								_tcscpy(p.name,buf);
#endif
								ff.Read(buf,32);
#if UNICODE
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
								_tcscpy(p.alb,ss1);
#else
								_tcscpy(p.alb,buf);
#endif
								ff.Read(buf,32);
#if UNICODE
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,ss1,2000);
								_tcscpy(p.art,ss1);
#else
								_tcscpy(p.art,buf);
#endif
								ff.Close();
								for(int j=0;j<i;j++){
									ss.Format(_T("%s::%04d"),f.GetFilePath(),j+1);_tcscpy(p.fol,ss);
									if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;	fnn = p.name;}
									Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
								}
								continue;
							}
						}
						if(ss==".hes"){
							for(int i=1;i<257;i++){
								ft.Format(_T("%s::%04d"),f.GetFileName(),i);
								_tcscpy(p.name,ft);
								ft.Format(_T("%s::%04d"),f.GetFilePath(),i);
								if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
								_tcscpy(p.fol,ft);						
								Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
							}
							continue;
						}
						if(ss==".ovi"||ss==".opi"||ss==".ozi"){
							CFile ff;char buf[512],*buf2;
							ff.Open(f.GetFilePath(),CFile::modeRead|CFile::shareDenyNone,NULL);
							if(ff.GetLength()>512)
								ff.Seek(-512,CFile::end);
							else
								ff.SeekToBegin();
							ff.Read(buf,512);
							ff.Close();
							for(int i=0;i<500;i++){
								if(buf[i]=='F'&&buf[i+1]=='M'&&buf[i+2]=='C') break;
							}
							if(i!=500){
								buf2=buf+i+4; ss=buf2;
								int st=ss.Find(0x0d,0);
								ft=ss.Left(st);_tcscpy(p.name,ft);
								int ed=ss.Find(0x0d,st+2);
								ft=ss.Mid(st+1,ed-st-1);_tcscpy(p.art,ft);
								st=ss.Find(0x0d,ed+2);
								ft=ss.Mid(ed+1,st-ed-1);_tcscpy(p.alb,ft);
								if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
								Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
								continue;
							}
						}
						ss=f.GetFileName().Right(2);ss.MakeLower();
						ft=f.GetFileName().Right(3);ft.MakeLower();
						if(ss==".m" || ft==".mz"){
							CFile ff;char buf[512],*buf2;
							ff.Open(f.GetFilePath(),CFile::modeRead|CFile::shareDenyNone,NULL);
							if(ff.GetLength()>512)
								ff.Seek(-512,CFile::end);
							else
								ff.SeekToBegin();
							ff.Read(buf,512);
							int jj=ff.GetLength();if(jj>510) jj=510;
							jj-=3;
							for(int i=jj;i>0;i--){
								if(buf[i]==0&&buf[i+1]==0&&buf[i+2]==0)break;
							}
							ff.Close();
							if(i!=0){
								buf2=buf+i+3;
								for(int j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
								TCHAR ss1[512];
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
								_tcscpy(p.name,ss1);buf2+=j;
#else
								_tcscpy(p.name,buf2);buf2+=j;
#endif
								for(j=0;;j++)if(buf2[j]!=0)break;
								buf2+=j;
								for(j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
								_tcscpy(p.art,ss1);buf2+=j;
#else
								_tcscpy(p.art,buf2);buf2+=j;
#endif
								for(j=0;;j++)if(buf2[j]!=0)break;
								buf2+=j;
								for(j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
								_tcscpy(p.alb,ss1);buf2+=j;
#else
								_tcscpy(p.alb,buf2);buf2+=j;
#endif
								if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
								Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
								continue;
							}
						}
						ft=f.GetFileName().Right(4);ft.MakeLower();
						if(ft==".tta"){
							CFile ff;char buf[512],*buf2;
							ff.Open(f.GetFilePath(),CFile::modeRead|CFile::shareDenyNone,NULL);
							if(ff.GetLength()>0x80)
								ff.Seek(-0x80,CFile::end);
							else
								ff.SeekToBegin();
							ff.Read(buf,0x80);
							for(int i=0;i<0x80;i++){
								if(buf[i+0]=='T'&&buf[i+1]=='A'&&buf[i+2]=='G')break;
							}
							ff.Close();
							if(i!=0x80){
								buf2=buf+i+3;
								for(int j=0;;j++)if(buf2[j]==0)break;
#if UNICODE
								TCHAR ss1[512];
								TCHAR buf3=buf2[30]; buf2[30]=0;
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
								buf2[30]=buf3;
								_tcscpy(p.name,ss1);buf2+=30;
#else
								_tcscpy(p.name,buf2);buf2+=30;
#endif
#if UNICODE
								buf3=buf2[30]; buf2[30]=0;
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
								buf2[30]=buf3;
								_tcscpy(p.art,ss1);buf2+=30;
#else
								_tcscpy(p.art,buf2);buf2+=30;
#endif
#if UNICODE
								buf3=buf2[30]; buf2[30]=0;
								MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf2,-1,ss1,2000);
								buf2[30]=buf3;
								_tcscpy(p.alb,ss1);buf2+=30;
#else
								_tcscpy(p.alb,buf2);buf2+=30;
#endif
								if(syo==0){	syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;	}
								Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
								continue;
							}
						}

					}
				}
				s.MakeLower();
				if(s.Right(4)==".png" || s.Right(4)==".url" || s.Right(4)==".jpg" || s.Right(4)==".bmp" || s.Right(4)==".cue" || s.Right(4)==".iso" || s.Right(4)==".bin" || s.Right(4)==".img" || s.Right(4)==".mds" || s.Right(4)==".mdf" || s.Right(4)==".ccd" || s.Right(4)==".sub" || s.Right(4)==".pdf" || s.Right(4)==".com" || s.Right(4)==".exe" || s.Right(4)==".dll" || s.Right(4)==".bat" || s.Right(4)==".reg" || s.Right(4)==".msi" || s.Right(4)==".nfo" || s.Right(4)==".diz" || s.Right(4)==".gif" || s.Right(4)==".ico" ||
				   s.Right(4)==".zip" || s.Right(4)==".lzh" || s.Right(4)==".cab" || s.Right(4)==".rar" || s.Right(4)==".txt" || s.Right(4)==".doc" || s.Right(4)=="html" || s.Right(4)==".htm" || s.Right(4)==".ini" || s.Right(4)==".xml" || s.Right(4)==".kar" || s.Right(4)==".hed" || s.Right(4)==".mzi" || s.Right(4)==".mag" || s.Right(4)==".mvi" || s.Right(4)==".lvi" || s.Right(4)==".mpi" || s.Right(4)==".pvi" || s.Right(4)==".pzi" || s.Right(4)==".p86" || s.Right(4)==".mml" || s.Right(4)==".m3u" || s.Right(4)==".frm" || s.Right(7)==".psflib" || s.Right(8)==".psf2lib" || s.Right(7)==".usflib" || s.Right(7)==".2sflib" || s.Right(3)==".gb" || s.Right(7)==".gsflib" || s.Right(4)==".pdx"){}else{
					if(syo==0){
						syo=1;syos=p.fol;modesub=p.sub;fnn = p.name;
					}
					Add(p.name,p.sub,p.loop1,p.loop2,p.art,p.alb,p.fol,0,0);
				}
			}
		}
	}
	f.Close();
	CString dir[1000];
	int cdd=0;
	CFileFind cf1;
	if(cf1.FindFile(_T("*.*")) != 0){
		int r=1;
		for(;r;){
			r=cf1.FindNextFile();
			CString ss,sss;
			ss=cf1.GetFileName();
			sss=cf1.GetFilePath();
			if(!(ss =="." || ss == "..")){
				if((cf1.IsHidden()==0)){
					if(cf1.IsDirectory()!=0){ //�t�H���_�H
//						if(fname.GetLength()==3)
							dir[cdd]=cf1.GetFilePath();//*/fname+cf1.GetFileName();
//						else
//							dir[cdd]=cf1.GetFilePath();//*/fname+"\\"+cf1.GetFileName();
						cdd++;
					}
				}
			}
		}
	}
	cf1.Close();
	for(int k=0;k<cdd;k++)
		Fol(dir[k]);
}




void CPlayList::plugs(CString fff, playlistdata *p,TCHAR* kpi)
{
	CString ss,ft;
	int flg=0;
	for(int i=0;i<kpicnt;i++){
		for(int j=0;;j++){
			if(ext[i][j]=="") break;
			ss=fff.Right(fff.GetLength()-fff.ReverseFind('.'));ss.MakeLower();
			if(ext[i][j]==ss){ss=kpif[i];flg=1;break;}
		}
		if(flg==1)break;
	}
	if(flg==1){
		_tcscpy(p->fol,fff);
		p->sub=-3;
		ft=fff.Right(fff.GetLength()-fff.ReverseFind('\\')-1);
		_tcscpy(p->name,ft);
		p->alb[0]=NULL;p->art[0]=NULL;p->loop1=p->loop2=p->ret2=0;
		_tcscpy(kpi,ss);
	}
}

void CPlayList::Save()
{
	TCHAR tmp[1024];int cnt,j;CString s;
	int cx,cy,x,y;RECT r;
	int c;
	_tgetcwd(tmp,1000);
	_tchdir(karento2);
	if(IsIconic()){
		ShowWindow(SW_RESTORE);
		GetWindowRect(&r);
		ShowWindow(SW_MINIMIZE);
	}else
		GetWindowRect(&r);
	x=r.left;y=r.top;cx=r.right-x;cy=r.bottom-y;
#if _UNICODE
	CFile f;if(f.Open(_T("playlistu.dat"),CFile::modeCreate|CFile::modeWrite,NULL)==TRUE){
#else
	CFile f;if(f.Open(_T("playlist.dat"),CFile::modeCreate|CFile::modeWrite,NULL)==TRUE){
#endif
		cnt=playcnt;
		f.Write(&cnt,4);
		f.Write(&x,4);
		f.Write(&y,4);
		f.Write(&cx,4);
		f.Write(&cy,4);
		c=m_lc.GetColumnWidth(0);f.Write(&c,4);
		c=m_lc.GetColumnWidth(1);f.Write(&c,4);
		c=m_lc.GetColumnWidth(3);f.Write(&c,4);
		c=m_lc.GetColumnWidth(4);f.Write(&c,4);
		c=m_lc.GetColumnWidth(7);f.Write(&c,4);
		playlistdata pld;
		for(int i=0;i<cnt;i++){ZeroMemory(&pld,sizeof(pld));
			_tcscpy(pld.alb,pc[i].alb);
			_tcscpy(pld.art,pc[i].art);
			_tcscpy(pld.fol,pc[i].fol);
			_tcscpy(pld.name,pc[i].name);
			pld.loop1=pc[i].loop1;
			pld.loop2=pc[i].loop2;
			pld.sub=pc[i].sub;
			pld.ret2=pc[i].ret2;
			pld.time=pc[i].time;
			f.Write(&pld,sizeof(pld));
		}
		c=m_loop.GetCheck();f.Write(&c,4);
		c=m_renzoku.GetCheck();f.Write(&c,4);
		c=m_tool.GetCheck();f.Write(&c,4);
		c=m_saisyo.GetCheck();f.Write(&c,4);
		c=m_lc.GetColumnWidth(2);f.Write(&c,4);
		c=m_lc.GetColumnWidth(5);f.Write(&c,4);
		f.Write(&pnt,4);
		f.Close();
	}
	_tchdir(tmp);
}

void CPlayList::Load()
{
	TCHAR tmp[1024];int cnt;
	int cx,cy,x=-10000,y,c;
	_tgetcwd(tmp,1000);
	_tchdir(karento2);
#if _UNICODE
	CFile f;if(f.Open(_T("playlistu.dat"),CFile::modeRead,NULL)==TRUE){
#else
	CFile f;if(f.Open(_T("playlist.dat"),CFile::modeRead,NULL)==TRUE){
#endif
		f.Read(&cnt,4);
		f.Read(&x,4);
		f.Read(&y,4);
		f.Read(&cx,4);
		f.Read(&cy,4);
		f.Read(&c,4);m_lc.SetColumnWidth(0,c);
		f.Read(&c,4);m_lc.SetColumnWidth(1,c);
		f.Read(&c,4);m_lc.SetColumnWidth(3,c);
		f.Read(&c,4);m_lc.SetColumnWidth(4,c);
		f.Read(&c,4);
		playlistdata pld;
		m_lc.SetItemCount(cnt);
		pc = (playlistdata0*)malloc(sizeof(playlistdata0)*(cnt+1));
		for(int i=0;i<cnt;i++){
			f.Read(&pld,sizeof(pld));
			Add(pld.name,pld.sub,pld.loop1,pld.loop2,pld.art,pld.alb,pld.fol,pld.ret2,pld.time,FALSE,FALSE);			
		}
		c=0;f.Read(&c,4);m_loop.SetCheck(c);
		c=0;f.Read(&c,4);m_renzoku.SetCheck(c);
		c=1;f.Read(&c,4);m_tool.SetCheck(c);
		c=1;f.Read(&c,4);m_saisyo.SetCheck(c);
		c=-1;f.Read(&c,4);if(c!=-1)m_lc.SetColumnWidth(2,c);
		c=-1;f.Read(&c,4);if(c!=-1)m_lc.SetColumnWidth(5,c);
		pnt1=-1;f.Read(&pnt1,4);//if(c!=-1)SIcon(pnt);
		f.Close();
	}
	_tchdir(tmp);
	if(x!=-10000){
		MoveWindow(x,y,cx,cy,TRUE);
		RECT r;
		GetClientRect(&r);
	m_lc.SetWindowPos(&wndNoTopMost,0,0,r.right-20,r.bottom-45,SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	}
}
int SC=0;
void CPlayList::SIcon(int i){
	if(i<0) return;
	if(i>=playcnt) return;
	if(i==pnt) return;
	RECT r;
	pc[i].icon=0; if(pnt>=0&&pnt<playcnt){ pc[pnt].icon=1;
			m_lc.GetItemRect(pnt,&r,LVIR_ICON);
			m_lc.RedrawWindow(&r);
	}
	pnt=i;
	m_lc.GetItemRect(pnt,&r,LVIR_ICON);
	m_lc.RedrawWindow(&r);
	m_lc.EnsureVisible(i,FALSE);
	KillTimer(30);
	SetTimer(30,1200,NULL);
	SC=0;
}

void CPlayList::SIconTimer(int i){
	if(pnt<0) return;
	if(pnt>=playcnt) return;
	if(IsBadReadPtr(&pc[pnt],sizeof(playlistdata0))) return;
	__try{
	if(i==0)
		pc[pnt].icon=2;
	else
		pc[pnt].icon=0;
	}__except(EXCEPTION_EXECUTE_HANDLER){}
	RECT r;
	m_lc.GetItemRect(pnt,&r,LVIR_ICON);
	m_lc.RedrawWindow(&r);
}
int pln=0;
extern int ps;
extern void DoEvent();
void CPlayList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;
	CString s;int i,j;
	int Lindex=-1;
	Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);i=Lindex;
	if(Lindex>=playcnt) return;
	if(Lindex==-1) return;
//	SIcon(i);
	fnn=pc[Lindex].name;
	filen=pc[Lindex].fol;
	modesub=pc[Lindex].sub;
	loop1=pc[Lindex].loop1;
	loop2=pc[Lindex].loop2;
	ret2=pc[Lindex].ret2;
	plcnt=i;
	if(pln==0){
		pln=1;
		og->OnRestart();
		for(;ps==1;){
			DoEvent();
			og->OnRestart();
		}
		pln=0;
	}
}
extern CDouga *pMainFrame1;
void CPlayList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	RECT r;
	GetClientRect(&r);
	if( ::IsWindow( this->GetSafeHwnd()) == TRUE &&  this->IsWindowVisible() == TRUE)
	m_lc.SetWindowPos(&wndNoTopMost,0,0,r.right-20,r.bottom-45,SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	if(plw){
		if (nType == SIZE_MINIMIZED){
			if(m_saisyo.GetCheck())
				og->ShowWindow(SW_MINIMIZE);
			if(pMainFrame1){
				pMainFrame1->ShowWindow(SW_HIDE);
			}
		}
		if(nType==SIZE_RESTORED){
			if(m_saisyo.GetCheck())
				og->ShowWindow(SW_RESTORE);
			if(pMainFrame1){
				pMainFrame1->ShowWindow(SW_SHOWNORMAL);
			}
		}
	}
}
int kk=0;
extern int lenl;
int tlg=0;
extern CPlayList*pl;
void timerpl(UINT nIDEvent);
void timerpl1(UINT nIDEvent);
void timerpl1(UINT nIDEvent)
{
	if(nIDEvent==50){
		pl->KillTimer(50);
		pl->SIcon(pl->pnt1);
	}
	if(nIDEvent==40){
		pl->KillTimer(40);
		plw=1;
	}

	if(nIDEvent==30){
		pl->SIconTimer(SC);
		SC++;SC=SC % 2;
	}
	if(nIDEvent==20){
		if(pl->w_flg==FALSE) return;
		if(pl->GetFocus()==NULL){return;}
		if(pl->m_find.GetFocus()->m_hWnd==pl->m_find.m_hWnd){return;}
		{
			if((GetKeyState(VK_RETURN)&0x8000)==0 && kk==1)
				kk=0;
			if(GetKeyState(VK_RETURN)&0x8000 && kk==0){
				kk=1;
				CString s;int i,j;
				int Lindex=-1;
				Lindex=pl->m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
				i=Lindex;
				if(Lindex>=playcnt) return;
				if(Lindex==-1) return;
//				pl->SIcon(i);
				fnn=pl->pc[Lindex].name;
				filen=pl->pc[Lindex].fol;
				modesub=pl->pc[Lindex].sub;
				loop1=pl->pc[Lindex].loop1;
				loop2=pl->pc[Lindex].loop2;
				ret2=pl->pc[Lindex].ret2;
				plcnt=i;
				og->OnRestart();
			}
			if((GetKeyState(VK_CONTROL)&0x8000) && (GetKeyState('A')&0x8000)){
				int i=pl->m_lc.GetItemCount();
				for(int j=0;j<i;j++){
					pl->m_lc.SetItemState(j,LVIS_SELECTED,LVIS_SELECTED);
				}
			}
		}
		int tl=pl->m_tool.GetCheck();
		if(tl!=tlg){
			tlg=tl;
			if(tlg){
				pl->m_lc.EnableToolTips(TRUE);
				tl=pl->m_lc.GetExtendedStyle();
				tl = tl & ~LVS_EX_INFOTIP;
				pl->m_lc.SetExtendedStyle(tl);
			}else{
				pl->m_lc.EnableToolTips(FALSE);
				tl=pl->m_lc.GetExtendedStyle();
				tl |=LVS_EX_INFOTIP;
				pl->m_lc.SetExtendedStyle(tl);
			}
		}
	}
}
void timerpl(UINT nIDEvent)
{
	try{
		_set_se_translator( trans_func1 );
		timerpl1(nIDEvent);
//	}__except(EXCEPTION_EXECUTE_HANDLER){}
	}catch(SE_Exception1 e){
	}
	catch(_EXCEPTION_POINTERS *ep){
	}
	catch(...){}
}

#if WIN64
void CPlayList::OnTimer(UINT_PTR nIDEvent) 
#else
void CPlayList::OnTimer(UINT nIDEvent) 
#endif
{
	timerpl(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}

void CPlayList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPlayList::OnBnClickedCheck4()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	Save();
}

void CPlayList::OnBnClickedCheck1()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	Save();
}

void CPlayList::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNM = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	POINT ptPos,ptPos2;
    HIMAGELIST hOneImageList;
    HIMAGELIST hTempImageList;
	IMAGEINFO imf;
	long iHeight;
	m_hDragImage = ListView_CreateDragImage(m_lc.m_hWnd,pNM->iItem,&ptPos);
	ImageList_GetImageInfo(m_hDragImage, 0, &imf);
	iHeight = imf.rcImage.bottom;
	for(int Lindex=-1;;){
		Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);//pNM->iItem
		if(Lindex==-1) break;
		if(pNM->iItem==Lindex){
		}else{
            hOneImageList= ListView_CreateDragImage(m_lc.m_hWnd,Lindex,&ptPos2);
            hTempImageList = ImageList_Merge(m_hDragImage, 
                             0, hOneImageList, 0, 0, iHeight);
            ImageList_Destroy(m_hDragImage);
            ImageList_Destroy(hOneImageList);
            m_hDragImage = hTempImageList;
            ImageList_GetImageInfo(m_hDragImage, 0, &imf);
            iHeight = imf.rcImage.bottom;		}
	}
 	// �h���b�O�J�n
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_lc.ScreenToClient(&ptCursor);

	long lX = ptCursor.x- ptPos.x;
	long lY = ptCursor.y- ptPos.y;

	ImageList_BeginDrag(m_hDragImage,0,lX,lY);
	ImageList_DragEnter(m_hWnd,0,0);
	SetCapture();


	*pResult = 0;
}

void CPlayList::OnDrag(int x,int y)
{
	POINT Point={x,y};
	ClientToScreen(&Point);
	RECT Rect;
	GetWindowRect(&Rect);
	ImageList_DragMove(Point.x-Rect.left,Point.y-Rect.top);
	{
		CPoint  point,point2;CRect rect;
		GetCursorPos(&point);
		ScreenToClient(&point);
		m_lc.GetWindowRect(&rect);
		point2.y=rect.top; point2.x=rect.left;
		ScreenToClient(&point2);
		point-=point2;
		int hItem = m_lc.HitTest(point ,NULL);
	}
}

void CPlayList::OnEndDrag()
{
	// �h���b�O�I��
	ImageList_DragLeave(m_hWnd);
	ImageList_EndDrag();
	ImageList_Destroy(m_hDragImage);
	m_hDragImage = NULL;

	// �J�[�\���\��
	ShowCursor(TRUE);
}
void CPlayList::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if(GetCapture()==this){
		OnDrag(point.x,point.y);
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CPlayList::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if(GetCapture()==this){
		OnEndDrag();
		// �L���v�`������
		ReleaseCapture();
		//���ۂ̈ړ��̂��߂̍��W���肾��
		CPoint  point,point2;CRect rect;
		GetCursorPos(&point);
		ScreenToClient(&point);
		m_lc.GetWindowRect(&rect);
		point2.y=rect.top; point2.x=rect.left;
		ScreenToClient(&point2);
		point-=point2;
		int hItem = m_lc.HitTest(point,NULL);
		if( hItem != -1){
			playlistdata *p;int cnt=0,j,cnt2=0,*cn;
			for(int Lindex=-1;;){
				Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
				if(Lindex==-1) break;
				cnt++;
			}
			//�]���f�[�^�����炩���ߍ���Ă���
			p = (playlistdata*)malloc(sizeof(playlistdata)*cnt);
			for(int Lindexx=-1;;cnt2++){
				Lindexx=m_lc.GetNextItem(Lindexx,LVNI_ALL |LVNI_SELECTED);
				if(Lindexx==-1) break;
			}
			//�]������C���e�b�N�X�ԍ����l������
			cn =(int*)malloc(sizeof(int)*cnt2);
			for(int cn1=0,Lindexx=-1;;cn1++){
				Lindexx=m_lc.GetNextItem(Lindexx,LVNI_ALL |LVNI_SELECTED);
				if(Lindexx==-1) break;
				cn[cn1]=Lindexx;
			}
			CString s;
			for(cnt=0,Lindex=-1;;cnt++){
				Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
				if(Lindex==-1) break;
				_tcscpy(p[cnt].name,pc[Lindex].name);
				_tcscpy(p[cnt].fol,pc[Lindex].fol);
				p[cnt].sub=pc[Lindex].sub;
				p[cnt].ret2=pc[Lindex].ret2;
			}
			for(Lindex=-1;;){
				playlistdata pp;
				Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
				if(Lindex==-1) break;
				_tcscpy(pp.name,pc[hItem].name);
				_tcscpy(pp.fol,pc[hItem].fol);
				pp.sub=pc[hItem].sub;
				pp.ret2=pc[hItem].ret2;
				for(int cnt1=0;cnt1<cnt;cnt1++){
					if(_tcscmp(p[cnt1].name,pp.name)==0 && _tcscmp(p[cnt1].fol,pp.fol)==0 && p[cnt1].sub==pp.sub && p[cnt1].ret2==pp.ret2){
						break;
					}
				}
				if(cnt1!=cnt) break;
				if(hItem<Lindex){//�I�����ڂ����@�h���b�v�ʒu����
					int k=Lindex-hItem;
					m_lc.SetItemState(Lindex,m_lc.GetItemState(Lindex,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
					for(int kk=0;kk<k;kk++){
						OnXCHG(Lindex-kk,Lindex-kk-1);
					}
					m_lc.SetItemState(hItem  ,m_lc.GetItemState(hItem,LVIS_SELECTED)|LVIS_SELECTED,LVIS_SELECTED);
				}else{//�I�����ڂ���@�h���b�v�ʒu����
					for(Lindexx=-1;;){
						Lindexx=m_lc.GetNextItem(Lindexx,LVNI_ALL |LVNI_SELECTED);
						if(Lindexx==-1) break;
						m_lc.SetItemState(Lindexx,m_lc.GetItemState(Lindexx,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
					}
					for(int i=0;i<cn1;i++){
						int k=hItem-cn[i];
						for(int kk=0;kk<k;kk++){
								OnXCHG(cn[i]+kk+1,cn[i]+kk);
						}
						for(int j=0;j<cn1;j++) cn[j]--;
					}
					hItem-=cn1;
					for(i=0;i<cn1;i++){
						hItem++;
						m_lc.SetItemState(hItem  ,m_lc.GetItemState(hItem,LVIS_SELECTED)|LVIS_SELECTED,LVIS_SELECTED);
					}
					break;
				}
				hItem++;
			}
			free(cn);
			free(p);
			m_lc.RedrawWindow();
			m_lDragTopItem=0;m_lDragTopItemt=0;
		 }
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CPlayList::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *lpDInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if(lpDInfo==NULL)return;
	try{
	_set_se_translator( trans_func1 );
	if( lpDInfo->item.mask & LVIF_TEXT ){
		 switch( lpDInfo->item.iSubItem ){
			 case 0:
				 _tcscpy( lpDInfo->item.pszText, pc[lpDInfo->item.iItem].name ) ;	 break ;
			 case 1:
				 _tcscpy( lpDInfo->item.pszText, pc[lpDInfo->item.iItem].game ) ;	 break ;
			 case 2:{
				 CString s;
				 if(pc[lpDInfo->item.iItem].time>=3600)
					 s.Format(_T("%d:%02d:%02d"),pc[lpDInfo->item.iItem].time/3600,(pc[lpDInfo->item.iItem].time/60)%60,pc[lpDInfo->item.iItem].time%60);
				 else
					 s.Format(_T("%d:%02d"),pc[lpDInfo->item.iItem].time/60,pc[lpDInfo->item.iItem].time%60);
				 if(pc[lpDInfo->item.iItem].time==0)s="";
				 if(pc[lpDInfo->item.iItem].time==-1)s="�擾�s�\";
				 _tcscpy( lpDInfo->item.pszText,s);
				 }break;
			 case 3:
				 _tcscpy( lpDInfo->item.pszText, pc[lpDInfo->item.iItem].art ) ;	 break ;
			 case 4:
				 _tcscpy( lpDInfo->item.pszText, pc[lpDInfo->item.iItem].alb ) ;	 break ;
			 case 5:
				 _tcscpy( lpDInfo->item.pszText, pc[lpDInfo->item.iItem].fol ) ;	 break ;
			default :
				break ;
		  }
	}
	if( lpDInfo->item.mask & LVIF_IMAGE ){
		lpDInfo->item.iImage=pc[lpDInfo->item.iItem].icon;
	}
	}
	catch(SE_Exception1 e){
	}
	catch(_EXCEPTION_POINTERS *ep){
	}
	catch(...){}
	*pResult = 0;
}

void CPlayList::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;

	CPoint point;
	CRect rect;
	GetCursorPos(&point);

	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_LIST));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);

}

void CPlayList::OnList()
{
	int Lindex=-1;
	Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	CListSyosai a(this);
	w_flg=FALSE;
	memcpy(&a.pc,&pc[Lindex],sizeof(playlistdata0));
	a.DoModal();
	w_flg=TRUE;
}

void CPlayList::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if(plw){
		if((nState==WA_ACTIVE || nState==WA_CLICKACTIVE) && bMinimized==0  && m_saisyo.GetCheck()){
			og->ShowWindow(SW_RESTORE);
			og->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
			SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

		}
	}
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CPlayList::OnPop32787()//�t�@�C�����ύX
{
	// TODO: �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	int Lindex=-1;
	Lindex=m_lc.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	CFilename a(this);
	w_flg=FALSE;
	memcpy(&a.pc,&pc[Lindex],sizeof(playlistdata0));
	int ret=a.DoModal();
	if(ret==IDOK){
		_tcscpy(pc[Lindex].name,a.pc.name);
		_tcscpy(pc[Lindex].art,a.pc.art);
		_tcscpy(pc[Lindex].alb,a.pc.alb);
		_tcscpy(pc[Lindex].fol,a.pc.fol);
		RECT r;
		m_lc.GetItemRect(Lindex,&r,LVIR_BOUNDS);
		m_lc.RedrawWindow(&r);
	}
	w_flg=TRUE;
}

void CPlayList::OnFindUp()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CString s;
	m_find.GetWindowText(s);
	if(s=="") return;
	int pnt2;

	if(pnt<0) pnt=-1;
	if(pnt>=playcnt) pnt=playcnt;

	pnt2=pnt;
	if(pnt1!=-1) pnt2=pnt1;


	int flg=0;
	for(int i=pnt2;i<playcnt;i++){
		CString ss;
		ss=pc[i].name;
		if(ss.Find(s)!=-1 && pnt2!=i) {flg=1;break;}
		ss=pc[i].alb;
		if(ss.Find(s)!=-1 && pnt2!=i) {flg=1;break;}
		ss=pc[i].art;
		if(ss.Find(s)!=-1 && pnt2!=i) {flg=1;break;}
	}

	if(flg){
		for(int k=0;k<playcnt;k++){
			m_lc.SetItemState(k,m_lc.GetItemState(k,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
		}

		pnt1=i;

		m_lc.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_lc.EnsureVisible(i,FALSE);
	}
	m_lc.SetFocus();
}

void CPlayList::OnFindDown()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CString s;
	m_find.GetWindowText(s);
	if(s=="") return;
	int pnt2;

	if(pnt<0) pnt=-1;
	if(pnt>=playcnt) pnt=playcnt;

	pnt2=pnt;
	if(pnt1!=-1) pnt2=pnt1;


	int flg=0;
	for(int i=pnt2;i>=0;i--){
		CString ss;
		ss=pc[i].name;
		if(ss.Find(s)!=-1 && pnt2!=i) {flg=1;break;}
		ss=pc[i].alb;
		if(ss.Find(s)!=-1 && pnt2!=i) {flg=1;break;}
		ss=pc[i].art;
		if(ss.Find(s)!=-1 && pnt2!=i) {flg=1;break;}
	}

	if(flg){
		for(int k=0;k<playcnt;k++){
			m_lc.SetItemState(k,m_lc.GetItemState(k,LVIS_SELECTED)&~LVIS_SELECTED,LVIS_SELECTED);
		}
		pnt1=i;

		m_lc.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_lc.EnsureVisible(i,FALSE);
	}
	m_lc.SetFocus();
}
