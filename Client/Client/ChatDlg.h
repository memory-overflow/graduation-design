
#pragma once

#include "Resource.h"
#include "map"
// CChatDlg �Ի���


class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHAT };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual void OnCancel();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSendChat();
	afx_msg void OnBnClickedButtonClose();
	CEdit *m_edit_send;
	CEdit *m_edit_message;
	CString name;
	SOCKET *conn;
	std::map<CString, CString> *message;
	std::map<CString, CChatDlg*> *onChat;
};
