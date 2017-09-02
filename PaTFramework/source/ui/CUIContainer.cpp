#include "CUIContainer.h"
//#include "CSystem.h"
#include "CUI.h"
#include "../helpers/Helper.h"

CUIContainer::CUIContainer()
	:
	CUIControl(),
	m_ppChild(0),
	m_nChild(0)
{}

CUIContainer::CUIContainer(CUIControl * parent)
	:
	CUIControl(parent),
	m_ppChild(0),
	m_nChild(0)
{}

CUIContainer::CUIContainer(CUIMenu * menu)
	:
	CUIControl(menu),
	m_ppChild(0),
	m_nChild(0)
{}

void CUIContainer::AddControl(CUIControl * control, float l, float r, float t, float b)
{
	if (!control)
		return;

	RECT w;
	w.right = r;
	w.top = t;
	w.bottom = b;
	w.left = l;

	control->SetMenu(NULL);
	control->SetParent(this);
	control->SetPosition(w);

	MEMORY::LIST::AddToListArr(m_ppChild, control, m_nChild); m_nChild--;
	MEMORY::LIST::AddToList(m_pChildRect, &w, m_nChild);

	int height = 0;
	for (int i = 0; i < m_nChild; i++)
	{
		if (m_ppChild[i]->GetRect().bottom > GetHeight())
		{
			if (m_ppChild[i]->GetRect().bottom - GetHeight() > height)
				height = m_ppChild[i]->GetRect().bottom - GetHeight();
		}
	}

	SetScrollMaxY(height);
}

void CUIContainer::ClearChildren()
{
	if (!m_pMenu)
		return;

	for (int i = 0; i < m_nChild; i++)
	{
		m_pMenu->RemoveControl(m_ppChild[i]);
	}

	m_nChild = 0;
	MEMORY::SafeDeleteArr(m_ppChild);
}

void CUIContainer::OnInit(RECT r, UINT id, char * active, char * inactive)
{
	CUIControl::OnInit(r, id, active, inactive);
	int height = r.bottom - r.top;
	r.left = -4;
	r.right += 4;

	r.top = -15;
	r.bottom = height;

	m_bg.SetParent(this);
	m_bg.Clip(true);
	m_bg.InitControl(r, id, active, inactive);
	m_bg.SetBorderInner(true);
	m_bg.RenderBorder(true,true,false,true);
	m_bg.GetSprite(true).SetColor(0.1, 0.1f, 0.1f, 1.0f);
	m_bg.GetSprite(false).SetColor(0.1f, 0.1f, 0.1f, 1.0f);


	RenderBorder(true);
	SetBorderInner(false);
	SetBorderColour(0.8f, 0.8f, 0.8f, 1.0f);
	SetBorderWidth(2);

	SetScrollSpeed(5);
}

void CUIContainer::OnRender()
{
	m_bg.SetBorderWidth(m_borderWidth);
	m_bg.SetBorderColour(m_border_active_color);
	//m_bg.Render(CSystem::m_gfx);

	int text_x;
	int text_y;
	int text_width = 0;
	/*if(m_controlText)
		text_width = CSystem::g_textRenderer.GetWidthOfString(m_controlText);
	*/
	text_x = m_bg.GetGlobalX();
	text_y = m_bg.GetGlobalY() + m_borderWidth;
	
	//CSprite spr = CSystem::m_gfx.DEFAULTSPRITE;
	//
	//spr.Resize(m_bg.GetWidth() - text_width, m_borderWidth);
	//spr.SetColor(m_border_active_color);

	//Render Container Name and top border
	//SYSTEM::SetTextColor(m_text_active_color);
	//SYSTEM::RenderText(m_controlText, text_x, text_y);
	//SYSTEM::RenderSprite(spr,9);

	//for (int i = 0; i < m_nChild; i++)
	//{
	//	m_ppChild[i]->Render(CSystem::m_gfx);
	//}
}

void CUIContainer::OnActive()
{

}

void CUIContainer::OnUpdate()
{
	m_bg.Update();
}

void CUIContainer::doScroll()
{
	CUIControl::doScroll();

	//Reposition all the child controls depending on the x and y scroll
	for (int i = 0; i < m_nChild; i++)
	{
		RECT w = m_pChildRect[i];
		w.top -= m_scroll_y;
		w.bottom -= m_scroll_y;
		w.left -= m_scroll_x;
		w.right -= m_scroll_x;

		m_ppChild[i]->SetPosition(w);
	}
}
