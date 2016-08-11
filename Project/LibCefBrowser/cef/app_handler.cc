// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "stdafx.h"

#include "cef/app_handler.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

#include "cef/client_handler.h"
#include "cef/client_renderer.h"

namespace {

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
}  // namespace

AppHandler::AppHandler() 
{}


void AppHandler::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	CefApp::OnBeforeCommandLineProcessing(process_type, command_line);
}


void AppHandler::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
{
	CefApp::OnRegisterCustomSchemes(registrar);
}


CefRefPtr<CefResourceBundleHandler> AppHandler::GetResourceBundleHandler()
{
	return this;
}


CefRefPtr<CefBrowserProcessHandler> AppHandler::GetBrowserProcessHandler()
{
	return this;
}


CefRefPtr<CefRenderProcessHandler> AppHandler::GetRenderProcessHandler()
{
	return this;
}


bool AppHandler::GetLocalizedString(int string_id, CefString& string)
{
	return false;
}


bool AppHandler::GetDataResource(int resource_id, void*& data, size_t& data_size)
{
	return false;
}


bool AppHandler::GetDataResourceForScale(int resource_id, ScaleFactor scale_factor, void*& data, size_t& data_size)
{
	return false;
}


void AppHandler::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();
	return CefBrowserProcessHandler::OnContextInitialized();
}


void AppHandler::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
	return CefBrowserProcessHandler::OnBeforeChildProcessLaunch(command_line);
}


void AppHandler::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
	return CefBrowserProcessHandler::OnRenderProcessThreadCreated(extra_info);
}


CefRefPtr<CefPrintHandler> AppHandler::GetPrintHandler()
{
	CEF_REQUIRE_UI_THREAD();
	return CefBrowserProcessHandler::GetPrintHandler();
}


void AppHandler::OnScheduleMessagePumpWork(int64 delay_ms)
{
	CEF_REQUIRE_UI_THREAD();
	return CefBrowserProcessHandler::OnScheduleMessagePumpWork(delay_ms);
}


void AppHandler::OnRenderThreadCreated(
	CefRefPtr<CefListValue> extra_info) {

	client::renderer::CreateDelegates(m_delegates);

	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnRenderThreadCreated(this, extra_info);
}


void AppHandler::OnWebKitInitialized() {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnWebKitInitialized(this);
}


void AppHandler::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnBrowserCreated(this, browser);
}


void AppHandler::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnBrowserDestroyed(this, browser);
}


CefRefPtr<CefLoadHandler> AppHandler::GetLoadHandler() {
	CefRefPtr<CefLoadHandler> load_handler;
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end() && !load_handler.get(); ++it)
		load_handler = (*it)->GetLoadHandler(this);

	return load_handler;
}


bool AppHandler::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	NavigationType navigation_type,
	bool is_redirect) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it) {
		if ((*it)->OnBeforeNavigation(this, browser, frame, request,
			navigation_type, is_redirect)) {
			return true;
		}
	}

	return false;
}


void AppHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnContextCreated(this, browser, frame, context);
}


void AppHandler::OnContextReleased(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnContextReleased(this, browser, frame, context);
}


void AppHandler::OnUncaughtException(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context,
	CefRefPtr<CefV8Exception> exception,
	CefRefPtr<CefV8StackTrace> stackTrace) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it) {
		(*it)->OnUncaughtException(this, browser, frame, context, exception,
			stackTrace);
	}
}


void AppHandler::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefDOMNode> node) {
	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end(); ++it)
		(*it)->OnFocusedNodeChanged(this, browser, frame, node);
}


bool AppHandler::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {
	DCHECK_EQ(source_process, PID_BROWSER);

	bool handled = false;

	RendererDelegateSet::iterator it = m_delegates.begin();
	for (; it != m_delegates.end() && !handled; ++it) {
		handled = (*it)->OnProcessMessageReceived(this, browser, source_process,
			message);
	}

	return handled;
}
