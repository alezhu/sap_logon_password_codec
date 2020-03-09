#pragma once
#include "stdafx.h"
#include "resource.h"
#include <atlddx.h>

class CAboutDlg :public CSimpleDialog<IDD_ABOUTBOX,TRUE>
{
public:

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CSimpleDialog<IDD_ABOUTBOX>)
	END_MSG_MAP()


	LRESULT OnInitDialog(
		_In_ UINT /*uMsg*/,
		_In_ WPARAM /*wParam*/,
		_In_ LPARAM /*lParam*/,
		_In_ BOOL& /*bHandled*/)
	{
		mLink.SubclassWindow(::GetDlgItem(this->m_hWnd, IDC_LINK));
		return TRUE;
	}


private:
	CHyperLink mLink;

};

