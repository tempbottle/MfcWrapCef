#ifndef CEF_CLIENT_HANDLER_PUPPET_H_
#define CEF_CLIENT_HANDLER_PUPPET_H_

#include <list>

#include "include/internal/cef_ptr.h"

class ClientHandler;
class ClientHandlerPuppet
{
private:
	ClientHandlerPuppet();
	~ClientHandlerPuppet();

public:
	static ClientHandlerPuppet& Instance();
	CefRefPtr<ClientHandler> CreateClientHandler();
	bool IsAllBrowserGroupsClosing();

private:
	typedef std::list< CefRefPtr<ClientHandler> >	ClientHandlerList;
	ClientHandlerList m_client_handler_list;
};

#endif // CEF_CLIENT_HANDLER_PUPPET_H_