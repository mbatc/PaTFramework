#pragma once
#include "CUIControl.h"

class CUIContainer : public CUIControl
{
public:
	CUIContainer();
	CUIContainer(CUIControl* parent);
	CUIContainer(CUIMenu* menu);

	void AddControl(CUIControl* control, float l, float r, float t, float b);
	void ClearChildren();

private:
	void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);
	void OnRender();
	void OnActive();
	void OnUpdate();

	void doScroll();

	CUIControl m_bg;

	int				m_nChild;
	CUIControl**	m_ppChild;
	RECT*			m_pChildRect;
};