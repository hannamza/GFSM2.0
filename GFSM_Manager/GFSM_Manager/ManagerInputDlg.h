#pragma once
#include "afxwin.h"


// CManagerInputDlg 대화 상자입니다.

class CManagerInputDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CManagerInputDlg)

public:
	CManagerInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManagerInputDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MANAGERINPUT };
#endif

public:
	CString m_sID, m_sPW, m_sEtc;
	int m_nLimit;
	int m_nFacpType;
	CBrush m_brush;

public:
	void GetInputValue(CString & sID, CString & sPW, CString & sEtc, int & nLimit, int & nFacpType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	CEdit m_editEtc;
	CComboBox m_ctrlCBFacpType;
};
