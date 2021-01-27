
// MFCserialDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFCserial.h"
#include "MFCserialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif //_DEBUG
CString str;
#define TR(s){CString str;str.Format(_T("%d"), s);OutputDebugString(str+_T("\n"));}


// CMFCserialDlg 대화 상자



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


// CMFCserialDlg 메시지 처리기

BOOL CMFCserialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCserialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void CMFCserialDlg::OnBnClickedBtConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}


void CMFCserialDlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}


void CMFCserialDlg::OnBnClickedBtClaer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_RCV_VIEW)->SetWindowText(_T(" "));
}


void CMFCserialDlg::OnBnClickedBtSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	GetDlgItem(IDC_EDIT_SEND_DATA)->SetWindowText(str);
	str += "\r\n";
	m_comm->Send(str, str.GetLength());
}

