#pragma once
#include <string>
#include <map>


using _RESTYPE  = std::map<std::wstring, std::wstring>;

class SapShortCut
{
public:
	void load(std::wstring_view fileName) ;
	const _RESTYPE& getData() const;
	
private:
	_RESTYPE mData;
};

