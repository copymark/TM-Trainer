#include "Page.h"


CPage::CPage(void)
	: m_iID(0)
	, m_iCurrentSelection(0)
{
	this->m_iID = g_pLCD->AddNewPage()-1;

	g_pLCD->ModifyControlsOnPage(this->m_iID);

	this->m_hNextArrow = g_pLCD->AddIcon(g_hIcoRight, 16, 16);
	g_pLCD->SetOrigin(this->m_hNextArrow, 142, 35);

	this->m_hPrevArrow = g_pLCD->AddIcon(g_hIcoLeft, 16, 16);
	g_pLCD->SetOrigin(this->m_hPrevArrow, 98, 35);

	this->m_hDownArrow = g_pLCD->AddIcon(g_hIcoDown, 16, 16);
	g_pLCD->SetOrigin(this->m_hDownArrow, 56, 35);

	this->m_hSelectArrow = g_pLCD->AddIcon(g_hIcoRight, 16, 16);
	g_pLCD->SetOrigin(this->m_hSelectArrow, 0, 2);

	this->m_hCircleButton = g_pLCD->AddIcon(g_hIcoCircle, 16, 16);
	g_pLCD->SetOrigin(this->m_hCircleButton, 15, 35);
}


CPage::~CPage(void)
{
}

// Leitet den Tastendruck weiter
void CPage::DoAction(void)
{
	this->m_vecSettings[this->m_iCurrentSelection]->DoAction();
}


// Fügt ein Setting hinzu
bool CPage::AddSetting(HWND phHwndDlg, int iID, char* pszName)
{
	int iFilledPlaces = this->m_vecSettings.size();

	if (iFilledPlaces > 2)
	{
		return false;
	}
	else
	{
		g_pLCD->ModifyControlsOnPage(this->m_iID);

		this->m_vecSettings.push_back(new CSetting(pszName, iID, iFilledPlaces, phHwndDlg));
		return true;
	}
}


// wird aufgerufen wenn die Settings Selektion geändert werden soll
void CPage::SelectionChange(void)
{
	g_pLCD->ModifyControlsOnPage(this->m_iID);

	this->m_iCurrentSelection = (this->m_iCurrentSelection+1) % this->m_vecSettings.size();

	g_pLCD->SetOrigin(this->m_hSelectArrow, 0, 2 + this->m_iCurrentSelection * g_iLineHeight);
}
