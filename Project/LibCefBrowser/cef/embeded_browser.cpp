#include "stdafx.h"
#include "cef/embeded_browser.h"

#include "include/wrapper/cef_closure_task.h"
#include "include/base/cef_bind.h"

#include "cef/client_handler.h"
#include "cef/client_handler_puppet.h"
#include "int/Cef.h"

bool EmbededBrowser::CreatBrowser(HWND hWnd, std::string& url)
{
	if (!Cef::IsInitialed())
		return false;
	// SimpleHandler implements browser-level callbacks.
	CefRefPtr<ClientHandler> handler(ClientHandlerPuppet::Instance().CreateClientHandler());

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	// Information used when creating the native window.
	CefWindowInfo window_info;

	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	RECT browser_rect;
	::GetClientRect(hWnd, &browser_rect);
	window_info.SetAsChild(hWnd, browser_rect);

	// Create the first browser window.
	return CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
		NULL);
}

EmbededBrowser::EmbededBrowser() : m_cef_browser(nullptr)
{}

EmbededBrowser::~EmbededBrowser()
{}

void EmbededBrowser::Attach(CefRefPtr<CefBrowser> cef_browser)
{
	m_cef_browser = cef_browser;

}

void EmbededBrowser::Detach(CefRefPtr<CefBrowser> cef_browser)
{
	if (nullptr != m_cef_browser && m_cef_browser->IsSame(cef_browser))
		m_cef_browser = nullptr; // Release reference of browser instance.
}

BrowserI& EmbededBrowser::BrowserInterface()
{
	return *this;
}

bool EmbededBrowser::IsAttached() const
{
	return (nullptr != m_cef_browser);
}

void EmbededBrowser::Close(bool force_close /*= false*/)
{
	if (nullptr == m_cef_browser)
		return;
	// get window handle
	m_cef_browser->GetHost()->CloseBrowser(force_close);
}

void EmbededBrowser::Resize(const RECT& rect)
{
	if (nullptr == m_cef_browser)
		return;

	// cef stretch to to full view
	CefWindowHandle hwnd = m_cef_browser->GetHost()->GetWindowHandle();
	if (hwnd)
	{
		HDWP hdwp = BeginDeferWindowPos(1);
		hdwp = DeferWindowPos(hdwp, hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		EndDeferWindowPos(hdwp);
	}
}

// Set focus to |browser| on the UI thread.
static void SetFocusToBrowser(CefRefPtr<CefBrowser> browser)
{
	if (!CefCurrentlyOn(TID_UI))
	{
		// Execute on the UI thread.
		CefPostTask(TID_UI, base::Bind(&SetFocusToBrowser, browser));
		return;
	}
	// Give focus to the browser.
	browser->GetHost()->SetFocus(true);
}

void EmbededBrowser::SetFocus()
{
	if (nullptr == m_cef_browser)
		return;
	SetFocusToBrowser(m_cef_browser);
}

void EmbededBrowser::Move()
{
	if (nullptr == m_cef_browser)
		return;
	m_cef_browser->GetHost()->NotifyMoveOrResizeStarted();
}

void EmbededBrowser::Paint()
{
	if (nullptr == m_cef_browser)
		return;
	PAINTSTRUCT ps;
	HDC hdc;
	HWND hwnd = m_cef_browser->GetHost()->GetWindowHandle();
	hdc = ::BeginPaint(hwnd, &ps);
	::EndPaint(hwnd, &ps);
}

void EmbededBrowser::Load(const std::string& url)
{
	if (nullptr == m_cef_browser)
		return;
	m_cef_browser->GetMainFrame()->LoadURL(url);
}

void EmbededBrowser::Reload()
{
	if (nullptr == m_cef_browser)
		return;

	m_cef_browser->Reload();
}

void EmbededBrowser::Farward()
{
	if (nullptr == m_cef_browser)
		return;

	if (m_cef_browser->CanGoForward())
		m_cef_browser->GoForward();
}

void EmbededBrowser::Back()
{
	if (nullptr == m_cef_browser)
		return;

	if (m_cef_browser->CanGoBack())
		m_cef_browser->GoBack();
}
