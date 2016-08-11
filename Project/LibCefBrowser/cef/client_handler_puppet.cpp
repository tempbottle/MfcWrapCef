#include "stdafx.h"

#include "cef/client_handler_puppet.h"

#include "cef/client_handler.h"


ClientHandlerPuppet::ClientHandlerPuppet()
{}

ClientHandlerPuppet::~ClientHandlerPuppet()
{
	ClientHandlerList::iterator itr = m_client_handler_list.begin();
	ClientHandlerList::iterator itr_end = m_client_handler_list.end();

	for (; itr != itr_end; itr++)
	{
		if (nullptr == *itr)
			continue;
		auto client_handler = *itr;
		if (!client_handler->IsClosing())
		{
			client_handler->CloseAllBrowsers(false);
			// TODO: log.
		}
	}
}

ClientHandlerPuppet& ClientHandlerPuppet::Instance()
{
	static ClientHandlerPuppet instance;
	return instance;
}

CefRefPtr<ClientHandler> ClientHandlerPuppet::CreateClientHandler()
{
	CefRefPtr<ClientHandler> client_handler = new ClientHandler;
	m_client_handler_list.push_back(client_handler);
	return client_handler;
}

bool ClientHandlerPuppet::IsAllBrowserGroupsClosing()
{
	static bool IS_FIRST_CALL = true;
	ClientHandlerList::iterator itr = m_client_handler_list.begin();

	bool is_all_closing = true;
	for (; itr != m_client_handler_list.end();)
	{
		auto client_handler = *itr;
		if (nullptr == client_handler || client_handler->IsClosing())
		{
			itr = m_client_handler_list.erase(itr);
			continue;
		}

		is_all_closing = false;
		if (IS_FIRST_CALL)
			client_handler->CloseAllBrowsers(false);
		++itr;
	}
	IS_FIRST_CALL = false;
	return is_all_closing;
}
