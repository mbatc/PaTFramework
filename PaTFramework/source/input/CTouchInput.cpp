#include "CTouchInput.h"
#include "../helpers/MemoryHelper.h"

#include <iostream>
using namespace std;

CTouchInputState::CTouchInputState()
	:
	m_pInputs(0),
	m_nInputs(0)
{}

void CTouchInputState::OnTouch(WPARAM wParam, LPARAM lParam)
{
	UINT cInputs = LOWORD(wParam);
	PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];

	if (pInputs)
	{
		if (GetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT)))
		{
			if (!CloseTouchInputHandle((HTOUCHINPUT)lParam))
			{
				cout << "CTouchInputState: Failed to close Touch Input Handle...\n";
			}
		}
		else
		{
			cout << "CTouchInputState: Could not retrieve Touch Input Info...\n";
		}
	}
	else
	{
		cout << "CTouchInputState: Unable to allocate Touch Input memory...\n";
		cout << "\t\t-Possibly OUT OF MEMORY\n";
	}

	for (int i = 0; i < cInputs; i++)
	{
		STOUCHINFO* touch = GetInput(pInputs[i].dwID);
		if (!touch)
		{
			STOUCHINFO n;
			n.ID = pInputs[i].dwID;
			n.touch_order = m_nSequenceTouchCount;
			n.pressed = false;
			MEMORY::LIST::AddToList(m_pInputs, &n, m_nInputs);
			touch = GetInput(pInputs[i].dwID);
			m_nSequenceTouchCount++;
			if (!touch)
			{
				cout << "CTouchInputState: Unable to add STOUCHINFO to state...\n";
			}
			
		}
		float x = pInputs[i].x;
		float y = pInputs[i].y;

		if (pInputs[i].dwFlags & TOUCHEVENTF_DOWN)
		{
			cout << "Touch Pressed\n";
			touch->x = x;
			touch->y = y;
			touch->prev_x = x;
			touch->prev_y = y;
			touch->pressed = true;
		}
		if (pInputs[i].dwFlags & TOUCHEVENTF_UP)
		{
			touch->released = true;
			cout << "Touch Released\n";
		}
		if (pInputs[i].dwFlags & TOUCHEVENTF_MOVE)
		{
			m_pInputs[i].x = x;
			m_pInputs[i].y = y;
		}
	}
}

void CTouchInputState::Update()
{
	if (!m_pInputs)
		m_nInputs = 0;

	if (!m_nInputs)
		m_nSequenceTouchCount = 0;

	for (int i = 0; i < m_nInputs; i++)
	{
		if (!m_pInputs)
			break;
		m_pInputs[i].prev_x = m_pInputs[i].x;
		m_pInputs[i].prev_y = m_pInputs[i].y;
		m_pInputs[i].pressed = false;

		if (m_pInputs[i].released)
		{
			MEMORY::LIST::RemoveItem(m_pInputs, m_nInputs, i);
			i--;
		}
	}
}

STOUCHINFO * CTouchInputState::GetInput(DWORD id)
{
	for (int i = 0; i < m_nInputs; i++)
	{
		if (m_pInputs[i].ID == id)
			return &m_pInputs[i];
	}

	return NULL;
}

const STOUCHINFO * CTouchInputState::GetInput(int touch_order)
{
	for (int i = 0; i < m_nInputs; i++)
	{
		if (m_pInputs[i].touch_order == touch_order)
			return &m_pInputs[i];
	}
	return nullptr;
}
