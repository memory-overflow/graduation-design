// ScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ScreenDlg.h"
#include "afxdialogex.h"
#include "ChatDlg.h"
#include "iostream"
#include "memory"

// CScreenDlg �Ի���

IMPLEMENT_DYNAMIC(CScreenDlg, CDialogEx)

CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CScreenDlg::~CScreenDlg()
{
	OnBnClickedButtonExit();
	if(clientdata)
		delete clientdata;
	clientdata = NULL;
}

void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CScreenDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CScreenDlg::OnBnClickedButtonSend)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, &CScreenDlg::OnNMDblclkListUser)
END_MESSAGE_MAP()


// CScreenDlg ��Ϣ�������

void CScreenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else if(nID==SC_CLOSE)
	{
		OnBnClickedButtonExit();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

BOOL CScreenDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, WS_EX_APPWINDOW);

	CListCtrl *m_list = (CListCtrl*)GetDlgItem(IDC_LIST_USER);
	m_list -> SetExtendedStyle(m_list -> GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	TCHAR rgtsz[2][10] = {_T("�û�"), _T("")};
	LV_COLUMN lvcomlumn;
	CRect rect;
	m_list -> GetWindowRect(&rect);
	for(int i = 0 ;i < 2; ++i)
	{
		lvcomlumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
		lvcomlumn.fmt = LVCFMT_LEFT;
		lvcomlumn.pszText = rgtsz[i];
		lvcomlumn.iSubItem = i;
		lvcomlumn.iOrder = i;
		if(i==0)
		{
			lvcomlumn.cx = rect.Width()*3/4;
		}
		else
		{
			lvcomlumn.cx = rect.Width()*1/4;
		}
		m_list -> InsertColumn(i, &lvcomlumn);
	}

	m_thread = CreateThread(NULL, 0, RecvMessage, this, 0, NULL);
	
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScreenDlg::OnPaint()
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
HCURSOR CScreenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//ȡ����Ӧ���������¼�
BOOL CScreenDlg::PreTranslateMessage(MSG *pMsg)
{
	int falg = 1;
	if (WM_KEYFIRST <= pMsg -> message && pMsg -> message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
	    if(pMsg -> wParam==VK_RETURN)
	    {
			OnBnClickedButtonSend();
			return TRUE;
		}
		else if(pMsg -> hwnd == GetDlgItem(IDC_EDIT_MESSAGE) -> m_hWnd)
		{
			return TRUE;
		}
	}
	else if(pMsg -> message == WM_RBUTTONDOWN)
	{
		std::cout << "Rbutton " << std::endl;
		if(pMsg -> hwnd == GetDlgItem(IDC_EDIT_MESSAGE) -> m_hWnd)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CScreenDlg::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TerminateThread(m_thread, 0);
	EndDialog(WM_EXIT);
	clientdata -> Exit();

}


void CScreenDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ����Ⱥ��Ϣ
	std::cout << "����" << std::endl;
	CString message;
	((CEdit*)GetDlgItem(IDC_EDIT2)) -> GetWindowText(message);
	((CEdit*)GetDlgItem(IDC_EDIT2)) -> SetWindowTextW(_T(""));
	if(message.IsEmpty())
		return ;
	char SendBuf[1000];
	USES_CONVERSION;
	sprintf_s(SendBuf, "2+%s+%s", W2A(clientdata -> UserName), W2A(message));
	send(clientdata->SockClient, SendBuf, strlen(SendBuf), 0);
}

DWORD WINAPI CScreenDlg::RecvMessage(LPVOID lpPara)
{
	std::cout << "��̨�����ɹ�" << std::endl;
	CScreenDlg *screen = (CScreenDlg*)lpPara;
	char RecvBuf[1000] = {0};
	while(true)
	{
		memset(RecvBuf, 0, sizeof RecvBuf);
		int len = recv(screen -> clientdata -> SockClient, RecvBuf, 1000, 0);
		std::cout << RecvBuf << std::endl;
		if(len > 0)
		{
			std::cout << "recv:" << RecvBuf << std::endl;
			if(RecvBuf[0] == '0')
			{
				// �����˳�

				CString User;
				USES_CONVERSION;
				User.Format(_T("%s"), A2W(RecvBuf + 2));
				auto x = find(screen -> clientdata -> UserLists.begin(), screen -> clientdata -> UserLists.end(), User);
				(screen -> clientdata -> UserLists).erase(x);
				CString message;
				((CEdit*)(screen -> GetDlgItem(IDC_EDIT_MESSAGE))) -> GetWindowText(message);
				(screen -> GetDlgItem(IDC_EDIT_MESSAGE)) -> SetWindowTextW(message + CString(_T("\r\n")) +  User + CString(_T(" �˳������ҡ�")));
				
				CListCtrl *m_list = (CListCtrl*)screen -> GetDlgItem(IDC_LIST_USER);
				for(size_t i = 0 ;i < screen -> clientdata -> UserLists.size(); ++i)
				{
					if(screen -> clientdata -> UserLists[i] == User)
					{
						m_list -> DeleteItem(i);
						break;
					}
				}
				screen -> clientdata -> Usercount--;
				CString display;
				display.Format(_T("�����û�(%d)��"),screen -> clientdata -> Usercount);
				screen -> GetDlgItem(IDC_STATIC_USER) -> SetWindowTextW(display);
				delete screen->clientdata->h_Chat[User];
			}
			else if(RecvBuf[0] == '1')
			{
				// ���˵�½
				CString User;
				USES_CONVERSION;
				User.Format(_T("%s"), A2W(RecvBuf + 2));
				std::wcout << W2A(User) << std::endl;

				screen -> clientdata -> UserLists.push_back(User);
				CString message;
				((CEdit*)(screen -> GetDlgItem(IDC_EDIT_MESSAGE))) -> GetWindowText(message);
				(screen -> GetDlgItem(IDC_EDIT_MESSAGE)) -> SetWindowTextW(message + CString(_T("\r\n")) + User + CString(_T(" ���������ҡ�")));

				CListCtrl *m_list = (CListCtrl*)screen -> GetDlgItem(IDC_LIST_USER);
				int nIndex = m_list -> GetItemCount();
				LV_ITEM lvitemAdd = {0};
				lvitemAdd.mask = LVIF_TEXT;
				lvitemAdd.iItem = nIndex;
				lvitemAdd.iSubItem = 0;
				lvitemAdd.pszText = User.GetBuffer();
				m_list -> InsertItem(&lvitemAdd);
				screen -> clientdata -> Usercount++;
				CString display;
				display.Format(_T("�����û�(%d)��"),screen -> clientdata -> Usercount);
				screen -> GetDlgItem(IDC_STATIC_USER) -> SetWindowTextW(display);
				/*if(m_list -> InsertItem(&lvitemAdd) != -1)
				{
					LV_ITEM lvitem = {0};
					lvitem.mask = LVIF_TEXT;
					lvitem.iItem = nIndex;
					lvitem.iSubItem = 1;
					lvitem.pszText = _T("1");
					m_list -> SetItem(&lvitem);
				}*/
			}
			else if(RecvBuf[0] == '2')
			{
				// Ⱥ��Ϣ
				std::cout << "Ⱥ��Ϣ" << std::endl;
				char Name[1000] = {0}, Message[1000] = {0};
				int i = 2, j = 0;
				for(j = 0; RecvBuf[i]; ++i, ++j)
				{
					if(RecvBuf[i] == '+')
					{
						Name[j] = 0;
						break;
					}
					Name[j] = RecvBuf[i];
				}
				for(++i, j = 0; RecvBuf[i]; ++i, ++j)
				{
					Message[j] = RecvBuf[i];
				}
				Message[j] = 0;

				CString display;
				USES_CONVERSION;
				display.Format(_T("%s:\r\n    %s"), A2W(Name), A2W(Message));
				CString message;
				((CEdit*)(screen -> GetDlgItem(IDC_EDIT_MESSAGE))) -> GetWindowText(message);
				screen -> GetDlgItem(IDC_EDIT_MESSAGE) -> SetWindowTextW(message + CString(_T("\r\n")) + display);
			}
			else if(RecvBuf[0] == '3')
			{
				std::cout << "˽��" << std::endl;
				char Name[1000] = {0}, Message[1000] = {0};
				int i = 2, j = 0;
				for(j = 0; RecvBuf[i]; ++i, ++j)
				{
					if(RecvBuf[i] == '+')
					{
						Name[j] = 0;
						break;
					}
					Name[j] = RecvBuf[i];
				}
				for(++i, j = 0; RecvBuf[i]; ++i, ++j)
				{
					Message[j] = RecvBuf[i];
				}
				Message[j] = 0;

				CString User, display;
				USES_CONVERSION;
				User.Format(_T("%s"), A2W(Name));
				display.Format(_T("%s:\r\n    %s"), A2W(Name), A2W(Message));
				ClientData *data = screen->clientdata;
				if(!data->h_Chat.count(User))
				{
					data->messageLeft[User] += CString(_T("\r\n")) + display;
					data->messageCount[User]++;

					CListCtrl* m_list = (CListCtrl*)screen->GetDlgItem(IDC_LIST_USER);
					int count = m_list->GetItemCount();
					for(int i = 0; i < count; ++i)
					{
						if(m_list->GetItemText(i, 0) == User)
						{
							LV_ITEM lvitem = {0};
							lvitem.mask = LVIF_TEXT;
							lvitem.iItem = i;
							lvitem.iSubItem = 1;
							char num[10];
							_itoa_s(data->messageCount[User], num, 10);
							USES_CONVERSION;
							lvitem.pszText = A2W(num);
							m_list -> SetItem(&lvitem);
						}
					}

				}
				else
				{
					data->messageRead[User] += CString(_T("\r\n")) + display;
					data->h_Chat[User]->m_edit_message->SetWindowTextW(data->messageRead[User]);
					data->h_Chat[User]->m_edit_message->UpdateWindow();
					data->h_Chat[User]->m_edit_message->LineScroll(data->h_Chat[User]->m_edit_message->GetLineCount() - 1);
				}
			}
		}
		screen -> UpdateWindow();
		CEdit * m_edit = (CEdit*)screen -> GetDlgItem(IDC_EDIT_MESSAGE);
		m_edit -> LineScroll(m_edit -> GetLineCount() - 1);
	}
	
	return 0;
}



void CScreenDlg::OnNMDblclkListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem = -1;
	if(pNMItemActivate != NULL)
	{

		nItem = pNMItemActivate->iItem;
		CListCtrl* m_listctrl = (CListCtrl*)GetDlgItem(IDC_LIST_USER);
		CString User = m_listctrl->GetItemText(nItem, 0);
		USES_CONVERSION;
		std::wcout << W2A(User) << std::endl;
		if(!clientdata->h_Chat.count(User))
		{
			clientdata->h_Chat[User] = new CChatDlg();
			CChatDlg* chatWindow = clientdata->h_Chat[User];
			chatWindow->name = User;
			chatWindow->onChat = &clientdata->h_Chat;
			chatWindow->conn = &clientdata->SockClient;
			chatWindow->message = &clientdata->messageRead;
			chatWindow->Create(IDD_DIALOG_CHAT);
			chatWindow->ShowWindow(SW_SHOW);
			if(!clientdata->messageCount.count(User))
				clientdata->messageCount[User] = 0;
			if(!clientdata->messageRead.count(User))
				clientdata->messageRead[User] = "";
			if(!clientdata->messageLeft.count(User))
				clientdata->messageLeft[User] = "";
			clientdata->messageRead[User] += clientdata->messageLeft[User];
			clientdata->messageCount[User] = 0;
			
			chatWindow->m_edit_message->SetWindowTextW(clientdata->messageRead[User]);
			chatWindow->UpdateWindow();
			chatWindow->m_edit_message->LineScroll(chatWindow->m_edit_message->GetLineCount() - 1);

			LV_ITEM lvitem = {0};
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = nItem;
			lvitem.iSubItem = 1;
			lvitem.pszText = LPWSTR("");
			m_listctrl -> SetItem(&lvitem);
		}
		else
		{
			CChatDlg* chatWindow = clientdata->h_Chat[User];
			chatWindow->SetFocus();
		}
	}
}
