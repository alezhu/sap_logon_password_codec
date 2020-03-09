#include "SapShortCut.h"

#include <memory>
#include <windows.h>

#ifdef FILESYSTEM_EXPERIMENTAL
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif


//void replaceAll(std::string& str, const std::string_view& from, const std::string_view& to) {
//	if (from.empty())
//		return;
//	size_t start_pos = 0;
//	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
//		str.replace(start_pos, from.length(), to);
//		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
//	}
//}

using _MAP = std::map<std::wstring, std::wstring>;

void FillSectionMap(const std::wstring_view fileName, const std::wstring_view section, _MAP& sectionMap)
{
	sectionMap.clear();
    auto iBufSize = fs::file_size(fileName)  / 3;
	if (!iBufSize) return;
	do
	{
		auto buffer = std::make_unique<wchar_t[]>(iBufSize);
		auto pBuffer = buffer.get();
		auto iReaded = GetPrivateProfileSection(section.data(), pBuffer, iBufSize, fileName.data());
		if (iReaded < iBufSize - 2)
		{
			while (*pBuffer)
			{
				auto pEq = wcschr(pBuffer, L'=');
				if (nullptr == pEq) {
					std::wstring key(pBuffer);
					sectionMap[key] = std::wstring();
					pBuffer += key.length() + 1;
					continue;
				}
				auto pEnd = wcschr(pEq, 0);
				std::wstring key(pBuffer, pEq - pBuffer );
				std::wstring value(pEq + 1, pEnd - pEq - 1);
				sectionMap[key] = value;
				pBuffer = pEnd + 1;

			}
			break;
		}
		iBufSize *= 2;
	} while (true);
}



void SapShortCut::load(std::wstring_view fileName)
{
	mData.clear();
	_MAP labelMap;
	FillSectionMap(fileName, L"Label", labelMap);
	_MAP commandMap;
	FillSectionMap(fileName, L"Command", commandMap);

	constexpr auto ARG = L"-pwenc=\"PW_";
#if __cplusplus > 201402
    constexpr auto iArgLen = std::char_traits<wchar_t>::length(ARG);
#else
    auto iArgLen = std::char_traits<wchar_t>::length(ARG);
#endif


	for (auto&& [key, title] : labelMap)
	{
		std::wstring_view command(commandMap[key]);
		auto iOffset = command.find(ARG);
		if (iOffset ==std::string_view::npos) continue;
		iOffset += iArgLen - 3;
		auto iEnd = command.find('"', iOffset);
		auto svPassword = command.substr(iOffset, iEnd - iOffset );
		mData[title] =  std::wstring(svPassword);
	}


}

const _RESTYPE& SapShortCut::getData() const
{
	return mData;
}

