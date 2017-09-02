#pragma once

#include "CUIControl.h"

class CUIText : public CUIControl
{
public:
	CUIText()
		:
		CUIControl()
	{
		SetStatic(true);
	}

	CUIText(CUIMenu* menu)
		:
		CUIControl(menu)
	{
		SetStatic(true);
	}

	CUIText(CUIControl* control)
		:
		CUIControl(control)
	{
		SetStatic(true);
	}

	void OnRender() override
	{
		CUIControl::OnRender();
	}

	void OnUpdate() override
	{
		GetSprite(true).SetScale(0.0f, 0.0f);
	}
};