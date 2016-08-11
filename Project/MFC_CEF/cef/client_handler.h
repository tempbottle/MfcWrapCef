// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_CLIENT_HANDLER_H_
#define CEF_CLIENT_HANDLER_H_

#include <list>
#include <set>
#include <atomic>

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"

class ClientHandlerPuppet;
class ClientHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
					  public CefRequestHandler{
private:
	ClientHandler();
	~ClientHandler();

	friend class ClientHandlerPuppet;

public:
	typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

public:
	// Provide access to the single global instance of this object.
	//static ClientHandler& Instance();

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;

	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
		CefProcessId source_process, 
		CefRefPtr<CefProcessMessage> message)OVERRIDE;

	// CefRequestHandler methods.
	virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, 
		TerminationStatus status) OVERRIDE;

	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, 
		CefRefPtr<CefFrame> frame, 
		CefRefPtr<CefRequest> request, 
		bool is_redirect)OVERRIDE;

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access)  OVERRIDE;

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, 
		CefRefPtr<CefFrame> frame, 
		ErrorCode errorCode, 
		const CefString& errorText, 
		const CefString& failedUrl) OVERRIDE;

	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, 
		int httpStatusCode) OVERRIDE;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

	bool IsClosing() const;


private:

	// Handles the browser side of query routing. The renderer side is handled
	// in client_renderer.cc.
	CefRefPtr<CefMessageRouterBrowserSide>	m_message_router;

	// Set of Handlers registered with the message router.
	MessageHandlerSet						m_message_handler_set;


private:
	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	typedef std::atomic<bool> AtomicBool;

	// List of existing browser windows. Only accessed on the CEF UI thread.
	BrowserList	m_browser_list;
	AtomicBool	m_is_closing;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(ClientHandler);
};

#endif  // CEF_CLIENT_HANDLER_H_
