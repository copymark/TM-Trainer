#include "NoWallFriction.h"
#include "CodeNop.h"



CNoWallFriction::CNoWallFriction()
{
	setButtonId(VK_NUMPAD6);
	initialize();
}


CNoWallFriction::~CNoWallFriction()
{
}

void CNoWallFriction::setupAddresses()
{
	addAddress(new CAddress("codefix", (BYTE*)"\xC7\x86\x00\x00\x00\x00\x01\x00\x00\x00\x83\xBC\x31", "xx????xxxxxxx", 0x006BDFEC));
}

void CNoWallFriction::initialize()
{
	CFeature::initialize();

	addCodeChange(new CCodeNop(getAddress("codefix"), 10));
}
