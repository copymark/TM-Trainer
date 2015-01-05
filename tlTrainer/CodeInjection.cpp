#include "CodeInjection.h"

CCodeInjection::CCodeInjection(void)
{
}

CCodeInjection::CCodeInjection(cHack *pHack, UINT_PTR uiAddress, BYTE *pInject, SIZE_T nInjectSize, SIZE_T nOrigJmpSize)
{
	this->Initialize(pHack, uiAddress, pInject, nInjectSize, nOrigJmpSize);
}


CCodeInjection::~CCodeInjection(void)
{
	delete[] this->m_pbInject;
	delete[] this->m_pbOriginal;
}

void CCodeInjection::Initialize(cHack *pHack, UINT_PTR uiAddress, BYTE *pInject, SIZE_T nInjectSize, SIZE_T nOrigJmpSize)
{
	// Würde falschen Originalwert auslesen
	if (this->IsEnabled())
		return;

	this->SetHack(pHack);
	this->m_uiAddress = uiAddress;
	this->m_nInjectSize = nInjectSize;
	this->m_nOrigJmpSize = nOrigJmpSize;

	this->m_pbInject = new BYTE[this->m_nInjectSize];
	memcpy(m_pbInject, pInject, this->m_nInjectSize);

	// Speicher allozieren und dann Original auslesen
	this->m_pbOriginal = new BYTE[this->m_nOrigJmpSize];
	this->GetHack()->ReadAddress(this->m_uiAddress, this->m_pbOriginal, this->m_nOrigJmpSize);

	// Initialized
	this->setIntializedStatus(true);
}

void CCodeInjection::EnableFeature(void)
{
	// Platz allozieren
	this->m_uiBase = (UINT_PTR)VirtualAllocEx(this->GetHack()->m_hGameHandle, NULL, this->m_nInjectSize, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	// Jump zum neuen Code erstellen
	BYTE *pbJump = new BYTE[this->m_nOrigJmpSize];
	for (unsigned int i = 0; i < this->m_nOrigJmpSize; i++)
	{
		pbJump[i] = 0x90;
	}
	this->GetJmpOpCode(pbJump, this->m_uiAddress, this->m_uiBase);

	// Backjump erstellen
	this->GetJmpOpCode(this->m_pbInject + this->m_nInjectSize - 5, this->m_uiBase + this->m_nInjectSize - 5, this->m_uiAddress + this->m_nOrigJmpSize);

	// Neuen Code in Programm schreiben
	this->GetHack()->WriteAddress(this->m_uiBase, this->m_pbInject, this->m_nInjectSize);
	this->GetHack()->WriteAddress(this->m_uiAddress, pbJump, this->m_nOrigJmpSize);
}

void CCodeInjection::DisableFeature(void)
{
	// Original wiederherstellen
	this->GetHack()->WriteAddress(this->m_uiAddress, this->m_pbOriginal, this->m_nOrigJmpSize);

	// Inject löschen
	VirtualFreeEx(this->GetHack()->m_hGameHandle, (LPVOID)this->m_uiBase, NULL, MEM_RELEASE);

	// Reset
	this->m_uiBase = 0;
}


UINT_PTR CCodeInjection::GetBaseAddress(void)
{
	return this->m_uiBase;
}

void CCodeInjection::GetJmpOpCode(BYTE OpCodes[], UINT_PTR StartAddress, UINT_PTR EndAddress)
{
	INT_PTR JumpLength = (EndAddress - StartAddress - 5);
	OpCodes[0] = 0xE9;
	OpCodes[1] = (BYTE)((JumpLength >> 0) & 0xFF);
	OpCodes[2] = (BYTE)((JumpLength >> 8) & 0xFF);
	OpCodes[3] = (BYTE)((JumpLength >> 16) & 0xFF);
	OpCodes[4] = (BYTE)((JumpLength >> 24) & 0xFF);
}

void CCodeInjection::UpdateCode(BYTE *pbUpdatedCode)
{
	delete this->m_pbInject;
	this->m_pbInject = new BYTE[this->m_nInjectSize];
	memcpy(this->m_pbInject, pbUpdatedCode, this->m_nInjectSize);
}