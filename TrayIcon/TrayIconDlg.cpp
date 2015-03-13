
// TrayIconDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TrayIcon.h"
#include "TrayIconDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTrayIconDlg *g_AuE; // 把ShowBallon定义成全局函数，并用这个全局变量去与对话框关联，这样就可以在类外使用。
									// 如果程序中还有其他类，要想在那些类中控制托盘图标的信息显示，就可以通过extern声明ShowBallon函数来实现。
void ShowBallon(LPCTSTR szMsg=L"你好，ILLI，有新的消息提示，请查看!", 
	LPCTSTR szTitle=L"提示消息", UINT uTimeout=5000, DWORD dwInfoFlgs=0x01);

void ShowBallon(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags)
{
	g_AuE->m_nid.uTimeout = uTimeout;
	g_AuE->m_nid.dwInfoFlags = (DWORD)dwInfoFlags; // None=0x00, Info=0x01, Warning=0x02, Error=0x03
	g_AuE->m_nid.uFlags |=  NIF_INFO ;
	lstrcpy(g_AuE->m_nid.szInfo, szMsg);
	lstrcpy(g_AuE->m_nid.szInfoTitle, szTitle);
	Shell_NotifyIcon(0x01, &g_AuE->m_nid); // Add=0x00, Delete=0x02, Modify=0x01;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTrayIconDlg 对话框




CTrayIconDlg::CTrayIconDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrayIconDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrayIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrayIconDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HIDE, &CTrayIconDlg::OnHide)
	ON_COMMAND(ID_DISPLAY, &CTrayIconDlg::OnDisplay)
	ON_COMMAND(ID_ABOUT, &CTrayIconDlg::OnAbout)
	ON_COMMAND(ID_EXIT, &CTrayIconDlg::OnExit)
	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TRAYTESTBTN, &CTrayIconDlg::OnBnClickedTraytestbtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrayIconDlg 消息处理程序

BOOL CTrayIconDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);   // 加上此句，则应用程序不能显示最小化按钮，因此不能最小化到托盘
	m_menu.LoadMenu(IDR_TRAYMENU); // 载入自定义的托盘右键菜单
	g_AuE = (CTrayIconDlg*)this;  // 初始化全局变量

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTrayIconDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if((nID & 0xFFF0) == SC_MINIMIZE)  // 最小化到托盘
	{
		ShowWindow(SW_HIDE);
	}
	else if(nID == WM_DESTROY)  // 关闭窗口时 删除托盘
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTrayIconDlg::OnPaint()
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
HCURSOR CTrayIconDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CTrayIconDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;  // 托盘图标ID，可以使用自定义的图标，这里为图简便，就直接使用了系统图标
	m_nid.uFlags =NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	m_nid.uTimeout = 5000;
	m_nid.uVersion = NULL;
	m_nid.dwStateMask = 0;
	m_nid.dwInfoFlags = (DWORD)0x01; // None=0x00, Info=0x01, Warning=0x02, Error=0x03
	m_nid.uCallbackMessage = WM_TRAYICON;  // 托盘消息关联，以便让托盘图标可以响应消息
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	lstrcpy(m_nid.szTip, L"托盘图标提示消息");
	Shell_NotifyIcon(NIM_ADD, &m_nid); // Add=0x00, Delete=0x02, Modify=0x01;

	return 0;
}

void CTrayIconDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Shell_NotifyIcon(NIM_DELETE, &m_nid);  // 关闭窗口时删除托盘图标
	CDialogEx::OnClose();
}

// 下面四个函数就是托盘右键菜单所对应的事件
void CTrayIconDlg::OnHide()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_HIDE);
}

void CTrayIconDlg::OnDisplay()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWNORMAL);
}

void CTrayIconDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CTrayIconDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	if(MessageBox(L"确定要退出应用程序吗？", L"系统提示", MB_OKCANCEL | MB_ICONQUESTION) != 1)
		return;
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	CDialogEx::OnCancel();
}

LRESULT CTrayIconDlg::OnTrayIcon( WPARAM wParam, LPARAM lParam )
{
	UINT uID;
	UINT uMsgID;
	POINT pt;
	uID = (UINT)wParam;
	uMsgID = (UINT)lParam;

	if(uID == IDR_MAINFRAME)
	{
		switch(uMsgID)
		{
		case WM_RBUTTONDOWN:
			{
				GetCursorPos(&pt);
				CMenu* m_subMenu = (CMenu*)m_menu.GetSubMenu(0);  // 在托盘图标上单击右键，显示自定义的右键菜单
				m_subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, AfxGetApp()->m_pMainWnd, TPM_LEFTALIGN);
				break;
			}
		case WM_LBUTTONDOWN:  // 在托盘图标上单击左键，交替此显示或隐藏窗口
			if(!IsWindowVisible())
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
			else
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			break;
		case NIN_BALLOONSHOW:// 显示提示消息
			//AfxMessageBox("提示显示！");
			break;
		case NIN_BALLOONUSERCLICK:// 在消息框中单击鼠标左键、或者按下滚轮
			AfxMessageBox(L"你单击了托盘图标消息提示！");
			break;
		case NIN_BALLOONTIMEOUT:// 单击关闭，或者超时隐藏， 或者在提示框中单击右键
			//AfxMessageBox("提示已超时！"); 
			break;
		case NIN_BALLOONHIDE:// 何时跳转到此消息？
			//AfxMessageBox("提示隐藏！");
			break;
		default:
			break;
		}
	}
	return 0;
}

void CTrayIconDlg::OnBnClickedTraytestbtn()  // 单击按钮，显示托盘提示信息
{
	// TODO: 在此添加控件通知处理程序代码
	ShowBallon();
}
