
// TrayIconDlg.h : 头文件
//

#pragma once

#define WM_TRAYICON WM_USER + 101

// CTrayIconDlg 对话框
class CTrayIconDlg : public CDialogEx
{
// 构造
public:
	CTrayIconDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRAYICON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); // 重载此消息，在创建程序的时候Add托盘图标
	afx_msg void OnClose();		// 重载此消息，在关闭窗口的时候Delete托盘图标
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHide();
	afx_msg void OnDisplay();
	afx_msg void OnAbout();
	afx_msg void OnExit();
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedTraytestbtn();
public:
	NOTIFYICONDATA m_nid;
	CMenu m_menu;
};
