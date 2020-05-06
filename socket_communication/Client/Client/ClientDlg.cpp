
// ClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <string>
#include "graphics.h"

#pragma comment(lib, "WS2_32.lib") 

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET clientSocket;//设为全局套接字
int Count = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_proMessage(_T("准备就绪,请在确认端口号输入正确后点击创建连接以申请连接服务器"))
	, c_Send(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_proMessage, m_proMessage);
	DDX_Control(pDX, IDC_send, c_send);
	DDX_Control(pDX, IDC_recv, c_recv);
	DDX_Text(pDX, IDC_send, c_Send);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_createCon, &CClientDlg::OnBnClickedcreatecon)
	ON_EN_CHANGE(IDC_value, &CClientDlg::OnEnChangevalue)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_send, &CClientDlg::OnEnChangesend)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedcreatecon()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//显示版本号 lp     f12帮助 给地址 要检查一下版本
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		m_proMessage = (_T("请求协议失败"));
	}

	//2)创建socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == clientSocket) {//说白了就是-1
		m_proMessage = (_T("套接字创建失败"));
		WSACleanup();
	}

	int m_value = GetDlgItemInt(IDC_value);
	if(m_value!=0)
	{ 
		//3)获取服务器协议地址族
		SOCKADDR_IN addr = { 0 };
		addr.sin_family = AF_INET;//协议版本
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//本地ip
		addr.sin_port = htons(m_value);//端口号，注意使用本机应用未占用的端口号

		//4)连接服务器
		int r = connect(clientSocket, (sockaddr*)&addr, sizeof addr);
		if (r == -1) {
			m_proMessage = (_T("请求连接服务器失败"));
		}
		m_proMessage = (_T("连接已建立，请发送消息"));
	}else
		m_proMessage = (_T("端口号为0或未输入端口号"));

	mythread = AfxBeginThread(Multhread, this, THREAD_PRIORITY_IDLE);
	UpdateData(FALSE);
}


void CClientDlg::OnEnChangevalue()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

UINT CClientDlg::Multhread(LPVOID pParam)
{
	CClientDlg* pDlg = (CClientDlg*)pParam;//创建一个指向主对话框的句柄
	char recvBuff[1024] = {0};//数据用数组存起来
	int r;
	while (1) {
		r = recv(clientSocket, recvBuff, 1023, NULL);
		if (r > 0) {
			recvBuff[r] = 0;//不兼容:项目->属性->常规->字符集->多字符，
			//GetDlgItem(IDC_recv)->SetWindowText(recvBuff);
			CString str = (CString)recvBuff;
			pDlg->GetDlgItem(IDC_recv)->SetWindowText(str);
			//::SetWindowText(::GetDlgItem(pDlg->m_hWnd, IDC_recv), recvBuff);
		}
	}
	return 0;
}

void CClientDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	//连接已建立，开始发送
	char buff[1024];
	memset(buff, 0, 1024);//每次都清空，为了防止出错

	//获取要发送的内容
	//CString str;
	//c_send.GetWindowText(str);
	LPSTR p;
	p =c_Send.GetBuffer();
	strcpy(buff, p);
	
	send(clientSocket, buff, strlen(buff), NULL);
	//在这儿就不检查发不发得过去了
	UpdateData(FALSE);
}

void CClientDlg::OnEnChangesend()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
