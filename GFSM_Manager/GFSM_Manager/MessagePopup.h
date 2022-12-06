#pragma once
#include "afxwin.h"


// CMessagePopup ��ȭ �����Դϴ�.

class CMessagePopup : public CSkinDialog
{
	DECLARE_DYNAMIC(CMessagePopup)

public:
	CMessagePopup(CString sTitle, CString sCaption, int nType, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMessagePopup();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MESSAGE };
#endif

public:
	CString m_sID, m_sPW;
	CBrush m_brush;

	CString m_sTitle, m_sCaption;
	int m_nType;

public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonConfirm();
	afx_msg void OnBnClickedButtonCancel();
	CSkinButton m_btnCancel;
	CSkinButton m_btnOk;
	CStatic m_staticText;
};
