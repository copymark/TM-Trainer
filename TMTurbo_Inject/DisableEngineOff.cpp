#include "DisableEngineOff.h"
#include "CodeModify.h"



CDisableEngineOff::CDisableEngineOff()
{
	setButtonId(VK_NUMPAD5);
	initialize();
}


CDisableEngineOff::~CDisableEngineOff()
{
}

void CDisableEngineOff::setupAddresses()
{
	addAddress(new CAddress("codefix", (BYTE*)"\x74\x33\x89\x86\x00\x00\x00\x00\xF7\xD8", "xxxx????xx", 0x006BFA9F));
}

void CDisableEngineOff::initialize()
{
	CFeature::initialize();

	BYTE shortJmpOpcode = 0xEB;
	addCodeChange(new CCodeModify(getAddress("codefix"), &shortJmpOpcode, sizeof(shortJmpOpcode)));
}
