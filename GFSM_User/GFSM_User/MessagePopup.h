#pragma once


// CMessagePopup 대화 상자입니다.

class CMessagePopup : public CDialogEx
{
	DECLARE_DYNAMIC(CMessagePopup)

public:
	CMessagePopup(CString sTitle, CString sCaption, int nType, CWnd* pParent = NULL);   // 표준 생성자입니다.
	CMessagePopup(CString sTitle, CString sCaption1, CString sCaption2, int nType, UINT nStyle, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMessagePopup();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MESSAGE };
#endif

private:
	CString m_sTitle, m_sCaption;
	int m_nType;
	CFont m_font, m_font2;
	CString m_sCaption1, m_sCaption2;
	UINT m_nStyle;

protected:
	void Redisplay();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOkClick();
	afx_msg void OnCancelClick();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CImageBtn m_btnOk;
	CImageBtn m_btnCancel;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
