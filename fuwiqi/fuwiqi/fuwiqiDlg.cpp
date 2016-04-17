
// fuwiqiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "fuwiqi.h"
#include "fuwiqiDlg.h"
#include "afxdialogex.h"
#include "Server.h"

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


// CfuwiqiDlg �Ի���



CfuwiqiDlg::CfuwiqiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfuwiqiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfuwiqiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfuwiqiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CfuwiqiDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CfuwiqiDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CfuwiqiDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CfuwiqiDlg ��Ϣ�������

BOOL CfuwiqiDlg::OnInitDialog()
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

	// ��ȡ����ip��ʾ
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	char Name[256] = {0};
	if(!gethostname(Name, 256))
	{
		hostent *hostinfo;
		if(hostinfo = gethostbyname(Name))
		{
			CString ip = (CString)inet_ntoa(*(struct in_addr*)*hostinfo -> h_addr_list);
			GetDlgItem(IDC_IPADDRESS1) -> SetWindowTextW(ip);
		}
	}
	WSACleanup();
	// ��ʾ�˿�
	GetDlgItem(IDC_EDIT3) -> SetWindowTextW(CString("8080"));
	//����button disbale
	GetDlgItem(IDC_BUTTON1) -> EnableWindow(true);
	GetDlgItem(IDC_BUTTON2) -> EnableWindow(false);
	GetDlgItem(IDC_BUTTON3) -> EnableWindow(true);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CfuwiqiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CfuwiqiDlg::OnPaint()
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
HCURSOR CfuwiqiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CfuwiqiDlg::OnBnClickedButton1()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("[%02d:%02d:%02d]"), time.wHour, time.wMinute, time.wSecond);
	if(Serverspace::StartServer())
	{
		GetDlgItem(IDC_BUTTON1) -> EnableWindow(false);
		GetDlgItem(IDC_BUTTON2) -> EnableWindow(true);
		UpdataEvent(CString("�����������ɹ�"));
	}
	else
	{
		UpdataEvent(CString("����������ʧ��"));
	}
}


void CfuwiqiDlg::OnBnClickedButton2()
{
	 
	Serverspace::StopServer();
	GetDlgItem(IDC_BUTTON1) -> EnableWindow(true);
	GetDlgItem(IDC_BUTTON2) -> EnableWindow(false);
	((CListBox*)GetDlgItem(IDC_LIST2)) -> ResetContent();
	//UpdateWindow();
	UpdataEvent(CString("������ͣ�óɹ�"));
}



void CfuwiqiDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Serverspace::GetServerStaues())
	{
		Serverspace::StopServer();
	}
	EndDialog(IDOK);
}

void CfuwiqiDlg::UpdataEvent(CString eve)
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("[%4d-%02d-%02d %02d:%02d:%02d]"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	CListBox *m_listbox = (CListBox*)GetDlgItem(IDC_LIST4);
	m_listbox -> AddString(strTime + eve);
	int count = m_listbox -> GetCount();
	m_listbox -> SetCurSel(count - 1);
	CString str;
	str.Format(_T("�û�(%d)��"),Serverspace::GetUserCount());
	SetDlgItemText(IDC_STATIC2, str);
}