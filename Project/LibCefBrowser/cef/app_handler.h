// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_APP_HANDLER_H_
#define CEF_APP_HANDLER_H_

#include <set>

#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class AppHandler : public CefApp,
				   public CefResourceBundleHandler,
                   public CefBrowserProcessHandler,
				   public CefRenderProcessHandler
{
public:
	AppHandler();

	// CefApp METHODS:
	virtual void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line);

	virtual void OnRegisterCustomSchemes(
		CefRefPtr<CefSchemeRegistrar> registrar);

	virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler();

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler();

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler();

	// CefResourceBundleHandler METHODS:
	virtual bool GetLocalizedString(int string_id,
		CefString& string);

	virtual bool GetDataResource(int resource_id,
		void*& data,
		size_t& data_size);

	virtual bool GetDataResourceForScale(int resource_id,
		ScaleFactor scale_factor,
		void*& data,
		size_t& data_size);


  // CefBrowserProcessHandler METHODS:
	virtual void OnContextInitialized();

	virtual void OnBeforeChildProcessLaunch(
		CefRefPtr<CefCommandLine> command_line);

	virtual void OnRenderProcessThreadCreated(
		CefRefPtr<CefListValue> extra_info);

	virtual CefRefPtr<CefPrintHandler> GetPrintHandler();

	virtual void OnScheduleMessagePumpWork(int64 delay_ms);


public:
	// Interface for renderer delegates. All Delegates must be returned via
	// CreateDelegates. Do not perform work in the Delegate
	// constructor. See CefRenderProcessHandler for documentation.
	class RendererDelegate : public virtual CefBase 
	{
	public:
		virtual void OnRenderThreadCreated(CefRefPtr<AppHandler> app,
			CefRefPtr<CefListValue> extra_info) {}

		virtual void OnWebKitInitialized(CefRefPtr<AppHandler> app) {}

		virtual void OnBrowserCreated(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser) {}

		virtual void OnBrowserDestroyed(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser) {}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler(
			CefRefPtr<AppHandler> app) 
		{ return NULL; }

		virtual bool OnBeforeNavigation(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			cef_navigation_type_t navigation_type,
			bool is_redirect) 
		{ return false;	}

		virtual void OnContextCreated(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) {}

		virtual void OnContextReleased(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) {}

		virtual void OnUncaughtException(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context,
			CefRefPtr<CefV8Exception> exception,
			CefRefPtr<CefV8StackTrace> stackTrace) {}

		virtual void OnFocusedNodeChanged(CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefDOMNode> node) {}

		// Called when a process message is received. Return true if the message was
		// handled and should not be passed on to other handlers. Delegates
		// should check for unique message names to avoid interfering with each
		// other.
		virtual bool OnProcessMessageReceived(
			CefRefPtr<AppHandler> app,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) 
		{ return false; }
	};

	typedef std::set<CefRefPtr<RendererDelegate> > RendererDelegateSet;

	// CefRenderProcessHandler METHODS:
	void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;

	void OnWebKitInitialized() OVERRIDE;

	void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;

	CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

	bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		NavigationType navigation_type,
		bool is_redirect) OVERRIDE;

	void OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;

	void OnContextReleased(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;

	void OnUncaughtException(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context,
		CefRefPtr<CefV8Exception> exception,
		CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;

	void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefDOMNode> node) OVERRIDE;

	bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
	// Set of supported Delegates.
	RendererDelegateSet m_delegates;

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(AppHandler);
};

#endif  // CEF_APP_HANDLER_H_
