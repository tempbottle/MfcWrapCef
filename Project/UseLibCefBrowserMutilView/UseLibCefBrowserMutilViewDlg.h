
// UseLibCefBrowserMutilViewDlg.h : 头文件
//

#pragma once

#include "BrowserView.h"

// CUseLibCefBrowserMutilViewDlg 对话框
class CUseLibCefBrowserMutilViewDlg : public CDialogEx
{
// 构造
public:
	CUseLibCefBrowserMutilViewDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_USELIBCEFBROWSERMUTILVIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:

	CBrowserView	m_browser_view_left;
	CBrowserView	m_browser_view_right;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
