// Render.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "ogg.h"
#include "Render.h"
#include "Graph.h"
extern IGraphBuilder *pGraphBuilder;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern save savedata;
/////////////////////////////////////////////////////////////////////////////
// CRender �_�C�A���O


CRender::CRender(CWnd* pParent /*=NULL*/)
	: CDialog(CRender::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRender)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CRender::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRender)
	DDX_Control(pDX, IDC_COMBO1, m_1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK1, m_evr);
	DDX_Control(pDX, IDC_CHECK2, m_con);
	DDX_Control(pDX, IDC_CHECK3, m_a);
	DDX_Control(pDX, IDC_CHECK27, m_ffd);
	DDX_Control(pDX, IDCANCEL2, m_l);
	DDX_Control(pDX, IDC_CHECK30, m_vob);
	DDX_Control(pDX, IDC_CHECK31, m_haali);
	DDX_Control(pDX, IDC_CHECK32, m_spc2x);
	DDX_Control(pDX, IDC_CHECK33, m_spc4x);
	DDX_Control(pDX, IDC_CHECK34, m_spc8x);
	DDX_Control(pDX, IDC_CHECK35, m_spc1x);
	DDX_Control(pDX, IDC_CHECK36, m_spc16x);
	DDX_Control(pDX, IDC_CHECK40, m_mp31);
	DDX_Control(pDX, IDC_CHECK37, m_mp315);
	DDX_Control(pDX, IDC_CHECK38, m_mp32);
	DDX_Control(pDX, IDC_CHECK39, m_mp325);
	DDX_Control(pDX, IDC_CHECK41, m_mp33);
	DDX_Control(pDX, IDC_CHECK45, m_kpi10);
	DDX_Control(pDX, IDC_CHECK42, m_kpi15);
	DDX_Control(pDX, IDC_CHECK43, m_kpi20);
	DDX_Control(pDX, IDC_CHECK44, m_kpi25);
	DDX_Control(pDX, IDC_CHECK46, m_kpi30);
	DDX_Control(pDX, IDCANCEL3, m_kpi);
	DDX_Control(pDX, IDC_CHECK47, m_mp3orig);
}


BEGIN_MESSAGE_MAP(CRender, CDialog)
	//{{AFX_MSG_MAP(CRender)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL2, &CRender::OnBnClickedCancel2)
	ON_BN_CLICKED(IDC_CHECK32, &CRender::Onspc2x)
	ON_BN_CLICKED(IDC_CHECK33, &CRender::Onspc4x)
	ON_BN_CLICKED(IDC_CHECK34, &CRender::Onspc8x)
	ON_BN_CLICKED(IDC_CHECK35, &CRender::Onspc1x)
	ON_BN_CLICKED(IDC_CHECK36, &CRender::Onspc16x)
	ON_BN_CLICKED(IDC_CHECK40, &CRender::Onmp31)
	ON_BN_CLICKED(IDC_CHECK37, &CRender::Onmp315)
	ON_BN_CLICKED(IDC_CHECK38, &CRender::Onmp32)
	ON_BN_CLICKED(IDC_CHECK39, &CRender::Onmp325)
	ON_BN_CLICKED(IDC_CHECK41, &CRender::Onmp33)
	ON_BN_CLICKED(IDC_CHECK45, &CRender::Onkpi10)
	ON_BN_CLICKED(IDC_CHECK42, &CRender::Onkpi15)
	ON_BN_CLICKED(IDC_CHECK43, &CRender::Onkpi20)
	ON_BN_CLICKED(IDC_CHECK44, &CRender::Onkpi25)
	ON_BN_CLICKED(IDC_CHECK46, &CRender::Onkpi30)
	ON_BN_CLICKED(IDCANCEL3, &CRender::Onkpi)
	ON_BN_CLICKED(IDC_FONT, &CRender::OnFontMain)
	ON_BN_CLICKED(IDC_FONT2, &CRender::OnFontList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRender ���b�Z�[�W �n���h��

BOOL CRender::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	OSVERSIONINFO in;ZeroMemory(&in,sizeof(in));in.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);GetVersionEx(&in);
	if(in.dwMajorVersion<=5)
		m_1.AddString(_T("�f�t�H���g"));
	else	
		m_1.AddString(_T("�f�t�H���g(����/EVR)"));
	m_1.AddString(_T("VMR7"));
	m_1.AddString(_T("VMR9"));
	m_1.SetCurSel(savedata.render);
	m_evr.SetCheck(savedata.evr);
	m_con.SetCheck(savedata.con);
	m_a.SetCheck(savedata.aero);
	m_ffd.SetCheck(savedata.ffd);
	m_vob.SetCheck(savedata.vob);
	m_haali.SetCheck(savedata.haali);
	switch(savedata.spc){
		case 1:m_spc1x.SetCheck(TRUE); break;
		case 2:m_spc2x.SetCheck(TRUE); break;
		case 4:m_spc4x.SetCheck(TRUE); break;
		case 8:m_spc8x.SetCheck(TRUE); break;
		case 16:m_spc16x.SetCheck(TRUE); break;
	}
	switch(savedata.mp3){
		case 1:m_mp31.SetCheck(TRUE); break;
		case 2:m_mp315.SetCheck(TRUE); break;
		case 3:m_mp32.SetCheck(TRUE); break;
		case 4:m_mp325.SetCheck(TRUE); break;
		case 5:m_mp33.SetCheck(TRUE); break;
	}
	switch(savedata.kpivol){
		case 1:m_kpi10.SetCheck(TRUE); break;
		case 2:m_kpi15.SetCheck(TRUE); break;
		case 3:m_kpi20.SetCheck(TRUE); break;
		case 4:m_kpi25.SetCheck(TRUE); break;
		case 5:m_kpi30.SetCheck(TRUE); break;
	}
	if(in.dwMajorVersion<=5){
		m_evr.EnableWindow(FALSE);
		m_con.EnableWindow(FALSE);
		m_a.EnableWindow(FALSE);
	}
	m_mp3orig.SetCheck(savedata.mp3orig);

	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDOK),_T("�ۑ����ĕ��܂�"));
	m_tooltip.AddTool(GetDlgItem(IDCANCEL),_T("�ۑ������ɕ��܂�"));
	m_tooltip.AddTool(GetDlgItem(IDCANCEL2),_T("�Đ����̎g�pDirectShow�t�B���^��\�����܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDCANCEL3),_T("kpi�ꗗ��\�����܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK1), _T("Windows Vista/7�ȍ~�ŗL���ł��B\nIndeo��p��������̏ꍇOFF�ɂ��Ă��������B\n����ȊO��ON�ł����ł��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK2), _T("Windows Vista/7�ȍ~�ŗL���ł��B\n�f�X�N�g�b�v�R���|�W�V����(Aero)���g�p���邩�ǂ�����I�����܂��B\n�g�p���Ȃ��ɂ����EVR����Ȃ��Ă������ʂ͂��ꂢ�ɂȂ�܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK3), _T("Windows Vista/7�ȍ~�ŗL���ł��B\nAero Grass���g�p���邩�ǂ������߂܂��B\n�����_�ł͎g���ƕ����܂œ��߂����Ⴄ�̂Ō��Â炢�ł��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK27), _T("�����ffdshow���g�����ǂ����I�����܂��B\nWindows7�̏ꍇ�A�f�t�H���g��Divx�Ȃǂ��Đ��ł���̂ł�������g�������l��OFF�ɂ��Ă��������B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK30),_T("vob��dat�t�@�C����Haali��ʂ��Ȃ��悤�ɍ���Ă��܂����A\nvob�ɕ�������������Ƃ��ɂ̓`�F�b�N�����ĉ������B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK31),_T("�����Haali���g���܂���B\n���悪�d���Ǝv�������╡���������������̓`�F�b�N������ƌy���Ȃ�܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK32),_T("kpi SPC/NEZplug++�̉��ʂ�1.5�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK33),_T("kpi SPC/NEZplug++�̉��ʂ�2�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK34),_T("kpi SPC/NEZplug++�̉��ʂ�2.5�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK35),_T("kpi SPC/NEZplug++�̉��ʂ𓙔{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK36),_T("kpi SPC/NEZplug++�̉��ʂ�3�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK40),_T("mp3�̉��ʂ𓙔{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK37),_T("mp3�̉��ʂ�1.5�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK38),_T("mp3�̉��ʂ�2�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK39),_T("mp3�̉��ʂ�2.5�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK41),_T("mp3�̉��ʂ�3�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK45),_T("kpi�̉��ʂ𓙔{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK42),_T("kpi�̉��ʂ�1.5�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK43),_T("kpi�̉��ʂ�2�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK44),_T("kpi�̉��ʂ�2.5�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK46),_T("kpi�̉��ʂ�3�{�ɂ��܂��B"));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK47),_T("mp3�̃f�R�[�_���I���W�i���̃f�R�[�_���g�킸�ɁA�Ǝ��Ŏg�����f�R�[�_���g���B\n�G���[�Ȃǂŉ��t�ł��Ȃ��Ƃ��Ƀ`�F�b�N����ĉ������B\n�܂��Ǝ��Ő���ɂȂ�Ȃ����͂͂����ĉ������B"));
	m_tooltip.SetDelayTime( TTDT_AUTOPOP, 10000 );
	m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 512);

#if WIN64
	m_kpi.EnableWindow(FALSE);
#else
#endif

	if(!pGraphBuilder)
		m_l.EnableWindow(FALSE);
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CRender::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	savedata.render=m_1.GetCurSel();
	savedata.evr=m_evr.GetCheck();
	savedata.con=m_con.GetCheck();
	savedata.aero=m_a.GetCheck();
	savedata.ffd=m_ffd.GetCheck();
	savedata.vob=m_vob.GetCheck();
	savedata.haali=m_haali.GetCheck();
//	savedata.mp3orig=m_mp3orig.GetCheck();
	CDialog::OnOK();
}

INT_PTR CRender::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	return CDialog::OnToolHitTest(point, pTI);
}

BOOL CRender::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
		m_tooltip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CRender::OnBnClickedCancel2()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CGraph a;
	if(pGraphBuilder)
		a.DoModal();
}

void CRender::Onspc2x()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_spc2x.SetCheck(TRUE);
	m_spc4x.SetCheck(FALSE);
	m_spc8x.SetCheck(FALSE);
	m_spc16x.SetCheck(FALSE);
	m_spc1x.SetCheck(FALSE);
	savedata.spc=2;
}

void CRender::Onspc4x()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_spc2x.SetCheck(FALSE);
	m_spc4x.SetCheck(TRUE);
	m_spc8x.SetCheck(FALSE);
	m_spc16x.SetCheck(FALSE);
	m_spc1x.SetCheck(FALSE);
	savedata.spc=4;
}

void CRender::Onspc8x()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_spc2x.SetCheck(FALSE);
	m_spc4x.SetCheck(FALSE);
	m_spc8x.SetCheck(TRUE);
	m_spc16x.SetCheck(FALSE);
	m_spc1x.SetCheck(FALSE);
	savedata.spc=8;
}

void CRender::Onspc1x()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_spc2x.SetCheck(FALSE);
	m_spc4x.SetCheck(FALSE);
	m_spc8x.SetCheck(FALSE);
	m_spc16x.SetCheck(FALSE);
	m_spc1x.SetCheck(TRUE);
	savedata.spc=1;
}

void CRender::Onspc16x()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_spc2x.SetCheck(FALSE);
	m_spc4x.SetCheck(FALSE);
	m_spc8x.SetCheck(FALSE);
	m_spc16x.SetCheck(TRUE);
	m_spc1x.SetCheck(FALSE);
	savedata.spc=16;
}

void CRender::Onmp31()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_mp31.SetCheck(TRUE);
	m_mp315.SetCheck(FALSE);
	m_mp32.SetCheck(FALSE);
	m_mp325.SetCheck(FALSE);
	m_mp33.SetCheck(FALSE);
	savedata.mp3=1;
}

void CRender::Onmp315()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_mp31.SetCheck(FALSE);
	m_mp315.SetCheck(TRUE);
	m_mp32.SetCheck(FALSE);
	m_mp325.SetCheck(FALSE);
	m_mp33.SetCheck(FALSE);
	savedata.mp3=2;
}

void CRender::Onmp32()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_mp31.SetCheck(FALSE);
	m_mp315.SetCheck(FALSE);
	m_mp32.SetCheck(TRUE);
	m_mp325.SetCheck(FALSE);
	m_mp33.SetCheck(FALSE);
	savedata.mp3=3;
}

void CRender::Onmp325()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_mp31.SetCheck(FALSE);
	m_mp315.SetCheck(FALSE);
	m_mp32.SetCheck(FALSE);
	m_mp325.SetCheck(TRUE);
	m_mp33.SetCheck(FALSE);
	savedata.mp3=4;
}

void CRender::Onmp33()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_mp31.SetCheck(FALSE);
	m_mp315.SetCheck(FALSE);
	m_mp32.SetCheck(FALSE);
	m_mp325.SetCheck(FALSE);
	m_mp33.SetCheck(TRUE);
	savedata.mp3=5;
}

void CRender::Onkpi10()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_kpi10.SetCheck(TRUE);
	m_kpi15.SetCheck(FALSE);
	m_kpi20.SetCheck(FALSE);
	m_kpi25.SetCheck(FALSE);
	m_kpi30.SetCheck(FALSE);
	savedata.kpivol=1;
}

void CRender::Onkpi15()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_kpi10.SetCheck(FALSE);
	m_kpi15.SetCheck(TRUE);
	m_kpi20.SetCheck(FALSE);
	m_kpi25.SetCheck(FALSE);
	m_kpi30.SetCheck(FALSE);
	savedata.kpivol=2;
}

void CRender::Onkpi20()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_kpi10.SetCheck(FALSE);
	m_kpi15.SetCheck(FALSE);
	m_kpi20.SetCheck(TRUE);
	m_kpi25.SetCheck(FALSE);
	m_kpi30.SetCheck(FALSE);
	savedata.kpivol=3;
}

void CRender::Onkpi25()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_kpi10.SetCheck(FALSE);
	m_kpi15.SetCheck(FALSE);
	m_kpi20.SetCheck(FALSE);
	m_kpi25.SetCheck(TRUE);
	m_kpi30.SetCheck(FALSE);
	savedata.kpivol=4;
}

void CRender::Onkpi30()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_kpi10.SetCheck(FALSE);
	m_kpi15.SetCheck(FALSE);
	m_kpi20.SetCheck(FALSE);
	m_kpi25.SetCheck(FALSE);
	m_kpi30.SetCheck(TRUE);
	savedata.kpivol=5;
}

#include "Kpilist.h"
void CRender::Onkpi()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CKpilist k;
	k.DoModal();
}

extern HFONT	hFont;
#include "afxdlgs.h"
void CRender::OnFontMain()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	LOGFONT      logFont;
	CFont* f=CFont::FromHandle(hFont);
	f->GetLogFont(&logFont);
	CFontDialog fontDlg(&logFont);
	if (fontDlg.DoModal() == IDOK){
		DeleteObject(hFont);
		hFont=CreateFontIndirect(fontDlg.m_cf.lpLogFont);
	}

}
#include "PlayList.h"
extern CPlayList *pl;
void CRender::OnFontList()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	LOGFONT      logFont;
	CFont* f=pl->m_lc.GetFont();
	f->GetLogFont(&logFont);
	CFontDialog fontDlg(&logFont,CF_SCREENFONTS);
	if (fontDlg.DoModal() == IDOK && pl){
		pl->m_lc.SetFont(fontDlg.GetFont());
	}
}
