#pragma once

#include "ClientData.h"
// CScreenDlg 对话框

class CScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenDlg)

public:
	CScreenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;

	// 生成的消息映射函数
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
