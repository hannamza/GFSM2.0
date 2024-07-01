#pragma once
#include "afxwin.h"


// CManagerModDlg ��ȭ �����Դϴ�.

class CManagerModDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CManagerModDlg)

public:
	CManagerModDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CManagerModDlg();

// ��ȭ ���� �������Դϴ�.
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
	CEdit m_editEtc;
	CEdit m_editWorksite;
	CEdit m_editLoc;
	CButton m_checkPw;
	CComboBox m_ctrlCBFacpType;
};
