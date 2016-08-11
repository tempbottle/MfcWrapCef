
// BrowserView.cpp : CBrowserView 类的实现
//

#include "stdafx.h"
#include "BrowserView.h"

#include "util\wm_cef_event.h"
#include "cef\embeded_browser.h"
#include "int\Cef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBrowserView

CBrowserView::CBrowserView(const std::string& start_url) 
	: m_start_url(start_url), m_browser_proxy(new EmbededBrowser), m_is_popuwindow_allowable(false)
{}

CBrowserView::~CBrowserView()
{}


BEGIN_MESSAGE_MAP(CBrowserView, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(WM_CEF_TITLE_CHANGE, &CBrowserView::OnCefTitleChange)
	ON_MESSAGE(WM_CEF_AFTER_CREATED, &CBrowserView::OnCefAfterCreated)
	ON_MESSAGE(WM_CEF_DO_CLOSE, &CBrowserView::OnCefDoClose)
	ON_MESSAGE(WM_CEF_BEFORE_CLOSE, &CBrowserView::OnCefBeforeclose)
	ON_MESSAGE(WM_CEF_LOAD_ERROR, &CBrowserView::OnCefLoaderror)
	ON_MESSAGE(WM_CEF_LOAD_END, &CBrowserView::OnCefLoadEnd)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_SIZING()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_CEF_BEFORE_POPUP, &CBrowserView::OnCefBeforePopup)
END_MESSAGE_MAP()


// CBrowserView 消息处理程序

std::string CBrowserView::GetTitile()
{
	return m_titile;
}

BrowserI& CBrowserView::GetBrowserI()
{
	return m_browser_proxy->BrowserInterface();
}

BOOL CBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

BOOL CBrowserView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, 
	CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类

	if (CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		EmbededBrowser::CreatBrowser(GetSafeHwnd(), m_start_url);
		return TRUE;
	}

	return  FALSE;
}

BOOL CBrowserView::DestroyWindow()
{
	// TODO:  在此添加专用代码和/或调用基类

	if (!m_browser_proxy->IsAttached())
		m_browser_proxy->Close(false);

	return CWnd::DestroyWindow();
}

void CBrowserView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	// TODO:  在此处添加消息处理程序代码

	// 不要为绘制消息而调用 CWnd::OnPaint()
}

void CBrowserView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	RECT client_rect;
	GetClientRect(&client_rect);
	m_browser_proxy->Resize(client_rect);
}

void CBrowserView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	m_browser_proxy->SetFocus();
}

void CBrowserView::OnClose()
{
	if (m_browser_proxy->IsAttached())
	{
		m_browser_proxy->Close(false);
	}
	CWnd::OnClose();
}

void CBrowserView::OnMove(int x, int y)
{
	CWnd::OnMove(x, y);

	m_browser_proxy->Move();
}

void CBrowserView::OnMoving(UINT fwSide, LPRECT pRect)
{
	CWnd::OnMoving(fwSide, pRect);

	m_browser_proxy->Move();
}

void CBrowserView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CWnd::OnSizing(fwSide, pRect);

	RECT client_rect;
	GetClientRect(&client_rect);
	m_browser_proxy->Resize(client_rect);
}

BOOL CBrowserView::OnEraseBkgnd(CDC* pDC)
{
	if (m_browser_proxy->IsAttached())
		return FALSE;
	return CWnd::OnEraseBkgnd(pDC);
}
