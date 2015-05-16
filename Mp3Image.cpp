// Mp3Image.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ogg.h"
#include "Mp3Image.h"

extern BOOL miw;
extern CMp3Image *mi;
extern int killw1;
void CMp3Image::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	killw1=1;
}
// CMp3Image ダイアログ

IMPLEMENT_DYNAMIC(CMp3Image, CDialog)

CMp3Image::CMp3Image(CWnd* pParent /*=NULL*/)
	: CDialog(CMp3Image::IDD, pParent)
{
m_hIcon = AfxGetApp()->LoadIcon(IDI_PL);
}

CMp3Image::~CMp3Image()
{
}

void CMp3Image::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_x);
	DDX_Control(pDX, IDC_STATIC2, m_y);
	DDX_Control(pDX, IDOK, m_close);
}


BEGIN_MESSAGE_MAP(CMp3Image, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CMp3Image::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CMp3Image メッセージ ハンドラ

BOOL CMp3Image::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	nnn=1;
	RECT r;
	GetClientRect(&r);
	rcm.top=0;rcm.left=0;rcm.right=r.right;rcm.bottom=r.bottom;
	m_close.MoveWindow(r.right-50,50,50,50);
	m_x.MoveWindow(r.right-50,110,50,50);
	m_y.MoveWindow(r.right-50,140,50,50);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

extern TCHAR karento2[1024];
void CMp3Image::Load(CString s)
{
	CString s1,s2;
	s1=karento2;
	s2=s1;



	CFile ff;
	if(ff.Open(s,CFile::modeRead|CFile::shareDenyNone,NULL)==FALSE){
		DestroyWindow();
		return;
	}
	BYTE buf[2005];
	ZeroMemory(buf,2005);
	ff.Read(buf,2000);
	int i,enc;
	if(buf[0x14]==0 || buf[0x14]==3) enc=0; else enc=1;
	for(i=0;i<2000;i++){
		if(buf[i]==0x41 && buf[i+1]==0x50 && buf[i+2]==0x49 && buf[i+3]==0x43){
			break;
		}
	}
	if(i==2000){
		DestroyWindow();
		return;
	}
	UINT size = (UINT)buf[i+4];
	size <<= 8;
	size |=(UINT)buf[i+5];
	size <<= 8;
	size |=(UINT)buf[i+6];
	size <<= 8;
	size |=(UINT)buf[i+7];
	i+=(4+4+3+6);
	int flg=0;
	if(buf[i]=='p'){ s1+=_T("111.png");} else {s1+=_T("111.jpg");}
	s2+=_T("111.bmp");
	for(;i<2000;i++){
//		if(enc==1){
			if(buf[i]==0 && buf[i+1]==0){
				if(buf[i+1]==0 && buf[i+2]==0)
					flg=1;
				break;
			}
//		}else{
//			if(buf[i]==0)
//				flg=1;
//				break;
//		}
	}
	if(i==2000){
		DestroyWindow();
		return;
	}
	i+=flg;
//	if(enc==1)
		i+=2;
	int ijk=i;
	CFile fff;



	if(fff.Open(s1,CFile::modeCreate|CFile::modeWrite,NULL)==FALSE){
		int a=0;
	}

	i = ijk;
	ff.SeekToBegin();
	ff.Seek(i,CFile::begin);
	char *b = new char[size];
	ff.Read(b,size);
	fff.Write(b,size);
	delete [] b;
	ff.Close();
	fff.Close();

//	img.S
	cdc0 = GetDC();
	img.Load(s1);
	img.Save(s2);
	//bmpsub = CBitmap::FromHandle(img);
	HBITMAP hbmp = (HBITMAP)::LoadImage(
    NULL, s2, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	dc.CreateCompatibleDC(NULL);
	bmpsub = CBitmap::FromHandle( hbmp );
	dc.SelectObject(bmpsub);
	ReleaseDC(cdc0);
	y=img.GetHeight();
	x=img.GetWidth();

	CString str;
	str.Format(_T("X: %4d"),x);
	m_x.SetWindowText(str);
	str.Format(_T("Y: %4d"),x);
	m_y.SetWindowText(str);

	Invalidate(FALSE);
//	InvalidateRect(&rect,FALSE);

}



void CMp3Image::OnPaint()
{
		CPaintDC dcc(this); // 描画用のデバイス コンテキスト
/*	if (IsIconic())
	{

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dcc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dcc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
*/		//if(plf!=0) 
		RECT r;
		GetClientRect(&r);
		SetStretchBltMode(dcc , COLORONCOLOR);
		dcc.StretchBlt(0,0,r.bottom,r.bottom,&dc,0,0,x,y,SRCCOPY);
		CDialog::OnPaint();
//	}
}

int CMp3Image::Create(CWnd *pWnd)
{
	 m_pParent = NULL;
	BOOL bret = CDialog::Create( CMp3Image::IDD, this);
    if( bret == TRUE)
        ShowWindow( SW_SHOW);
    return bret;
}

void CMp3Image::OnClose()
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	nnn=0;
	DestroyWindow();

	CDialog::OnClose();
}

BOOL CMp3Image::DestroyWindow()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	img.Destroy();
	BOOL rr=CDialog::DestroyWindow();
	mi=NULL;
	if(nnn)
		delete this;
	miw=0;
	return rr;
}
void CMp3Image::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	DestroyWindow();

}

void CMp3Image::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	Invalidate(FALSE);
}

void CMp3Image::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
	RECT r,rr;
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	 //左右比を保つ
	r.bottom=rcm.bottom;r.top=rcm.top;
	r.right=rcm.right;r.left=rcm.left;
    int     width,height;
	int x,y;
	double _x1,_y1;
    width=r.right-r.left;
    height=r.bottom-r.top;
	x=r.bottom-r.top; y=r.right-r.left;xx=(double)y; yy=(double)x;//動画の画像の大きさを獲得
	r.bottom=pRect->bottom;	r.top=pRect->top;
	r.right=pRect->right;	r.left=pRect->left;
	x1=r.bottom - r.top; y1_=r.right - r.left;xx1=(double)y1_; yy1_=(double)x1;//現在のサイズ獲得
	_x1=xx1/xx;
	_y1=yy1_/yy;
	switch(fwSide){
		case WMSZ_TOP:
		case WMSZ_BOTTOM:
			pRect->right=pRect->left+(int)(width*_y1)-(GetSystemMetrics(SM_CYSIZEFRAME)+::GetSystemMetrics(SM_CYCAPTION));
			break;
		case WMSZ_LEFT:
        case WMSZ_RIGHT:
			pRect->bottom=pRect->top+(int)(height*_x1)+(GetSystemMetrics(SM_CYSIZEFRAME)+::GetSystemMetrics(SM_CYCAPTION));
			break;
		case WMSZ_BOTTOMRIGHT:
			if(((double)width<(double)height))
				pRect->right=pRect->left+(int)(width*_y1);
			else
				pRect->bottom=pRect->top+(int)(height*_x1);
			break;
		case    WMSZ_TOPLEFT:
			if(((double)width<(double)height))
                pRect->left=pRect->right-(int)(width*_y1);
            else
                pRect->top=pRect->bottom-(int)(height*_x1);
			break;
 		case    WMSZ_TOPRIGHT:
			if(((double)width<(double)height))
				pRect->right=pRect->left+(int)(width*_y1);
            else
                pRect->top=pRect->bottom-(int)(height*_x1);
			break;
		case    WMSZ_BOTTOMLEFT:
			if(((double)width<(double)height))
                pRect->left=pRect->right-(int)(width*_y1);
            else
				pRect->bottom=pRect->top+(int)(height*_x1);
			break;
	}
	RECT rrr;
	GetClientRect(&rrr);
	m_close.MoveWindow(rrr.right-50,50,50,50);
	m_x.MoveWindow(rrr.right-50,110,50,50);
	m_y.MoveWindow(rrr.right-50,140,50,50);
	//SetWindowPos(NULL, 0,0,pRect->right, pRect->bottom,   SWP_NOMOVE|SWP_NOOWNERZORDER);
	Invalidate(FALSE);
}
