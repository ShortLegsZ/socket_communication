
// ServerDlg.h: 头文件
//
#pragma once

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "WS2_32.lib") 

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = nullptr);	// 标准构造函数


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString s_proMessage;
	afx_msg void OnBnClickedrececon();
	afx_msg void OnBnClickedsend();
	CWinThread* mythread[2];
	static UINT MulthreadA(LPVOID pParam);
	static UINT MulthreadB(LPVOID pParam);
	CEdit s_recv;
	int ThreadFlag;
	CEdit s_send;
	CString s_Send;
};
