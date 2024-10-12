
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>



std::wstring GetClipboardText();
bool SetClipboardText(const std::wstring &s);

void MakeTextFilenameCompatible(std::wstring &s);



int WINAPI WinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     szCmdLine,
	_In_     int       iCmdShow)
{
	// ignored parameters
	(void)hInstance;
	(void)hPrevInstance;
	(void)szCmdLine;
	(void)iCmdShow;


	std::wstring s = GetClipboardText();
	if (s.empty())
		return 1;

	MakeTextFilenameCompatible(s);

	if (!SetClipboardText(s))
		return 1;


	return 0;
}



std::wstring GetClipboardText()
{
	std::wstring result;

	if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(NULL))
	{
		HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);
		if (hClipboardData != NULL)
		{
			const wchar_t *szIn = (wchar_t*)GlobalLock(hClipboardData);
			if (szIn != nullptr)
				result = szIn;
			GlobalUnlock(hClipboardData);
		}

		if (result.empty())
			CloseClipboard();
	}


	return result;
}

bool SetClipboardText(const std::wstring &s)
{
	EmptyClipboard();

	bool result = false;

	const size_t lenOut    = s.length();
	const size_t sizeOut   = (lenOut + 1) * sizeof(wchar_t);
	HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, sizeOut);

	if (hClipboardData != 0)
	{
		wchar_t *const pchData = (wchar_t *)GlobalLock(hClipboardData);

		result = pchData != nullptr;

		if (result)
			memcpy(pchData, s.c_str(), sizeOut);

		GlobalUnlock(hClipboardData);

		if (result)
			SetClipboardData(CF_UNICODETEXT, hClipboardData);
		else
			GlobalFree(hClipboardData);
	}

	CloseClipboard();

	return result;
}



constexpr const wchar_t *FILENAME_FRIENDLY_ASCII[] =
{
	// control characters

	L"\x00",   L"\x01",   L"\x02",   L"\x03",   L"\x04",   L"\x05",   L"\x06",   L"\x07",
	L"\x08",   L"\x09",   L"\x0A",   L"\x0B",   L"\x0C",   L"\x0D",   L"\x0E",   L"\x0F",

	L"\x10",   L"\x11",   L"\x12",   L"\x13",   L"\x14",   L"\x15",   L"\x16",   L"\x17",
	L"\x18",   L"\x19",   L"\x1A",   L"\x1B",   L"\x1C",   L"\x1D",   L"\x1E",   L"\x1F",

	// regular characters

	L"\x20",   L"\x21",   L"''",     L"\x23",   L"\x24",   L"\x25",   L"\x26",   L"\x27",
	L"\x28",   L"\x29",   L"\uFF0A", L"\x2B",   L"\x2C",   L"\x2D",   L"\x2E",   L"\uFF0F",

	L"\x30",   L"\x31",   L"\x32",   L"\x33",   L"\x34",   L"\x35",   L"\x36",   L"\x37",
	L"\x38",   L"\x39",   L"\uFF1A", L"\x3B",   L"\uFF1C", L"\x3D",   L"\uFF1E", L"\uFF1F",

	L"\x40",   L"\x41",   L"\x42",   L"\x43",   L"\x44",   L"\x45",   L"\x46",   L"\x47",
	L"\x48",   L"\x49",   L"\x4A",   L"\x4B",   L"\x4C",   L"\x4D",   L"\x4E",   L"\x4F",

	L"\x50",   L"\x51",   L"\x52",   L"\x53",   L"\x54",   L"\x55",   L"\x56",   L"\x57",
	L"\x58",   L"\x59",   L"\x5A",   L"\x5B",   L"\uFF3C", L"\x5D",   L"\x5E",   L"\x5F",

	L"\x60",   L"\x61",   L"\x62",   L"\x63",   L"\x64",   L"\x65",   L"\x66",   L"\x67",
	L"\x68",   L"\x69",   L"\x6A",   L"\x6B",   L"\x6C",   L"\x6D",   L"\x6E",   L"\x6F",

	L"\x70",   L"\x71",   L"\x72",   L"\x73",   L"\x74",   L"\x75",   L"\x76",   L"\x77",
	L"\x78",   L"\x79",   L"\x7A",   L"\x7B",   L"\uFF5C", L"\x7D",   L"\x7E",   L"\x7F",
};

/*
	Replaced characters:

	U+0022 = "
	U+002A = *
	U+002F = /
	U+003A = :
	U+003C = <
	U+003E = >
	U+003F = ?
	U+005C = \
	U+007C = |
*/

void MakeTextFilenameCompatible(std::wstring &s)
{
	std::wstring sNew;
	sNew.reserve(s.length());

	for (size_t idx = 0; idx < s.length(); ++idx)
	{
		const wchar_t c = s[idx];

		// non-ASCII --> always allowed
		if (c & 0xFF80)
			sNew += c;

		else
			sNew += FILENAME_FRIENDLY_ASCII[c];
	}

	s = std::move(sNew);
}
