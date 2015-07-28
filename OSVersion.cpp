#include "stdafx.h"
#include "OSVersion.h"



BOOL COSVersion::IsWow64()
{
	BOOL bIsWow64 = FALSE;

	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

CString COSVersion::GetVersionString()
{
	edition = PRODUCT_UNDEFINED;
	ZeroMemory(&in, sizeof(in)); in.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX); GetVersionExW((OSVERSIONINFO*)&in);
	CString s,ss;
	HMODULE	hModule;

	BOOL(CALLBACK* pfnGetProductInfo)(DWORD dwOSMajorVersion, DWORD dwOSMinorVersion, DWORD dwSpMajorVersion, DWORD dwSpMinorVersion, PDWORD pdwReturnedProductType);
	hModule = ::LoadLibrary(_T("kernel32.dll"));
	(*(FARPROC*)&pfnGetProductInfo) = ::GetProcAddress(hModule, "GetProductInfo");
	switch (in.dwPlatformId) {
	case 1:
		switch (in.dwMinorVersion) {
		case 0:
			ss = _T("Windows 95"); break;
		case 10:
			ss = _T("Windows 98"); break;
		case 90:
			ss = _T("Windows Me"); break;
		}break;
	case 2:
		switch (in.dwMajorVersion) {
		case 3:
			ss = _T("Windows NT 3.51"); break;
		case 4:
			ss = _T("Windows NT 4.0"); break;
		case 5:
			if (in.dwMinorVersion == 0) {
				ss = _T("Windows 2000"); break;
			}
			if (in.dwMinorVersion == 1) {
				ss = _T("Windows XP");
				if ((in.wSuiteMask & VER_SUITE_PERSONAL) == VER_SUITE_PERSONAL) {
					ss += " Home Edition";
				}
				else {
					ss += " Professional Edition";
				}
				break;
			}
			if (in.dwMinorVersion == 2) {
				ss = _T("Windows Server 2003");
				if ((in.wSuiteMask & VER_SUITE_DATACENTER)
					== VER_SUITE_DATACENTER)
				{
					ss += " Datacenter Edition";
				}
				else if ((in.wSuiteMask & VER_SUITE_ENTERPRISE)
					== VER_SUITE_ENTERPRISE)
				{
					ss += " Enterprise Edition";
				}
				else if (in.wSuiteMask == VER_SUITE_BLADE)
				{
					ss += " Web Edition";
				}
				else
				{
					ss += " Standard Edition";
				}
				break;
			}
		case 6:
			if (pfnGetProductInfo(
				in.dwMajorVersion,
				in.dwMinorVersion,
				in.wServicePackMajor,
				in.wServicePackMinor,
				&edition)) {
				if (in.dwMinorVersion == 0) {
					if (in.wProductType == VER_NT_WORKSTATION)
					{
						ss = _T("Windows Vista");
						switch (edition)
						{
						case PRODUCT_ENTERPRISE:
						case PRODUCT_ENTERPRISE_N:
						case PRODUCT_ENTERPRISE_E:
						case PRODUCT_ENTERPRISE_SERVER_CORE:
						case PRODUCT_ENTERPRISE_SERVER_IA64:
							ss += " Enterprise Edition";
							break;
						case PRODUCT_ULTIMATE:
						case PRODUCT_ULTIMATE_N:
						case PRODUCT_ULTIMATE_E:
							ss += " Ultimate Edition";
							break;
						case PRODUCT_BUSINESS:
							ss += " Business Edition";
							break;
						case PRODUCT_HOME_PREMIUM:
						case PRODUCT_HOME_PREMIUM_N:
							ss += " Home Premium Edition";
							break;
						case 0x30:
						case 49:
							ss += " Professional Edition";
							break;
						case PRODUCT_HOME_BASIC:
							ss += " Home Basic Edition";
							break;
						case PRODUCT_STARTER:
							ss += " Starter Edition";
							break;
						default:
							ss += " Unknown Edition";
							break;
						}
					}
					else {
						ss = _T("Windows Server 2008");
						if ((in.wSuiteMask & VER_SUITE_DATACENTER)
							== VER_SUITE_DATACENTER)
						{
							ss += " Datacenter Edition";
						}
						else if ((in.wSuiteMask & VER_SUITE_ENTERPRISE)
							== VER_SUITE_ENTERPRISE)
						{
							ss += " Enterprise Edition";
						}
						else if (in.wSuiteMask == VER_SUITE_BLADE)
						{
							ss += " Web Edition";
						}
						else
						{
							ss += " Standard Edition";
						}
						break;
					}

					break;
				}
				else {
					if (in.dwMinorVersion == 1) {
						if (in.wProductType == VER_NT_WORKSTATION) {
							ss = _T("Windows 7");
							switch (edition)
							{
							case PRODUCT_ENTERPRISE:
							case PRODUCT_ENTERPRISE_N:
							case PRODUCT_ENTERPRISE_E:
							case PRODUCT_ENTERPRISE_SERVER_CORE:
							case PRODUCT_ENTERPRISE_SERVER_IA64:
								ss += " Enterprise Edition";
								break;
							case PRODUCT_ULTIMATE:
							case PRODUCT_ULTIMATE_N:
							case PRODUCT_ULTIMATE_E:
								ss += " Ultimate Edition";
								break;
							case PRODUCT_BUSINESS:
								ss += " Business Edition";
								break;
							case PRODUCT_HOME_PREMIUM:
							case PRODUCT_HOME_PREMIUM_N:
								ss += " Home Premium Edition";
								break;
							case 0x30:
							case 49:
								ss += " Professional Edition";
								break;
							case PRODUCT_HOME_BASIC:
								ss += " Home Basic Edition";
								break;
							case PRODUCT_STARTER:
								ss += " Starter Edition";
								break;
							default:
								ss += " Unknown Edition";
								break;
							}
						}
						else {
							ss = _T("Windows Server 2008 R2");
							if ((in.wSuiteMask & VER_SUITE_DATACENTER)
								== VER_SUITE_DATACENTER)
							{
								ss += " Datacenter Edition";
							}
							else if ((in.wSuiteMask & VER_SUITE_ENTERPRISE)
								== VER_SUITE_ENTERPRISE)
							{
								ss += " Enterprise Edition";
							}
							else if (in.wSuiteMask == VER_SUITE_BLADE)
							{
								ss += " Web Edition";
							}
							else
							{
								ss += " Standard Edition";
							}
							break;
						}
						break;
					}
					if (in.dwMinorVersion == 2) {
						if (in.wProductType == VER_NT_WORKSTATION) {
							ss = _T("Windows 8");
							if (edition == 0x48 || edition == 4 || edition == 0x54)
								ss += " Enterprise Edition";
							if (edition == 0x30 || edition == 0x31)
								ss += " Professional Edition";
							if (edition == 0x67)
								ss += " Pro with MC Edition";
							if (edition == 0x36 || edition == 0x65 || edition == 0x62)
								ss += " Home Edition";
							break;
						}
						else {
							ss = _T("Windows Server 2012");
							if ((in.wSuiteMask & VER_SUITE_DATACENTER)
								== VER_SUITE_DATACENTER)
							{
								ss += " Datacenter Edition";
							}
							else if ((in.wSuiteMask & VER_SUITE_ENTERPRISE)
								== VER_SUITE_ENTERPRISE)
							{
								ss += " Enterprise Edition";
							}
							else if (in.wSuiteMask == VER_SUITE_BLADE)
							{
								ss += " Web Edition";
							}
							else
							{
								ss += " Standard Edition";
							}
							break;
						}
						break;
					}
					if (in.dwMinorVersion == 3) {
						if (in.wProductType == VER_NT_WORKSTATION) {
							ss = _T("Windows 8.1");
							if (edition == 0x48 || edition == 4 || edition == 0x54)
								ss += " Enterprise Edition";
							if (edition == 0x30 || edition == 0x31)
								ss += " Professional Edition";
							if (edition == 0x67)
								ss += " Pro with MC Edition";
							if (edition == 0x36 || edition == 0x65 || edition == 0x62)
								ss += " Home Edition";
							break;
						}
						else {
							ss = _T("Windows Server 2012 R2");
							if ((in.wSuiteMask & VER_SUITE_DATACENTER)
								== VER_SUITE_DATACENTER)
							{
								ss += " Datacenter Edition";
							}
							else if ((in.wSuiteMask & VER_SUITE_ENTERPRISE)
								== VER_SUITE_ENTERPRISE)
							{
								ss += " Enterprise Edition";
							}
							else if (in.wSuiteMask == VER_SUITE_BLADE)
							{
								ss += " Web Edition";
							}
							else
							{
								ss += " Standard Edition";
							}
							break;
						}
						break;
					}
					if (in.dwMinorVersion == 4) {
						ss = _T("Windows 10 Preview"); break;
						switch (edition)
						{
						case PRODUCT_ENTERPRISE:
						case PRODUCT_ENTERPRISE_N:
						case PRODUCT_ENTERPRISE_E:
						case PRODUCT_ENTERPRISE_SERVER_CORE:
						case PRODUCT_ENTERPRISE_SERVER_IA64:
							ss += " Enterprise Edition";
							break;
						case PRODUCT_ULTIMATE:
						case PRODUCT_ULTIMATE_N:
						case PRODUCT_ULTIMATE_E:
							ss += " Ultimate Edition";
							break;
						case PRODUCT_BUSINESS:
							ss += " Business Edition";
							break;
						case PRODUCT_HOME_PREMIUM:
						case PRODUCT_HOME_PREMIUM_N:
							ss += " Home Premium Edition";
							break;
						case 0x30:
						case 49:
							ss += " Professional Edition";
							break;
						case PRODUCT_HOME_BASIC:
							ss += " Home Basic Edition";
							break;
						case PRODUCT_STARTER:
							ss += " Starter Edition";
							break;
						default:
							ss += " Unknown Edition";
							break;
						}
					}
				}
			}
		case 10:
			if (pfnGetProductInfo(
				in.dwMajorVersion,
				in.dwMinorVersion,
				in.wServicePackMajor,
				in.wServicePackMinor,
				&edition)) {
				if (in.dwMinorVersion == 0) {
					ss = _T("Windows 10");
					switch (edition)
					{
					case PRODUCT_ENTERPRISE:
					case PRODUCT_ENTERPRISE_N:
					case PRODUCT_ENTERPRISE_E:
					case PRODUCT_ENTERPRISE_SERVER_CORE:
					case PRODUCT_ENTERPRISE_SERVER_IA64:
						ss += " Enterprise Edition";
						break;
					case PRODUCT_ULTIMATE:
					case PRODUCT_ULTIMATE_N:
					case PRODUCT_ULTIMATE_E:
						ss += " Ultimate Edition";
						break;
					case PRODUCT_BUSINESS:
						ss += " Business Edition";
						break;
					case PRODUCT_HOME_PREMIUM:
					case PRODUCT_HOME_PREMIUM_N:
						ss += " Home Premium Edition";
						break;
					case 0x30:
					case 49:
						ss += " Professional Edition";
						break;
					case PRODUCT_HOME_BASIC:
						ss += " Home Basic Edition";
						break;
					case PRODUCT_STARTER:
						ss += " Starter Edition";
						break;
					default:
						ss += " Unknown Edition";
						break;
					}
				}
			}
		}
	}

	::FreeLibrary(hModule);
	s.Format(_T("%s %s %s"), ss, in.szCSDVersion, IsWow64() ? _T("64bit") : _T("32bit"));
	return s;
}

void COSVersion::GetVersionInfo(OSVERSIONINFOEX& in, DWORD& edition, BOOL& bit)
{
	HMODULE	hModule;
	edition = PRODUCT_UNDEFINED;
	ZeroMemory(&in, sizeof(in)); in.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX); GetVersionExW((OSVERSIONINFO*)&in);
	BOOL(CALLBACK* pfnGetProductInfo)(DWORD dwOSMajorVersion, DWORD dwOSMinorVersion, DWORD dwSpMajorVersion, DWORD dwSpMinorVersion, PDWORD pdwReturnedProductType);
	hModule = ::LoadLibrary(_T("kernel32.dll"));
	(*(FARPROC*)&pfnGetProductInfo) = ::GetProcAddress(hModule, "GetProductInfo");
	if (pfnGetProductInfo) {
		pfnGetProductInfo(
			in.dwMajorVersion,
			in.dwMinorVersion,
			in.wServicePackMajor,
			in.wServicePackMinor,
			&edition);
	}
}