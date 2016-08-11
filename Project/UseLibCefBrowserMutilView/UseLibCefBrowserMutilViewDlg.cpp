
// UseLibCefBrowserMutilViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UseLibCefBrowserMutilView.h"
#include "UseLibCefBrowserMutilViewDlg.h"
#include "afxdialogex.h"

#include "Cef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CUseLibCefBrowserMutilViewDlg 对话框



CUseLibCefBrowserMutilViewDlg::CUseLibCefBrowserMutilViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUseLibCefBrowserMutilViewDlg::IDD, pParent), 
	m_browser_view_left("www.baidu.com"), 
	m_browser_view_right("www.bing.com")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUseLibCefBrowserMutilViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUseLibCefBrowserMutilViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CUseLibCefBrowserMutilViewDlg 消息处理程序

BOOL CUseLibCefBrowserMutilViewDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUseLibCefBrowserMutilViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUseLibCefBrowserMutilViewDlg::OnPaint()
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
HCURSOR CUseLibCefBrowserMutilViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUseLibCefBrowserMutilViewDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	auto is_close_browser_done = Cef::CloseBrowser();
	if (!is_close_browser_done)
		return; // Wait the cef close all browsers, then "wm_close" event will be send to the app main frame wind.

	CDialogEx::OnClose();
}


int CUseLibCefBrowserMutilViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	const int view_width = 400;
	const int view_height = 300;
	const int horizontal_interval = 10;
	int x_pos = 5;
	int y_pos = 5;

	// TODO:  在此添加您专用的创建代码
	if (!m_browser_view_left.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(x_pos, y_pos, x_pos + view_width, y_pos + view_height), this, NULL, NULL))
		return -1;
	x_pos += (view_width + horizontal_interval);
	if (!m_browser_view_right.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(x_pos, y_pos, x_pos + view_width, y_pos + view_height), this, NULL, NULL))
		return -1;
	return 0;
}
