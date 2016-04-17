#pragma once

#include "ClientData.h"
// CScreenDlg �Ի���

class CScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenDlg)

public:
	CScreenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	DECLARE_MESSAGE_MAP()
public:
	ClientData *clientdata;
	HANDLE m_thread;
	static DWORD WINAPI RecvMessage(LPVOID lpPara);
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnNMDblclkListUser(NMHDR *pNMHDR, LRESULT *pResult);
};
