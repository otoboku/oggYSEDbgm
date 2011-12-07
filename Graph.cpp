// Graph.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ogg.h"
#include "Graph.h"

extern IGraphBuilder *pGraphBuilder;
// CGraph ダイアログ

IMPLEMENT_DYNAMIC(CGraph, CDialog)

CGraph::CGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CGraph::IDD, pParent)
{

}

CGraph::~CGraph()
{
}

void CGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_LIST1, m_l);
}


BEGIN_MESSAGE_MAP(CGraph, CDialog)
END_MESSAGE_MAP()


// CGraph メッセージ ハンドラ

BOOL CGraph::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(pGraphBuilder){
//		IFilterGraph *fg;
//		pGraphBuilder->QueryInterface(IID_IFilterGraph,(void**)&fg);

		EnumFilters(pGraphBuilder);
//		fg->Release();
	}else EndDialog(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

HRESULT CGraph::EnumFilters (IGraphBuilder *pGraph) 
{
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter;
    ULONG cFetched;

    HRESULT hr = pGraph->EnumFilters(&pEnum);
    if (FAILED(hr)) return hr;

    while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
    {
		IEnumPins *p;
		IPin *pPin;
		CString s,ss,sss;s="",ss="",sss="";
        FILTER_INFO FilterInfo,FilterInfo1;
        pFilter->QueryFilterInfo(&FilterInfo);
		pFilter->EnumPins(&p);
		s=" -> ";
        char szName[MAX_FILTER_NAME];
        char szName1[MAX_FILTER_NAME];
        int cch = WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName,
            -1, szName, MAX_FILTER_NAME, 0, 0);
		if (cch > 0){
			ss=szName;
		}
		while(p->Next(1, &pPin, 0) == S_OK)
		{
			PIN_DIRECTION PinDirThis;
			pPin->QueryDirection(&PinDirThis);
			s=" -> ";
			if (PinDirThis == PINDIR_OUTPUT){
				PIN_INFO pp;
				IPin *pn;
				if(pPin->ConnectedTo(&pn)==S_OK){
					pn->QueryPinInfo(&pp);
					pp.pFilter->QueryFilterInfo(&FilterInfo1);
					WideCharToMultiByte(CP_ACP, 0, FilterInfo1.achName,
						-1, szName1, MAX_FILTER_NAME, 0, 0);
					s+=szName1;sss=ss+s;
					if(!(szName[1]==':'||sss.Find('.')!=-1)){
						m_l.AddString(sss);
					}else{
						sss=_T("Source")+s;
						m_l.AddString(sss);
					}	
					s=" -> ";pn->Release();
				}else{
					//s+="UnKnown";sss=ss+s;
					//if(!(szName[1]==':'||sss.Find('.')!=-1)){
					//	m_l.AddString(sss);
				//	}
					s=" -> ";
				}
			}
		}
		p->Release();
        // FILTER_INFO 構造体はフィルタ グラフ マネージャへのポインタを保持する。
        // その参照カウントは解放しなければならない。
        if (FilterInfo.pGraph != NULL)
            FilterInfo.pGraph->Release();
        pFilter->Release();
    }

    pEnum->Release();
    return S_OK;
}
