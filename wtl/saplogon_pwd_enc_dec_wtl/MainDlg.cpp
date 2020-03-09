// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

#include <atlconv.h>
#include "encoder.hpp"
#include <atlenc.h>
#include <SapShortCut.h>
#include "CListDlg.h"
#include <vector>
#include "CAboutDlg.h"


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

	m_edPassword = GetDlgItem(IDC_EDIT_PASSWORD);
	m_edEncrypted = GetDlgItem(IDC_EDIT_ENCRYPTED);
	m_edFilePath = GetDlgItem(IDC_EDIT_FILE_PATH);

	auto file = LR"(%APPDATA%\SAP\Common\sapshortcut.ini)";
	m_edFilePath.SetWindowTextW(file);

	auto hMenu = GetSystemMenu(false);

	if(hMenu) {
		CString sAbout;
		if (sAbout.LoadStringW(ID_APP_ABOUT)) {
			AppendMenu(hMenu, MF_STRING, ID_APP_ABOUT, sAbout);
		}
	}

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}




LRESULT CMainDlg::OnBnClickedButtonEncrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoEncrypt();
	return 0;
}

static constexpr auto prefix = std::string_view("PW_");

std::vector<uint8_t> encryptStr(std::string_view sData) {
	std::vector<uint8_t> result(sData.cbegin(),sData.cend());
	
	encode_decode(result.data(), result.size());

	return std::move(result);
}

std::string toHex(std::vector<uint8_t> vData) {
	auto iHexLen = AtlHexEncodeGetRequiredLength(vData.size());
	std::string sResult(iHexLen,0);
	AtlHexEncode(vData.data(), vData.size(), sResult.data(), &iHexLen);
	return std::move(sResult);
}

std::string encrypt(std::string_view sData) {
	return std::move(toHex(encryptStr(sData)));
}


std::vector<uint8_t> fromHex(std::string_view sData) {

	auto iHexLen = AtlHexDecodeGetRequiredLength(sData.length());
	std::vector<uint8_t> vResult(iHexLen);

	AtlHexDecode(sData.data(), sData.length(), vResult.data(), &iHexLen);

	return vResult;
}

std::string decryptStr(std::vector<uint8_t> vData) {

	encode_decode(vData.data(), vData.size());

	return std::move(std::string(vData.cbegin(),vData.cend()));
}

std::string decrypt(std::string_view sData) {
	return std::move(decryptStr(fromHex(sData)));
}

void CMainDlg::DoEncrypt()
{
	CWaitCursor();
	CString wsPassword;
	auto iLen = m_edPassword.GetWindowTextW(wsPassword);
	if (!iLen) {
		m_edEncrypted.SetWindowTextW(L"");
		return;
	};
	
	std::string sDecrypted(CW2A(wsPassword.GetString()));

	auto sEncrypted = encrypt(sDecrypted);

	std::string sResult(prefix);
	sResult += sEncrypted;
	CString wsEncrypted(CA2W(sResult.c_str()));
	m_edEncrypted.SetWindowTextW(wsEncrypted);

}

void CMainDlg::DoDecrypt()
{
	CWaitCursor();
	CString wsEncrypted;
	auto iLen = m_edEncrypted.GetWindowTextW(wsEncrypted);
	if (!iLen) {
		m_edPassword.SetWindowTextW(L"");
		return;
	};
	std::string sEncrypted(CW2A(wsEncrypted.GetString()));
	if (sEncrypted.find(prefix) == std::string::npos) {
		m_edPassword.SetWindowTextW(L"");
		return;
	}
	std::string_view svEncrypted(sEncrypted);
	svEncrypted.remove_prefix(prefix.length());
	auto sDecrypted = decrypt(svEncrypted);
	CString wsDecrypted(CA2W(sDecrypted.c_str()));
	m_edPassword.SetWindowTextW(wsDecrypted);

}


void CMainDlg::ExpandFilePath(CSimpleString& filePath)
{
	/*int nLength;
	LPTSTR pszText;

	nLength = GetWindowTextLength();
	pszText = strText.GetBuffer(nLength + 1);
	nLength = GetWindowText(pszText, nLength + 1);
	strText.ReleaseBuffer(nLength);

	return nLength;*/

	CString sDst(filePath);
	auto lpszBuffer = sDst.GetBuffer(MAX_PATH);
	ZeroMemory(lpszBuffer, MAX_PATH);
	auto iLen = ExpandEnvironmentStrings(filePath.GetString(), lpszBuffer, MAX_PATH);
	sDst.ReleaseBuffer(MAX_PATH);
	filePath = sDst;
}


LRESULT CMainDlg::OnBnClickedButtonDecrypt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDecrypt();

	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonOpenFileDialog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString filePath;
	m_edFilePath.GetWindowTextW(filePath);
	ExpandFilePath(filePath);

	CFileDialog fd(true, _T("ini"), filePath, OFN_FILEMUSTEXIST,
		_T("Ini Files ( *.ini ) \0 *.ini\0All Files ( *.* ) \0 *.*\0\0"), this->m_hWnd);
	if (fd.DoModal() == IDOK)
	{
		auto unexpanded = std::make_unique<TCHAR[]>(MAX_PATH);
		PathUnExpandEnvStringsW(fd.m_szFileName, unexpanded.get(), MAX_PATH);
		m_edFilePath.SetWindowTextW(unexpanded.get());
	}
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonDecryptFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	CString filePath;
	m_edFilePath.GetWindowTextW(filePath);
	ExpandFilePath(filePath);

	if (!PathFileExists(filePath)) {
		MessageBox(_T("File not found"), 0, MB_ICONERROR);
		return 0;
	}

	SapShortCut oSapShortCut;
	oSapShortCut.load(filePath.GetString());
	auto encryptedData = oSapShortCut.getData();


	LoadLibrary(L"Riched20.dll");
	CListDlg oListDlg;
	std::vector<std::wstring> listData;
	std::wstring eq(L" = ");
	for (auto&& [key, value] : encryptedData) {
		CW2A sValue(value.c_str());
		std::string_view svEncrypted(sValue);
		svEncrypted.remove_prefix(prefix.length());
		auto sDecrypted = decrypt(svEncrypted);
		listData.push_back(std::wstring(key).append(eq).append(CA2W(sDecrypted.c_str())));
	}
	oListDlg.setData(listData);

	oListDlg.DoModal();

	return 0;

	
}
