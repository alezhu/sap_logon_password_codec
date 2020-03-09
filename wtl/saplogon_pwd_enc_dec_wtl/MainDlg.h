// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"

class CMainDlg : public CDialogImpl<CMainDlg>,	public CDialogResize<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_CLOSE, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)

		if (uMsg == WM_SYSCOMMAND && ID_APP_ABOUT == LOWORD(wParam))
		{ 
			bHandled = TRUE; 
			lResult = OnAppAbout(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); 
			if (bHandled) 
				return TRUE; 
		}

		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)

		COMMAND_HANDLER(IDC_BUTTON_ENCRYPT, BN_CLICKED, OnBnClickedButtonEncrypt)
		COMMAND_HANDLER(IDC_BUTTON_DECRYPT, BN_CLICKED, OnBnClickedButtonDecrypt)
		COMMAND_HANDLER(IDC_BUTTON_OPEN_FILE_DIALOG, BN_CLICKED, OnBnClickedButtonOpenFileDialog)
		COMMAND_HANDLER(IDC_BUTTON_DECRYPT_FILE, BN_CLICKED, OnBnClickedButtonDecryptFile)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CMainDlg)
		DLGRESIZE_CONTROL(IDC_EDIT_PASSWORD, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_ENCRYPT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_ENCRYPTED, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_DECRYPT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_FILE_PATH, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_OPEN_FILE_DIALOG, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON_DECRYPT_FILE, DLSZ_CENTER_X)
		DLGRESIZE_CONTROL(IDC_GROUP_FILE, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

		
private:
	CEdit m_edPassword = nullptr;
	CEdit m_edEncrypted = nullptr;
	CEdit m_edFilePath = nullptr;

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//LRESULT OnCloseCmd(
	//	_In_ UINT /*uMsg*/,
	//	_In_ WPARAM /*wParam*/,
	//	_In_ LPARAM /*lParam*/,
	//	_In_ BOOL& /*bHandled*/)
	//{
	//	this->EndDialog(FALSE);
	//	return TRUE;
	//}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  {
		auto result = CDialogResize<CMainDlg>::OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
		if ((m_ptMinTrackSize.y != -1))
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMaxTrackSize.y = m_ptMinTrackSize.y;
		}
		return result;
	}

	LRESULT OnBnClickedButtonEncrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void DoEncrypt();
	void DoDecrypt();
	
	void ExpandFilePath(_Inout_ CSimpleString& filePath);

	LRESULT OnBnClickedButtonDecrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonOpenFileDialog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonDecryptFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
