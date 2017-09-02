#pragma once

#include "CUIControl.h"

class CUITextIO : public CUIControl
{
public:
	CUITextIO();
	CUITextIO(CUIMenu* pMenu);
	CUITextIO(CUIControl* pParent);
	
	bool IsActive() { return (m_activeID == GetID()); }

	void AllowReturn(bool b) { m_bAllow_ret = b; }
	void DoHorizontalScroll(bool b) { m_bHorizontalScroll = b; }
	void WordWrap(bool b);
	
private:
	void OnUpdate();
	void OnRender();
	void OnActive();
	void OnInit();
	void OnClickAway();

	void GetInputBuffer();

	int GetNonWrappedIndex(int index);
	int GetWrappedIndex(int index);

	static UINT m_activeID;
	
	int start;
	int end;

	float m_timer;
	bool m_indicator;
	bool m_bAllow_ret;
	bool m_bWordWrap;	
	bool m_bHorizontalScroll;
	int cursor_index;

	char* m_wrapped_text;

	TEXTOFFSET m_temp_offset;
};