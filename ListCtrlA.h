#pragma once
#include "stdafx.h"
class CListCtrlA : public CListCtrl
{
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	int CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const;
#if WIN64
	virtual INT_PTR 
#else
	virtual int 
#endif
		OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

public:
	CListCtrlA(void);
	~CListCtrlA(void);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

};
