#pragma once

#include <Windows.h>

#include "../CSprite.h"
#include "../CD3DGraphics.h"

#define UI_LIST_ITEM 0x0001u

enum TEXTOFFSET
{
	LEFT,
	RIGHT,
	CENTER,
	TOP,
	BOTTOM
};

class CUIMenu;

class CUIControl
{
	friend CUIMenu;
public:
	CUIControl();
	CUIControl(CUIMenu* pMenu);
	CUIControl(CUIControl* pParent);
	~CUIControl();

	CUIControl* GetParent() { return m_pParent; }
	CUIMenu* GetMenu() { return m_pMenu; }

	const int GetGlobalX() { return m_translatedPosX; }
	const int GetGlobalY() { return m_translatedPosY; }

	RECT GetRect() { return m_activeRect; }
	RECT GetBounds() { return m_bounds; }

	char* GetText() { return m_controlText; }

	CSprite& GetSprite(bool active) { return active ? m_spriteStateActive : m_spriteStateInactive; }

	virtual void InitControl(RECT r, UINT id,char* active = NULL, char* inactive = NULL);

	UINT GetID();

	virtual void CheckIfActive(int mouseX, int mouseY, bool buttonDown);
	virtual void CheckTouchActive(int x, int y, bool isDown);

	void Update();
	void Render(CD3DGraphics& gfx);
	void Clip(bool b) { m_clip = b; }
	
	void Scale(float x, float y);
	void SetPosition(RECT r);
	void SetText(char* text);
	void SetTextColor(D3DXVECTOR4 color, bool active = true, bool inactive = true);

	void SetMenu(CUIMenu* pMenu);
	void SetParent(CUIControl* pParent);

	void SetTextOffsetX(TEXTOFFSET offset) { m_text_offset_x = offset; }
	void SetTextOffsetY(TEXTOFFSET offset) { m_text_offset_y = offset; }

	/////////////////////////////////////////////////////////////////////
	//CONTROL BORDER FUNCTIONS
	//*******************************************************************

	void RenderBorder(bool b);
	void RenderBorder(bool left, bool right, bool top, bool bottom);
	void SetBorderInner(bool b);
	void SetBorderWidth(int w) { m_borderWidth = w; }
	int GetBorderWidth() { return m_borderWidth; }

	void SetBorderColour(float r, float g, float b, float a);
	void SetBorderColour(float r, float g, float b, float a, bool active);
	void SetBorderColour(D3DXVECTOR4 color);
	void SetBorderColour(D3DXVECTOR4 color, bool active);
	void SetBorderColour(D3DXVECTOR4 active, D3DXVECTOR4 inactive);

	void SetBorderSprite();
	void SetBorderSprite(CSprite* corner, CSprite* edge);
	void SetBorderSprite(char* corner, char* edge);
	//*******************************************************************

	void SetActiveSprite(char* filename);
	void SetActiveSprite(CSprite sprite) { m_spriteStateActive = sprite; }
	void SetInactiveSprite(char* filename);
	void SetInactiveSprite(CSprite sprite) { m_spriteStateInactive = sprite; }

	void SetID(UINT ID);

	int GetHeight() { return m_activeRect.bottom - m_activeRect.top; }
	int GetWidth() { return m_activeRect.right - m_activeRect.left; }

	void RenderText();
	void Resize(RECT r) { m_activeRect = r; OnResize(); }

	void ShowControl(bool b);

	void Scroll(bool b) { m_bDoScroll = b; }
	void RightClickActive(bool b) { m_bRightClickActive = b; }
	void SetScrollSpeed(int speed) { m_scroll_speed = speed; };

	void SetScrollMaxX(int max) { m_scroll_x_max = max; }
	void SetScrollMaxY(int max) { m_scroll_y_max = max; }

	void ShowVerticalScrollBar(bool b) { m_bShowScrollVer = b; }
	void ShowHorizontalScrollBar(bool b) { m_bShowScrollHor = b; }
protected:
	void SetBorderColor(bool active);
	void SetStatic(bool b) { m_bStatic = b; }
	virtual void doScroll();

	virtual void OnPressed();
	virtual void OnActive();
	virtual void OnHover();
	virtual void OnRelease();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnClickAway();
	virtual void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);
	virtual void OnResize();

	virtual void OnPressedTouch() { OnPressed(); }
	virtual void OnReleaseTouch() { OnRelease(); }

	void RenderVerticalScrollBar();
	void RenderHorizontalScrollBar();

	void RenderBorder();
	void UpdateBounds();

	D3DXVECTOR4 GetBorderColor(bool active);

	int GetTextX();
	int GetTextY();

	CUIMenu* m_pMenu;
	CUIControl* m_pParent;

	UINT m_buttonActiveID;
	char* m_controlText;

	void SetRenderBoundary(RECT r);

	RECT m_activeRect;
	RECT m_originalRect;
	RECT m_bounds;

	int m_xPos, m_yPos, m_translatedPosX, m_translatedPosY;
	bool m_bIsActive;
	bool m_bStatic;

	bool m_bRenderBorder;
	bool m_bInnerBorder;

	bool m_bShowScrollHor;
	bool m_bShowScrollVer;

	bool m_bDoScroll;

	bool m_bRightClickActive;

	bool m_clip;

	bool m_bShow;

	int m_borderWidth;

	CSprite m_bdrCorner[4];
	CSprite m_bdrEdge[4];
	bool	m_bdrRender[4];

	CSprite m_spriteStateActive;
	CSprite m_spriteStateInactive;

	TEXTOFFSET m_text_offset_x;
	TEXTOFFSET m_text_offset_y;

	int m_scroll_x;
	int m_scroll_y;

	int m_scroll_y_max;
	int m_scroll_x_max;

	int m_scroll_speed;

	D3DXVECTOR4 m_text_active_color;
	D3DXVECTOR4 m_text_inactive_color;

	D3DXVECTOR4 m_border_active_color;
	D3DXVECTOR4 m_border_inactive_color;
};