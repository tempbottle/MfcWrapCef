// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_CLIENT_RENDERER_H_
#define CEF_CLIENT_RENDERER_H_
#pragma once

#include "include/cef_base.h"
#include "cef/app_handler.h"

namespace client {
namespace renderer {

// Create the renderer delegate. Called from client_app_delegates_renderer.cc.
void CreateDelegates(AppHandler::RendererDelegateSet& delegates);

}  // namespace renderer
}  // namespace client

#endif  // CEF_CLIENT_RENDERER_H_
