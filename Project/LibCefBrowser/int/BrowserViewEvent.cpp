#include "stdafx.h"

#include "BrowserView.h"

#include "util\string_converter.h"
#include "cef\embeded_browser.h"

#include <string>

afx_msg LRESULT CBrowserView::OnCefTitleChange(WPARAM wParam, LPARAM lParam)
{
	//std::string* title = (std::string*)lParam;
	//::SetWindowText(GetParent()->GetSafeHwnd(), str_2_wstr(title->c_str()).c_str());

	m_titile = *((std::string*)lParam);
	return 0;
}


afx_msg LRESULT CBrowserView::OnCefAfterCreated(WPARAM wParam, LPARAM lParam)
{
	m_browser_proxy->Attach((CefBrowser*)wParam);
	SendMessage(WM_SIZE); //  Make mfc wnd refresh, and update browser rect.
	return 0;
}


afx_msg LRESULT CBrowserView::OnCefDoClose(WPARAM wParam, LPARAM lParam)
{
	m_browser_proxy->Detach((CefBrowser*)wParam);
	return 0;
}


afx_msg LRESULT CBrowserView::OnCefBeforeclose(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT CBrowserView::OnCefLoaderror(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT CBrowserView::OnCefLoadEnd(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
