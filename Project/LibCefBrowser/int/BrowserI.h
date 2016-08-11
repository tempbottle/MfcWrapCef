#ifndef INT_BROWSER_I_H_
#define INT_BROWSER_I_H_

#include <string>

class BrowserI
{
protected:
	BrowserI(){}
	~BrowserI(){}

public:
	virtual void Load(const std::string& url) = 0;
	virtual void Reload() = 0;
	virtual void Farward() = 0;
	virtual void Back() = 0;
};

#endif // INT_BROWSER_I_H_