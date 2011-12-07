// Kpilist.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ogg.h"
#include "Kpilist.h"


// CKpilist �_�C�A���O

IMPLEMENT_DYNAMIC(CKpilist, CDialog)

CKpilist::CKpilist(CWnd* pParent /*=NULL*/)
	: CDialog(CKpilist::IDD, pParent)
{

}

CKpilist::~CKpilist()
{
}

void CKpilist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lc);
}


BEGIN_MESSAGE_MAP(CKpilist, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CKpilist::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CKpilist ���b�Z�[�W �n���h��
extern CString ext[10000][100];
extern int kpicnt;
extern CString kpif[10000];

BOOL CKpilist::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDOK),_T("���܂�"));
	m_tooltip.SetDelayTime( TTDT_AUTOPOP, 10000 );
	m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 512);


	DWORD dwExStyle = m_lc.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP;
	m_lc.SetExtendedStyle(dwExStyle);
	m_lc.ModifyStyle ( 0, LVS_REPORT );
	m_lc.InsertColumn ( 0, _T("kpi"), LVCFMT_LEFT, 100, 0 );
	m_lc.InsertColumn ( 1, _T("�g���q"), LVCFMT_LEFT, 400, 0 );

	TCHAR buf[1024];
	LV_ITEM LvItem;
	int      idItem;
	m_lc.DeleteAllItems();
	int Lindex=-1;
	for(int j=0;j<kpicnt;j++){//�I������Ă�����̂��s�b�N�A�b�v
		CString s;s="";
		for(int i=0;;i++){
			if(ext[j][i]=="") break;
			s+=ext[j][i];s+="/";
		}
		s=s.Left(s.GetLength()-1);
		_tcscpy(buf,kpif[j].Right(kpif[j].GetLength()-kpif[j].ReverseFind('\\')-1));	LvItem.pszText = buf;
		LvItem.iItem = m_lc.GetItemCount( );
		LvItem.mask = LVIF_TEXT | LVIF_STATE;
		LvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
		LvItem.state = 0;
		LvItem.iSubItem = 0;
		LvItem.cchTextMax = _tcslen(LvItem.pszText);
		idItem = m_lc.InsertItem(&LvItem);
		// InsertItem() �ɂ���� item ID (�s�ԍ�) ���Ԃ����
		LvItem.iItem = idItem;
		_tcscpy(buf,s);	LvItem.iSubItem = 1;
		LvItem.pszText = buf;
		m_lc.SetItem(&LvItem);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CKpilist::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	*pResult = 0;
}

BOOL CKpilist::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
		m_tooltip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}