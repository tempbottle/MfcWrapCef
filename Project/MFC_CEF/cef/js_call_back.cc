#include "stdafx.h"

#include "cef\js_call_back.h"

#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "int\JSHandler.h"

namespace{

	typedef struct  
	{
		std::string action_name;
		std::string action_data;
	}JSPackage;


	bool ParserJSRequest(const std::string& str_request, JSPackage& js_request)
	{
		std::stringstream ss;
		ss << str_request;

		namespace PT = boost::property_tree;
		try
		{
			PT::ptree ptree;
			PT::read_json(ss, ptree);
			js_request.action_name = ptree.get<std::string>("action_name");
			js_request.action_data = ptree.get <std::string>("request_data");
		}
		catch (...)
		{
			if (js_request.action_name.empty())
				js_request.action_name = "unknow";
			js_request.action_data = "json format error!";
			return false;
		}
		return true;
	}

	std::string MakeJSResponse(const JSPackage& js_response)
	{
		std::stringstream ss;
		std::string str_response;

		namespace PT = boost::property_tree;
		try
		{
			PT::ptree ptree;
			ptree.put<std::string>("action_name", js_response.action_name);
			ptree.put<std::string>("response_data", js_response.action_data);
			PT::write_json(ss, ptree, true);
		}
		catch (...)
		{
			return "{action_name:\"" + js_response.action_name + "\",response_data:\"" + js_response.action_data + "\"}";
		}
		return ss.str();
	}

	class Handler :public CefMessageRouterBrowserSide::Handler
	{
		virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int64 query_id,
			const CefString& request,
			bool persistent,
			CefRefPtr<Callback> callback) {

			JSPackage js_request;
			JSPackage js_response;
			if (!ParserJSRequest(request.ToString(),js_request))
			{
				js_response = js_request;
				callback->Failure((int)query_id, MakeJSResponse(js_response));
				return true; // cancle the next js handler to handing this request.
			}
			js_response.action_name = js_request.action_name;
			auto is_success = JSHandlerChain::Handing(js_request.action_name, js_request.action_data, js_response.action_data);
			if (is_success)
			{
				callback->Success(MakeJSResponse(js_response));
				return true;
			}
			return false;
		}

		virtual void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int64 query_id) {}

	};
}

void JS_Call_Back::CreateMessageHandlers(ClientHandler::MessageHandlerSet& handlers)
{
	handlers.insert(new Handler());
}

