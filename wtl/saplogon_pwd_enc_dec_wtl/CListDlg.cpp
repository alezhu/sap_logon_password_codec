#include "CListDlg.h"

LRESULT CListDlg::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
		// Init the CDialogResize code
		DlgResize_Init(true, true, WS_SIZEBOX | WS_CLIPCHILDREN);
		// center the dialog on the screen
		CenterWindow();


		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

#if USE_RICH_EDIT
		mEdit = GetDlgItem(IDC_RICHEDIT21);
#else
		mEdit = GetDlgItem(IDC_EDIT);
#endif
			
		mEdit.SetWindowTextW(mData.c_str());
		return TRUE;
	
}

void CListDlg::setData(const std::vector<std::wstring>& data)
{
	std::wstring s;
	std::wstring LF(L"\r\n");
	for (const auto& piece : data) s += piece + LF;
	mData = s;
	
}

CListDlg::CListDlg()
{
#if USE_RICH_EDIT
	LoadLibrary(L"Msftedit.dll");
#endif // USE_RICH_EDIT
}

LRESULT CListDlg::OnCancel(WORD, WORD wID, HWND, BOOL&)
{
	EndDialog(wID);
	return 0;
}
