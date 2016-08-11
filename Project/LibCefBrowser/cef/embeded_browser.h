#ifndef CEF_EMBEDED_BROWSER_H_
#define CEF_EMBEDED_BROWSER_H_

#include "include/internal/cef_ptr.h"
#include "include/cef_browser.h"

#include "int/BrowserI.h"

class EmbededBrowser : private BrowserI
{
public:
	static bool CreatBrowser(HWND hWnd, std::string& url);

	EmbededBrowser();
	~EmbededBrowser();

	void Attach(CefRefPtr<CefBrowser> cef_browser);
	void Detach(CefRefPtr<CefBrowser> cef_browser);

	BrowserI& BrowserInterface();

	bool IsAttached() const;

	void Close(bool force_close = false);

	void Resize(const RECT& rect);

	void SetFocus();

	void Move();

	void Paint();

	//override funcition.
private:
	void Load(const std::string& url);
	void Reload();
	void Farward();
	void Back();

private:
	CefRefPtr<CefBrowser> m_cef_browser; // keep reference to the CEF browser.
};
#endif // CEF_EMBEDED_BROWSER_H_
