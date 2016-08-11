// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "stdafx.h"

#include "cef/client_renderer.h"

#include <sstream>
#include <string>

#include "include/cef_dom.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"

namespace client {
namespace renderer {

namespace{
	class LoadHandler : public CefLoadHandler
	{
	public:
		LoadHandler(){}

		virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
			bool isLoading,
			bool canGoBack,
			bool canGoForward) {}

		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			TransitionType transition_type) {}

		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) {}

		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) {}
		IMPLEMENT_REFCOUNTING(CefLoadHandler);
	};
}// namespace

namespace {

	// Must match the value in client_handler.cc.
	const char kFocusedNodeChangedMessage[] = "ClientRenderer.FocusedNodeChanged";

	class ClientRenderDelegate : public AppHandler::RendererDelegate 
	{
	public:
		ClientRenderDelegate()
		: m_last_node_is_editable(false), m_load_handler(new LoadHandler)
		{}

		virtual void OnWebKitInitialized(CefRefPtr<AppHandler> app) OVERRIDE 
		{
			// Create the renderer-side router for query handling.
			CefMessageRouterConfig config;
			m_message_router = CefMessageRouterRendererSide::Create(config);
		}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler(CefRefPtr<AppHandler> app)
		{
			return m_load_handler;
		}

		virtual void OnContextCreated(CefRefPtr<AppHandler> app,
									CefRefPtr<CefBrowser> browser,
									CefRefPtr<CefFrame> frame,
									CefRefPtr<CefV8Context> context) OVERRIDE 
		{
			m_message_router->OnContextCreated(browser,  frame, context);
		}

		virtual void OnContextReleased(CefRefPtr<AppHandler> app,
									CefRefPtr<CefBrowser> browser,
									CefRefPtr<CefFrame> frame,
									CefRefPtr<CefV8Context> context) OVERRIDE 
		{
			m_message_router->OnContextReleased(browser,  frame, context);
		}

		virtual void OnFocusedNodeChanged(CefRefPtr<AppHandler> app,
									CefRefPtr<CefBrowser> browser,
									CefRefPtr<CefFrame> frame,
									CefRefPtr<CefDOMNode> node) OVERRIDE 
		{
			bool is_editable = (node.get() && node->IsEditable());
			if (is_editable != m_last_node_is_editable) 
			{
				// Notify the browser of the change in focused element type.
				m_last_node_is_editable = is_editable;
				CefRefPtr<CefProcessMessage> message =
					CefProcessMessage::Create(kFocusedNodeChangedMessage);
				message->GetArgumentList()->SetBool(0, is_editable);
				browser->SendProcessMessage(PID_BROWSER, message);
			}
		}

		virtual bool OnProcessMessageReceived(CefRefPtr<AppHandler> app,
										CefRefPtr<CefBrowser> browser,
										CefProcessId source_process,
										CefRefPtr<CefProcessMessage> message) OVERRIDE 
		{
			return m_message_router->OnProcessMessageReceived(
				browser, source_process, message);
		}

	private:
		bool											m_last_node_is_editable;

		// Handles the renderer side of query routing.
		CefRefPtr<CefMessageRouterRendererSide>			m_message_router;

		CefRefPtr<CefLoadHandler>						m_load_handler;

		IMPLEMENT_REFCOUNTING(ClientRenderDelegate);
	};

}  // namespace

void CreateDelegates(AppHandler::RendererDelegateSet& delegates) 
{
	delegates.insert(new ClientRenderDelegate);
}

}  // namespace renderer
}  // namespace client
