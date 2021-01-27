
// MFCserialDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "Mycomm.h"


// CMFCserialDlg ��ȭ ����
class CMFCserialDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFCserialDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	CMycomm* m_comm;
	LRESULT OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT OnReceive(WPARAM length, LPARAM lpara);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSERIAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL comport_state;
	CComboBox m_combo_comport_list;
	CString m_str_comport;
	CComboBox m_combo_baudrate_list;
	CString m_str_baudrate;
	CEdit m_edit_rcv_view;
	CEdit m_edit_send_data;
	// afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnCbnSelchangeComboComport();
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnBnClickedBtClaer();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnEnChangeEditRcvView();
};
