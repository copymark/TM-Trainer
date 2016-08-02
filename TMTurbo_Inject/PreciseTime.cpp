#include "PreciseTime.h"
#include "CodeModify.h"
#include "CodeNop.h"



void CPreciseTime::setupAddresses()
{
	addAddress(new CAddress("codefix", (BYTE*)"\xB8\x00\x00\x00\x00\xF7\xE7\x8B\x44\x24\x20\xC1\xEA\x03", "x????xxxxxxxxx", 0x005113B0));
	addAddress(new CAddress("datafix", (BYTE*)"\x25\x73\x25\x64\x3A\x25\x2E\x32\x64\x2E\x25\x2E\x32\x64", "xxxxxxxxxxxxxx", 0x01674528));
}

void CPreciseTime::initialize()
{
	CFeature::initialize();
	
	BYTE fix1[2] = {0x8b, 0xd7}; // mov edx, edi
	addCodeChange(new CCodeModify(getAddress("codefix"), fix1, sizeof(fix1)));
	addCodeChange(new CCodeNop(getAddress("codefix") + 0x2, 5));
	addCodeChange(new CCodeNop(getAddress("codefix") + 0xB, 3));

	BYTE asciiThree = 0x33;
	addCodeChange(new CCodeModify(getAddress("datafix") + 0xC, &asciiThree, sizeof(asciiThree)));
	addCodeChange(new CCodeModify(getAddress("datafix") + 0x21, &asciiThree, sizeof(asciiThree)));
}

CPreciseTime::CPreciseTime()
{
	setButtonId(VK_NUMPAD4);
	initialize();
}


CPreciseTime::~CPreciseTime()
{
}

