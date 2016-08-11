// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "stdafx.h"

#include <windows.h>

#include "cef.h"

#include "include/cef_sandbox_win.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/cef_parser.h"
#include "include/cef_task.h"
#include "include/cef_trace.h"
#include "include/cef_web_plugin.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "cef/embeded_browser.h"
#include "cef/app_handler.h"
#include "cef/client_handler.h"
#include "cef/client_handler_puppet.h"


// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically if using the required compiler version. Pass -DUSE_SANDBOX=OFF
// to the CMake command-line to disable use of the sandbox.
// Uncomment this line to manually enable sandbox support.
 #define CEF_USE_SANDBOX 1

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

namespace
{
	CefRefPtr<AppHandler> g_app;
	bool g_is_iniialed = false;
}

bool Cef::Init()
{
	if (g_is_iniialed)
		return true;
	// Enable High-DPI support on Windows 7 or newer.
	//CefEnableHighDPISupport();

	g_app = new AppHandler;

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(::GetModuleHandle(NULL));

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, g_app, NULL);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return false;
	}

	// Specify CEF global settings here.
	CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif
	
	settings.multi_threaded_message_loop = true; // Cef massege loop will be placed in other single thread.
	//settings.single_process = true;

	// SimpleApp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.

	// Initialize CEF.
	g_is_iniialed = CefInitialize(main_args, settings, g_app.get(), sandbox_info);;
	return g_is_iniialed;
}

bool Cef::IsInitialed()
{
	return g_is_iniialed;
}

bool Cef::CloseBrowser()
{
	return ClientHandlerPuppet::Instance().IsAllBrowserGroupsClosing();
}

void Cef::Exit()
{
	// release cef app.
	g_app = nullptr;
	// shutdown CEF
	if (g_is_iniialed)
		CefShutdown();
	g_is_iniialed = false;
}
