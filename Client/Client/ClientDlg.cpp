
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "iostream"
#include "ScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg �Ի���



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent), clientdata(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CClientDlg::~CClientDlg()
{
	if(clientdata)
		delete clientdata;
	clientdata = NULL;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CClientDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CClientDlg::OnBnClickedButtonRegister)
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_IPADDRESS_IP) -> SetWindowTextW(_T("192.168.1.29"));
	GetDlgItem(IDC_EDIT_PORT) -> SetWindowTextW(_T("8080"));
	GetDlgItem(IDC_EDIT_USER_NAME) -> SetWindowTextW(_T("test"));
	GetDlgItem(IDC_EDIT_PASS) -> SetWindowTextW(_T("zgh"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CClientDlg::PreTranslateMessage(MSG *pMsg)
{
	if (WM_KEYFIRST <= pMsg -> message && pMsg -> message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
	    if(pMsg -> wParam==VK_RETURN)
	    {
			OnBnClickedButtonLogin();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(clientdata)
		delete clientdata;
	clientdata = new ClientData();

	CIPAddressCtrl *IpAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP);
	BYTE f0,f1,f2,f3;
	IpAddr -> GetAddress(f0, f1, f2, f3);
	sprintf_s(clientdata -> ServerIp, "%d.%d.%d.%d",f0, f1, f2, f3);
	GetDlgItem(IDC_EDIT_PORT) -> GetWindowText(clientdata -> Port);
	GetDlgItem(IDC_EDIT_USER_NAME) -> GetWindowText(clientdata -> UserName);
	GetDlgItem(IDC_EDIT_PASS) -> GetWindowText(clientdata -> UserPass);
	
	std::cout << std::endl;
	std::cout << "login " << std::endl;
	std::cout << "IP: " << clientdata -> ServerIp << std::endl;
	std::wcout << "Port: " << (LPCTSTR)clientdata -> Port << std::endl;
	std::wcout << "User: " << (LPCTSTR)clientdata -> UserName << std::endl;
	std::wcout << "Pass: " << (LPCTSTR)clientdata -> UserPass << std::endl;
	std::cout << std::endl;

	int result = clientdata -> Info();
	switch (result)
	{
	case 1:
		{
			//MessageBoxExA(NULL, "�˿ڲ���Ϊ��", "error", 0, MB_OK);
			AfxMessageBox(_T("�˿ڲ���Ϊ��"));
			return ;
		}
	case 2:
		{
			//MessageBoxExA(NULL, "�û�������Ϊ��", "error", 0, MB_OK);
			AfxMessageBox(_T("�û�������Ϊ��"));
			return ;
		}
	case 3:
		{
			//MessageBoxExA(NULL, "���벻��Ϊ��", "error", 0, MB_OK);
			AfxMessageBox(_T("���벻��Ϊ��"));
			return ;
		}
	default:
		break;
	}
	if(!clientdata -> Login())
		return ;
	EndDialog(IDOK);
}


void CClientDlg::OnBnClickedButtonRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(clientdata)
		delete clientdata;

	clientdata = new ClientData();

	CIPAddressCtrl *IpAddr = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_IP);
	BYTE f0,f1,f2,f3;
	IpAddr -> GetAddress(f0, f1, f2, f3);
	sprintf_s(clientdata -> ServerIp, "%d.%d.%d.%d",f0, f1, f2, f3);
	GetDlgItem(IDC_EDIT_PORT) -> GetWindowText(clientdata -> Port);
	GetDlgItem(IDC_EDIT_USER_NAME) -> GetWindowText(clientdata -> UserName);
	GetDlgItem(IDC_EDIT_PASS) -> GetWindowText(clientdata -> UserPass);

	std::cout << std::endl;
	std::cout << "Regester" << std::endl;
	std::cout << "IP: " << clientdata -> ServerIp << std::endl;
	std::wcout << "Port: " << (LPCTSTR)clientdata -> Port << std::endl;
	std::wcout << "User: " << (LPCTSTR)clientdata -> UserName << std::endl;
	std::wcout << "Pass: " << (LPCTSTR)clientdata -> UserPass << std::endl;
	std::cout << std::endl;

	int result = clientdata -> Info();
	switch (result)
	{
	case 1:
		{
			//MessageBoxExA(NULL, "�˿ڲ���Ϊ��", "error", 0, MB_OK);
			AfxMessageBox(_T("�˿ڲ���Ϊ��"));
			return ;
		}
	case 2:
		{
			//MessageBoxExA(NULL, "�û�������Ϊ��", "error", 0, MB_OK);
			AfxMessageBox(_T("�û�������Ϊ��"));
			return ;
		}
	case 3:
		{
			//MessageBoxExA(NULL, "���벻��Ϊ��", "error", 0, MB_OK);
			AfxMessageBox(_T("���벻��Ϊ��"));
			return ;
		}
	default:
		break;
	}
	clientdata -> Register();
}

