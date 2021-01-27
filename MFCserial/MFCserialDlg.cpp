
// MFCserialDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFCserial.h"
#include "MFCserialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif //_DEBUG
CString str;
#define TR(s){CString str;str.Format(_T("%d"), s);OutputDebugString(str+_T("\n"));}


// CMFCserialDlg ��ȭ ����



CMFCserialDlg::CMFCserialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCSERIAL_DIALOG, pParent)
	, m_str_comport(_T(""))
	, m_str_baudrate(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCserialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_combo_comport_list);
	DDX_CBString(pDX, IDC_COMBO_COMPORT, m_str_comport);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate_list);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_str_baudrate);
	DDX_Control(pDX, IDC_EDIT_RCV_VIEW, m_edit_rcv_view);
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_edit_send_data);


}

BEGIN_MESSAGE_MAP(CMFCserialDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_MYRECEIVE, &CMFCserialDlg::OnReceive)
	ON_MESSAGE(WM_MYCLOSE, &CMFCserialDlg::OnThreadClosed)
	

	ON_BN_CLICKED(IDC_BT_CONNECT, &CMFCserialDlg::OnBnClickedBtConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CMFCserialDlg::OnCbnSelchangeComboComport)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CMFCserialDlg::OnCbnSelchangeComboBaudrate)
	ON_BN_CLICKED(IDC_BT_CLAER, &CMFCserialDlg::OnBnClickedBtClaer)
	ON_BN_CLICKED(IDC_BT_SEND, &CMFCserialDlg::OnBnClickedBtSend)
	
END_MESSAGE_MAP()


// CMFCserialDlg �޽��� ó����

BOOL CMFCserialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_combo_comport_list.AddString(_T("COM1"));
	m_combo_comport_list.AddString(_T("COM2"));
	m_combo_comport_list.AddString(_T("COM3"));
	m_combo_comport_list.AddString(_T("COM4"));
	m_combo_comport_list.AddString(_T("COM5"));

	m_combo_baudrate_list.AddString(_T("9600"));
	m_combo_baudrate_list.AddString(_T("19200"));
	m_combo_baudrate_list.AddString(_T("115200"));

	comport_state = false;
	GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
	m_str_comport = _T("COM3");
	m_str_baudrate = _T("9600");
	UpdateData(FALSE);
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFCserialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMFCserialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CMFCserialDlg::OnThreadClosed(WPARAM length, LPARAM lpara){
	((CMycomm*)lpara)->HandleClose();
	delete ((CMycomm*)lpara);

	return 0;
}

LRESULT CMFCserialDlg::OnReceive(WPARAM length, LPARAM lpara){
	
	CString strDR, strDS, strCG, strD;
	int lengths;
	char data[4096];
	
	if (m_comm){
		m_comm->Receive(data, length);
		data[length] = _T('\0');
		int i = 0;
		while (data[i] != _T('\0')){
			str += data[i];
			i++;
			lengths = str.GetLength();
			if (lengths == 33){
				str += _T("\r\n");
			
				m_edit_rcv_view.ReplaceSel(str);
				AfxExtractSubString(strDR, str, 0, '&');
				AfxExtractSubString(strDS, str, 1, '&');
				AfxExtractSubString(strCG, str, 2, '&');
				str = "";

				strDR = strDR.Mid(3, 7);
				strDS = strDS.Mid(3, 7);
				strCG = strCG.Mid(3, 7);

				double nNumA = _wtof(strDR);
				double nNumB = _wtof(strDS);
				double nNumC = _wtof(strCG);

				strDR.Format(_T("DR : %.1f "), nNumA);
				strDS.Format(_T("DS : %.1f "), nNumB);
				strCG.Format(_T("CG : %.1f "), nNumC);

				strD = strDR+strDS+strCG + _T("\r\n");

				m_edit_rcv_view.ReplaceSel(strD);
				m_edit_rcv_view.LineScroll(m_edit_rcv_view.GetLineCount());

				strDR, strDS, strCG, strD = "";
			}
		}
	}
	return 0;
}

// void CMFCserialDlg::OnCbnSelchangeComboBaudrate()
//{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void CMFCserialDlg::OnBnClickedBtConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (comport_state){
		if (m_comm){
			m_comm->Close();
			m_comm = NULL;
			AfxMessageBox(_T("COM PORT CLOSE"));
			comport_state = false;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else{
		m_comm = new CMycomm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));
		if (m_comm->Create(GetSafeHwnd()) != 0){
			AfxMessageBox(_T("COMPORT OPEN"));
			comport_state = true;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("CLOSE"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else{
			AfxMessageBox(_T("Error!"));
		}
	}
	
}


void CMFCserialDlg::OnCbnSelchangeComboComport()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
}


void CMFCserialDlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
}


void CMFCserialDlg::OnBnClickedBtClaer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_EDIT_RCV_VIEW)->SetWindowText(_T(" "));
}


void CMFCserialDlg::OnBnClickedBtSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	GetDlgItem(IDC_EDIT_SEND_DATA)->SetWindowText(str);
	str += "\r\n";
	m_comm->Send(str, str.GetLength());
}

