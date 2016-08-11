#ifndef INT_JS_HANDLER_H_
#define INT_JS_HANDLER_H_

#include <memory>
#include <string>

class JSHandlerChain
{
public:
	static void Register(std::shared_ptr<JSHandlerChain> handler);
	static bool Handing(const std::string& action_name, const std::string& rq, std::string& sp);

protected:
	JSHandlerChain(const std::string& action_name);
	virtual ~JSHandlerChain();

private:
	void SetPredecessor(std::shared_ptr<JSHandlerChain> handler);

	bool Handler(const std::string& action_name, const std::string& rq, std::string& sp);

	virtual std::string  Deal(const std::string& rq) = 0;

private:
	std::shared_ptr<JSHandlerChain> m_predecessor;
	std::string m_action_name;
};

class JSHandlerTest : public JSHandlerChain
{
public:
	JSHandlerTest(const std::string& action_name);
private:
	std::string  Deal(const std::string& rq);
};


#endif // INT_JS_HANDLER_H_