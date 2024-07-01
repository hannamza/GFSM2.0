#pragma once
#include "afxwin.h"


// CManagerModDlg 대화 상자입니다.

class CManagerModDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CManagerModDlg)

public:
	CManagerModDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManagerModDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODMANAGER };
#endif

public:
	void SetManagerValue(CString sID, CString sPW, CString sEtc, int nLimit, int nFacpType);
	void GetManagerValue(CString & sID, CString & sPW, CString & sEtc, int & nLimit, int & nFacpType);

public:
	CString m_sID, m_sPW, m_sEtc;
	int m_nLimit, m_nFacpType;
	CBrush m_brush;

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
	CEdit m_editWorksite;
	CEdit m_editLoc;
	CButton m_checkPw;
	CComboBox m_ctrlCBFacpType;
};
