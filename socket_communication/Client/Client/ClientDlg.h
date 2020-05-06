﻿
// ClientDlg.h: 头文件
//

#pragma once


// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
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
	CString m_prompt_message;
	CString m_proMessage;
	afx_msg void OnBnClickedcreatecon();
	afx_msg void OnEnChangevalue();
	CWinThread* mythread;
	static UINT Multhread(LPVOID pParam);
	afx_msg void OnBnClickedButton2();
//	CEdit c_send;
	CEdit c_recv;
	CEdit c_send;
	afx_msg void OnEnChangesend();
	CString c_Send;
};
