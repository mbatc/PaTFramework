#include "CUITextIO.h"
#include "../helpers/Helper.h"
#include "../CD3DGraphics.h"
//#include "CSystem.h"

#include <iostream>

UINT CUITextIO::m_activeID = 0;

CUITextIO::CUITextIO()
	:
	CUIControl(),
	m_bAllow_ret(false),
	m_bHorizontalScroll(true),
	m_bWordWrap(false)
{}

CUITextIO::CUITextIO(CUIMenu * pMenu)
	:
	CUIControl(pMenu),
	m_bAllow_ret(false),
	m_bHorizontalScroll(true),
	m_bWordWrap(false)
{}

CUITextIO::CUITextIO(CUIControl * pParent)
	:
	CUIControl(pParent),
	m_bAllow_ret(false),
	m_bHorizontalScroll(true),
	m_bWordWrap(false)
{}

void CUITextIO::WordWrap(bool b)
{
	m_bWordWrap = b;
}

void CUITextIO::OnUpdate()
{
	//start = end;
	//end = clock();
	//
	//if (m_activeID != GetID())
	//{
	//	SetBorderColor(false);
	//}
	//
	//if (KEYBOARD::IsPressed(VK_RETURN) && !m_bAllow_ret && m_activeID == GetID())
	//{
	//	m_activeID = 0;
	//}
	//
	//if (m_activeID == GetID())
	//{
	//	GetInputBuffer();
	//
	//
	//	if (KEYBOARD::IsPressed(VK_LEFT))
	//	{
	//		cursor_index--;
	//	}
	//	if (KEYBOARD::IsPressed(VK_RIGHT))
	//	{
	//		cursor_index++;
	//	}
	//	SetBorderColor(true);
	//}
	//
	//m_timer += (float)(end-start)/ (float)CLOCKS_PER_SEC;
	//
	//if (m_timer >= 0.5f)
	//{
	//	m_indicator = !m_indicator;
	//	m_timer = 0;
	//}
}

void CUITextIO::OnRender()
{
	//RECT t;
	//if (m_clip)
	//{
	//	t = GetBounds(); 
	//	SetRenderBoundary(t);
	//
	//	if (m_bRenderBorder)
	//	{
	//		for (int i = 0; i < 4; i++)
	//		{
	//			m_bdrCorner[i].SetRectLock(t);
	//			m_bdrEdge[i].SetRectLock(t);
	//		}
	//	}
	//}
	//
	//m_temp_offset = m_text_offset_x;
	//
	//if (m_activeID == GetID())
	//{
	//	SYSTEM::RenderSprite(GetSprite(true), 8);
	//
	//	if(CSystem::g_textRenderer.GetWidthOfString(m_controlText) > GetWidth() && m_bHorizontalScroll)
	//		m_text_offset_x = RIGHT;
	//}
	//else
	//{
	//	m_indicator = false;
	//	SYSTEM::RenderSprite(GetSprite(false), 8);
	//}
	//
	//if (m_controlText)
	//{
	//	if (m_bWordWrap)
	//	{
	//		char* temp = 0;
	//
	//		STRING::Copy(temp, m_controlText);
	//
	//		for (int i = 0; i < strlen(m_controlText); i++)
	//		{
	//			CVector2 p = CSystem::g_textRenderer.GetCharacterLocation(m_controlText, i);
	//			if (p.x + 5 > GetWidth())
	//			{
	//				char* str = 0;
	//				char c = m_controlText[i];
	//				m_controlText[i] = '\0';
	//				STRING::Append(str, m_controlText, "\n");
	//				m_controlText[i] = c;
	//				STRING::Append(m_controlText, str, &m_controlText[i]);
	//				i++;
	//			}
	//		}
	//		if (m_indicator)
	//		{
	//			//Calculate the position of the cursor (right now its only at the end of the string)
	//			CVector2 pos;
	//			pos.x = 0;
	//			pos.y = 0;
	//			if (m_controlText)
	//				pos = CSystem::g_textRenderer.GetCharacterLocation(m_controlText, cursor_index);
	//			
	//			SYSTEM::SetTextColor(m_text_active_color);
	//			SYSTEM::RenderText("|", (int)(GetTextX() + pos.x), (int)(GetTextY() + pos.y),1.0f,&t);
	//		}
	//		RenderText();
	//		STRING::Copy(m_wrapped_text, m_controlText);
	//		STRING::Copy(m_controlText, temp);
	//
	//		MEMORY::SafeDeleteArr(temp);
	//	}
	//	else
	//	{
	//		if (m_indicator)
	//		{
	//			//Calculate the position of the cursor (right now its only at the end of the string)
	//			CVector2 pos;
	//			pos.x = 0;
	//			pos.y = 0;
	//			if (m_controlText)
	//				pos = CSystem::g_textRenderer.GetCharacterLocation(m_controlText, cursor_index);
	//			
	//			SYSTEM::SetTextColor(m_text_active_color);
	//			SYSTEM::RenderText("|", (int)(GetTextX() + pos.x), (int)(GetTextY() + pos.y), 1.0f, &t);
	//		}
	//		RenderText();
	//		STRING::Copy(m_wrapped_text, m_controlText);
	//	}
	//}
	//else
	//{
	//	MEMORY::SafeDeleteArr(m_wrapped_text);
	//	if (m_indicator)
	//	{
	//		//Calculate the position of the cursor (right now its only at the end of the string)
	//		CVector2 pos;
	//		pos.x = 0;
	//		pos.y = 0;
	//		SYSTEM::SetTextColor(m_text_active_color);
	//		SYSTEM::RenderText("|", (int)(GetTextX() + pos.x), (int)(GetTextY() + pos.y), 1.0f, &t);
	//	}
	//}
	//m_text_offset_x = m_temp_offset;
}

void CUITextIO::OnActive()
{
	//m_activeID = GetID();
	//cursor_index = -1;
	//for (int i = 0; i < strlen(m_wrapped_text); i++)
	//{
	//	CVector2 pos = CSystem::g_textRenderer.GetCharacterLocation(m_wrapped_text, i);
	//	pos.x += GetTextX();
	//	pos.y += GetTextY();
	//
	//	int left_x = pos.x - CSystem::g_textRenderer.GetActiveFont().
	//		GetCharacterSprite(m_wrapped_text[i]).GetWidth()/2;
	//	int right_x = pos.x + CSystem::g_textRenderer.GetActiveFont().
	//		GetCharacterSprite(m_wrapped_text[i+1]).GetWidth()/2;
	//	if (left_x <= MOUSE::GetMouseX() && right_x >= MOUSE::GetMouseX() &&
	//		pos.y <= MOUSE::GetMouseY() &&
	//		pos.y + CSystem::g_textRenderer.GetActiveFont().GetFontHeight() >= MOUSE::GetMouseY())
	//	{
	//		cursor_index = i;
	//	}
	//}
	//
	//if (cursor_index < 0)
	//	cursor_index = strlen(m_wrapped_text);
}

void CUITextIO::OnClickAway()
{
	if (m_activeID == GetID())
	{
		m_activeID = 0;
		SetBorderColor(false);
	}
}

void CUITextIO::GetInputBuffer()
{
	//const char* ib = KEYBOARD::GetInBuffer();
	//
	//if (!ib)
	//	return;
	//
	//char temp[2] = { '\0','\0' };
	//char* text = 0;
	//
	//if (GetText())
	//	STRING::Copy(text, GetText());
	//else
	//	STRING::Copy(text, "");
	//
	//int ib_len = strlen(ib) + 1;
	//
	//for (int i = 0; i < ib_len; i++)
	//{
	//	temp[0] = ib[i];
	//	if (temp[0] == '\0')
	//		continue;
	//
	//	switch (ib[i])
	//	{
	//	case VK_RETURN:
	//		temp[0] = '\n';
	//	default:
	//		//Get the index of the original string (not the string wrapped to fit the control)
	//		int split_i = GetNonWrappedIndex(cursor_index);
	//
	//		if(m_wrapped_text && m_controlText)
	//			if (cursor_index == strlen(m_wrapped_text))
	//				split_i = strlen(m_controlText);
	//
	//		//Special cases for backspace and delete keys as we want to remove characters
	//		if (temp[0] == VK_BACK)
	//		{
	//			//Set temp[0] = '\0' so that a character is removed and decrease the cursor index
	//			//Also decrease the split_i so that the character behind the cursor is removed
	//			temp[0] = '\0';
	//			split_i--;
	//			cursor_index--;
	//
	//			if (!m_controlText)
	//				continue;
	//		}
	//
	//		/*
	//		if (temp[0] == VK_DELETE)
	//		{
	//			//Set temp[0] = '\0' but leave the split index the same so the that character in front
	//			//of the cursor is removed
	//			temp[0] = '\0';
	//			if (!m_controlText)
	//				continue;
	//		}
	//		*/
	//
	//		if (!m_controlText)
	//		{
	//			STRING::Copy(text, temp);
	//			cursor_index++;
	//			continue;
	//		}
	//		//If split is < 0 then we are attempting to remove a character when the string is empty,
	//		//so we should exit as this is not possible
	//		if (split_i < 0)
	//			break;
	//		//Get the right part of the string (after the cursor)
	//		char* second = 0;
	//		char c = 0;
	//		int str_len = 0;
	//		if (m_controlText)
	//			str_len = strlen(m_controlText);
	//		if (split_i < str_len)
	//		{
	//			second = &text[split_i];
	//			c = text[split_i];
	//			text[split_i] = '\0';
	//		}
	//
	//		char* first = 0;
	//		//Get the left part of the string (before the cursor)
	//		if (text)
	//			STRING::Copy(first, text);
	//		else
	//			STRING::Copy(first, "");
	//
	//		//because a character has been added, we are not removing one
	//		if (temp[0] != '\0')
	//		{
	//			STRING::Append(first, first, temp);
	//			//Increase the cursors index to infront of the character just added
	//			cursor_index++;
	//		}
	//		else
	//		{
	//			if(second)
	//				second = &second[1];
	//		}
	//
	//		text[split_i] = c;
	//		//Add the two halves of the string together
	//
	//		if (second)
	//			STRING::Append(text, first, second);
	//		else
	//			STRING::Copy(text, first);
	//
	//		break;
	//	}
	//}
	//
	//if (text)
	//	SetText(text);
	//else
	//	SetText("");
	//
	////Delete the copy of the controls text as it is no longer needed
	//MEMORY::SafeDeleteArr(text);
}

int CUITextIO::GetNonWrappedIndex(int index)
{
	if (!m_wrapped_text)
		return 0;
	for (int i = 0, oi = 0; i < strlen(m_wrapped_text); i++)
	{
		if (i == index)
			return oi;

		if (m_wrapped_text[i] == m_controlText[oi])
		{
			oi++;
		}
	}

	return 0;
}

int CUITextIO::GetWrappedIndex(int index)
{
	if (!m_wrapped_text)
		return 0;

	for (int i = 0, oi = 0; i < strlen(m_wrapped_text); i++)
	{
		if (oi == index)
			return i;

		if (m_wrapped_text[i] == m_controlText[oi])
		{
			oi++;
		}
	}
	return 0;
}

void CUITextIO::OnInit()
{
	SetInactiveSprite(CD3DGraphics::DEFAULTSPRITE);
	SetActiveSprite(CD3DGraphics::DEFAULTSPRITE);

	GetSprite(true).SetColor(0.4f, 0.4f, 0.4f, 1.0f);
	GetSprite(false).SetColor(0.1f, 0.1f, 0.1f, 1.0f);

	m_timer = 0;
	//end = clock();
}
