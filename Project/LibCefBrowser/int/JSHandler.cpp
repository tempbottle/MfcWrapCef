#include "stdafx.h"

#include "int/JSHandler.h"

#include <mutex>

namespace{
	std::shared_ptr<JSHandlerChain>		g_head = nullptr;
	std::mutex							g_mutex;
}

void JSHandlerChain::Register(std::shared_ptr<JSHandlerChain> handler)
{
	std::lock_guard<std::mutex> lg(g_mutex);

	handler->SetPredecessor(g_head);
	g_head = handler;
}

bool JSHandlerChain::Handing(const std::string& action_name, const std::string& rq, std::string& sp)
{
	std::lock_guard<std::mutex> lg(g_mutex);
	if (nullptr == g_head)
	{
		sp = "";// TODO: log.
		return false;
	}
	return g_head->Handler(action_name, rq, sp);
}

JSHandlerChain::JSHandlerChain(const std::string& action_name)
	: m_action_name(action_name), m_predecessor(nullptr)
{}

JSHandlerChain::~JSHandlerChain()
{}

void JSHandlerChain::SetPredecessor(std::shared_ptr<JSHandlerChain> handler)
{
	m_predecessor = handler;
}

bool JSHandlerChain::Handler(const std::string& action_name, const std::string& rq, std::string& sp)
{
	if (0 == action_name.compare(m_action_name))
	{
		sp = Deal(rq);
		return true;
	}
	if (nullptr == m_predecessor)
	{
		sp = "error action handler not found.";// TODO: log.
		return false;
	}
	return m_predecessor->Handler(action_name, rq, sp);
}

JSHandlerTest::JSHandlerTest(const std::string& action_name) : JSHandlerChain(action_name)
{}

std::string JSHandlerTest::Deal(const std::string& rq)
{
	return "success!";
}
