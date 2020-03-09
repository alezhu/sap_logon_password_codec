#pragma once

#include "stdafx.h"

#include "resource.h"


#define USE_RICH_EDIT true


class CListDlg : public CDialogImpl<CListDlg>, public CDialogResize<CListDlg>
{
public:
#if USE_RICH_EDIT
	enum { IDD = IDD_DIALOGLIST_RE };
#else
	enum { IDD = IDD_DIALOGLIST };
#endif

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_CLOSE, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CDialogResize<CListDlg>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CMainDlg)
#if USE_RICH_EDIT
		DLGRESIZE_CONTROL(IDC_RICHEDIT21, DLSZ_SIZE_X | DLSZ_SIZE_Y)
#else
		DLGRESIZE_CONTROL(IDC_EDIT, DLSZ_SIZE_X | DLSZ_SIZE_Y )
#endif
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_CENTER_X)
	END_DLGRESIZE_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void setData(const std::vector<std::wstring>& data);

	CListDlg();

private:
#if USE_RICH_EDIT
	CRichEditCtrl mEdit;
#else
	CEdit mEdit;
#endif
	std::wstring mData;

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

