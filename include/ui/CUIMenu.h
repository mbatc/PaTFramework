#pragma once

#include <Windows.h>

#include "CUIControl.h"
#include "../CD3DGraphics.h"

#define TESTCONTROL 69

class CUIMenu
{
public:
	CUIMenu();
	~CUIMenu();
	void Init(CD3DGraphics& gfx, UINT id, RECT menuRect, bool drawBackground = true, D3DXVECTOR4 bgColor = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));

	void ShowMenu(bool showWindow);
	void UpdateWindow(bool updateWindow);
	void DrawBackground(bool drawBackground);

	void CheckControls(int x, int y, bool mouseDown);
	void CheckTouchControls(int x, int y, bool isDown);
	
	void Update();
	void Render(CD3DGraphics& gfx);

	void ResizeMenu(RECT r,bool moveControls);

	bool AddControl(CUIControl* control, UINT ID);
	CUIControl* AddControl(UINT ID, RECT r, CUIControl* control = new CUIControl, char* active = NULL, char* inactive = NULL);

	void RemoveControl(CUIControl* control);
	void RemoveControl(UINT id);

	CUIControl* GetControl(UINT id);
	CUIControl* GetControl(int index);

	int GetControlCount() { return m_nControls; }

	UINT GetID() { return m_menuID; }

	RECT GetRect() { return m_menuRect; }
	void SetRect(RECT r) { m_menuRect = r; }

	bool isActive() { return m_bUpdateMenu || m_bShowMenu; }


	static bool MenuProc(UINT msg, UINT subMsg);

	void SetZOrder(int z) { m_z_order = z; }
private:
	bool DoesIDExist(UINT ID);

	bool m_bDrawbackground;
	D3DXVECTOR4 m_bgColor;
	RECT m_menuRect;
	RECT m_originalRect;

	CSprite m_bg;

	CUIControl** m_pControlList;
	int m_nControls;
	bool m_bShowMenu;
	bool m_bUpdateMenu;

	UINT m_menuID;

	UINT* m_controlIDList;

	int m_z_order;
};