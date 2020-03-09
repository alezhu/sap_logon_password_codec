#include "CAboutDlg.h"

LRESULT CAboutDlg::OnInitDialog(_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam,
	_In_ BOOL& bHandled)
{
	auto result = CSimpleDialog::OnInitDialog(uMsg,wParam,lParam,bHandled);

	mLink.Attach(GetDlgItem(IDC_LINK));

	return result;
}
