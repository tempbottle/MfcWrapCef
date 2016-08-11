// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "stdafx.h"

#include "cef/client_handler.h"

#include <sstream>
#include <string>
#include <algorithm>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "util/wm_cef_event.h"
#include "cef/js_call_back.h"

ClientHandler::ClientHandler(): m_is_closing(false) {}
ClientHandler::~ClientHandler() {}

// static
//ClientHandler& ClientHandler::Instance()
//{
//	static CefRefPtr<ClientHandler> instance(new ClientHandler);
//	return *instance;
//}

bool ClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_message_router->OnProcessMessageReceived(browser, source_process, message))
		return true;

	return CefClient::OnProcessMessageReceived(browser, source_process, message);
}

void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status)
{
	CEF_REQUIRE_UI_THREAD();

	m_message_router->OnRenderProcessTerminated(browser);
}

bool ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
	CEF_REQUIRE_UI_THREAD();

	m_message_router->OnBeforeBrowse(browser, frame);

	return CefRequestHandler::OnBeforeBrowse(browser, frame, request, is_redirect);
}

void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) 
{
	CEF_REQUIRE_UI_THREAD();

	HWND hWindow = GetParent(browser->GetHost()->GetWindowHandle());

	std::string browser_title(title.ToString());
	::SendMessage(hWindow, WM_CEF_TITLE_CHANGE,WPARAM(browser.get()),LPARAM(&browser_title));

}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) 
{
	CEF_REQUIRE_UI_THREAD();

	if (!m_message_router) 
	{
		// Create the browser-side router for query handling.
		CefMessageRouterConfig config;
		m_message_router = CefMessageRouterBrowserSide::Create(config);

		// Register handlers with the router.
		JS_Call_Back::CreateMessageHandlers(m_message_handler_set);

		for (const auto& handler : m_message_handler_set)
			m_message_router->AddHandler(handler, false);
	}

	// Add to the list of existing browsers.
	m_browser_list.push_back(browser);

	HWND hWindow = GetParent(browser->GetHost()->GetWindowHandle());
	::SendMessage(hWindow, WM_CEF_AFTER_CREATED, WPARAM(browser.get()), LPARAM(NULL));

	CefLifeSpanHandler::OnAfterCreated(browser);
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser) 
{
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (m_browser_list.size() == 1) 
	{
		// Set a flag to indicate that the window close should be allowed.
		m_is_closing = true;
	}

	HWND hWindow = GetParent(browser->GetHost()->GetWindowHandle());
	::SendMessage(hWindow, WM_CEF_DO_CLOSE, WPARAM(browser.get()), LPARAM(NULL));


	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
{
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	for (auto& browser_elm:m_browser_list)
	{
		if (browser_elm->IsSame(browser))
		{
			m_browser_list.erase(std::find(m_browser_list.begin(), m_browser_list.end(), browser_elm));
			break;
		}
	}

	if (m_browser_list.empty()) 
	{
		// Remove and delete message router handlers.
		for (auto& handler:m_message_handler_set) 
		{
			m_message_router->RemoveHandler(handler);
			delete handler;
		}
		m_message_handler_set.clear();
		m_message_router = NULL;
	}

	HWND hWindow = GetParent(browser->GetHost()->GetWindowHandle());
	::SendMessage(hWindow, WM_CEF_BEFORE_CLOSE, WPARAM(browser.get()), LPARAM(NULL));
}

void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;
  
  std::string wm_error_info;

  HWND hWindow = GetParent(browser->GetHost()->GetWindowHandle());

  ::SendMessage(hWindow, WM_CEF_BEFORE_CLOSE, WPARAM(browser.get()), LPARAM(&wm_error_info));

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL " << std::string(failedUrl) <<
        " with error " << std::string(errorText)<<
		" (" << errorCode <<").</h2>"
		"<p>"<<wm_error_info<<"</p>"
		"</body></html>";
  frame->LoadString(ss.str(), failedUrl);
}

void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	HWND hwnd_browser = browser->GetHost()->GetWindowHandle();
	HWND hWindow = GetParent(hwnd_browser);

	::PostMessage(hWindow, WM_CEF_LOAD_END, WPARAM(browser.get()), LPARAM(httpStatusCode));
}

void ClientHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI,
        base::Bind(&ClientHandler::CloseAllBrowsers, this, force_close));
    return;
  }

  if (m_browser_list.empty())
    return;

  BrowserList::const_iterator it = m_browser_list.begin();
  for (; it != m_browser_list.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

bool ClientHandler::IsClosing() const
{
	return m_is_closing;
}
