#include "PreciseTime.h"



void CPreciseTime::setupAddresses()
{
	addAddress(new CAddress("codefix", (BYTE*)"\xB8\x00\x00\x00\x00\xF7\xE7\x8B\x44\x24\x20\xC1\xEA\x03", "x????xxxxxxxxx", 0x005113B0));
	addAddress(new CAddress("datafix", (BYTE*)"\x25\x73\x25\x64\x3A\x25\x2E\x32\x64\x2E\x25\x2E\x32\x64", "xxxxxxxxxxxxxx", 0x01674528));
}

void CPreciseTime::initialize()
{
	CFeature::initialize();
	m_SavedData = new BYTE[14];
	//DWORD oldProtect;
	//VirtualProtect((void*)getAddress("codefix"), 14, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(m_SavedData, (void*)getAddress("codefix"), 14);
	//VirtualProtect((void*)getAddress("codefix"), 14, oldProtect, &oldProtect);
}

CPreciseTime::CPreciseTime()
{
	setButtonId(VK_INSERT);
	setupAddresses();
	initialize();
}


CPreciseTime::~CPreciseTime()
{
}

void CPreciseTime::enable()
{
	/*
	 * Delete rounding to 10
	 */
	CMemTools::fillWithNops(getAddress("codefix"), 7);
	BYTE fix1[2] = {0x8b, 0xd7}; // mov edx, edi
	CMemTools::changeCode(getAddress("codefix"), fix1, 2);

	CMemTools::fillWithNops(getAddress("codefix")+0xB, 3);

	/*
	 * Change %s%d:%.2d.%.2d to %s%d:%.2d.%.3d
	 */
	BYTE val = 0x33; // 3
	CMemTools::changeCode(getAddress("datafix")+0xC, &val, 1);
	CMemTools::changeCode(getAddress("datafix")+0x21, &val, 1);
	CFeature::enable();
}

void CPreciseTime::disable()
{
	CMemTools::changeCode(getAddress("codefix"), m_SavedData, 14);
	/*
	* Change %s%d:%.2d.%.3d to %s%d:%.2d.%.2d
	*/
	BYTE val = 0x32; // 2
	CMemTools::changeCode(getAddress("datafix") + 0xC, &val, 1);
	CMemTools::changeCode(getAddress("datafix") + 0x21, &val, 1);
	CFeature::disable();
}

