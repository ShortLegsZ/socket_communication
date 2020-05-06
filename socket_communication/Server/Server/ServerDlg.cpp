
// ServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKADDR_IN cAddr = { 0 };
int len = sizeof cAddr;
SOCKET clientSocket[1024];
SOCKET serverSocket;
int count = 0;


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


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, s_proMessage(_T("准备就绪，准备接收客户端请求"))
	, s_Send(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_proMessage, s_proMessage);
	DDX_Control(pDX, IDC_EDIT3, s_recv);
	DDX_Control(pDX, IDC_EDIT4, s_send);
	DDX_Text(pDX, IDC_EDIT4, s_Send);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_rececon, &CServerDlg::OnBnClickedrececon)
	ON_BN_CLICKED(IDC_send, &CServerDlg::OnBnClickedsend)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	ThreadFlag = FALSE;

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

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedrececon()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//1)请求协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		s_proMessage = (_T("请求协议失败"));//这个错误一般不会出现，但是以防万一
	}

	//2)创建socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {//说白了就是-1
		s_proMessage = (_T("请求套接字失败"));
		WSACleanup();
	}
	
	//3)创建协议地址族
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//协议版本
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(10000);//端口 使用1w左右的

	//4）绑定端口
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		s_proMessage = (_T("绑定端口失败"));
		closesocket(serverSocket);
		WSACleanup();
	}

	//5）监听
	r = listen(serverSocket, 10);//创建队列，长度为10
	if (r == -1) {
		s_proMessage = (_T("创建监听失败"));
		closesocket(serverSocket);
		WSACleanup();
	}
	s_proMessage = (_T("创建监听成功,准备监听来自客户端的请求"));
	//次线程1接收到连接请求
	mythread[0] = AfxBeginThread(MulthreadA, this, THREAD_PRIORITY_IDLE);
	//HANDLE hMultiThread = AfxBeginThread(MulthreadA, /*(LPVOID)*/this, THREAD_PRIORITY_IDLE);
	//AfxBeginThread(MulthreadA, /*(LPVOID)*/this, THREAD_PRIORITY_IDLE);
	//一直等到有客户端连为止,所以这里需要创建另一个线程，使得即使主线程阻塞，下面的消息可以继续发送
	//次线程2用于接收内容和发送
	mythread[1] = AfxBeginThread(MulthreadB, /*(LPVOID)*/this, THREAD_PRIORITY_IDLE);

	UpdateData(FALSE);
}


void CServerDlg::OnBnClickedsend()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	char sendData[200] = {0};
	//memset(buff, 0, 1024);//每次都清空，为了防止出错
	//发送消息
	LPSTR p;
	p = s_Send.GetBuffer();
	strcpy(sendData, p);
	//CString s_str;
	//s_send.GetWindowText(s_str);
	send(clientSocket[count], sendData, strlen(sendData), NULL);
	UpdateData(FALSE);
}

//接收来自客户端的请求
UINT CServerDlg::MulthreadA(LPVOID pParam)
{
	CServerDlg* pDlg = (CServerDlg*)pParam;//创建一个指向主对话框的句柄
	char buff[1024];
	char buff1[1024];
	int r;
	while (1) {
		clientSocket[count] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		if (clientSocket[count] == SOCKET_ERROR) {
			char sstr[] = "客户端接入服务器错误";
			CString Str=(CString)sstr;
			pDlg->GetDlgItem(IDC_proMessage)->SetWindowText(Str);
			closesocket(serverSocket);
			//9)清除协议信息
			WSACleanup();
		}
		else {
			char sstr[] = "有客户端接入服务器";
			CString Str = (CString)sstr;
			pDlg->GetDlgItem(IDC_proMessage)->SetWindowText(Str);
		}
	}
	return 0;
	
}

//接收并发送消息
UINT CServerDlg::MulthreadB(LPVOID pParam)
{
	CServerDlg* pDlg = (CServerDlg*)pParam;//创建一个指向主对话框的句柄
	char buff[1024];
	char buff1[1024];
	int r;
	while (1) {
		r = recv(clientSocket[count], buff, 1023, NULL);
		if (r > 0) {
			buff[r] = 0;
			//s_recv = buff;
			memset(buff1, 0, 1024);//每次都清空，为了防止出错
			//显示在消息接收文本框
			//CString str = (CString)buff;
			//pDlg->GetDlgItem(IDC_EDIT3)->SetWindowText(str);
			::SetWindowText(::GetDlgItem(pDlg->m_hWnd, IDC_EDIT3), buff);
		}
	}
}