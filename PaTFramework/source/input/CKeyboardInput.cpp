#include "CKeyboardInput.h"
#include "..\helpers\MemoryHelper.h"
#include "..\helpers\StringHelper.h"

void CKeyboardInputState::OnKeyPressed(UINT keyCode)
{
	if (keyCode < 0)
		return;
	if (keyCode >= 256)
		return;

	if (keyState[keyCode].nFramesDown == 0)
	{
		if (keyCode == VK_DELETE)
		{
			OnCharacterPressed(VK_DELETE);
		}

		keyState[keyCode].isPressed = true;
	}
	keyState[keyCode].isDown = true;
	keyState[keyCode].nFramesUp = 0;

}

void CKeyboardInputState::OnKeyReleased(UINT keyCode)
{
	if (keyCode < 0)
		return;
	if (keyCode >= 256)
		return;

	if (keyState[keyCode].nFramesUp == 0)
	{
		keyState[keyCode].isReleased = true;
	}
	keyState[keyCode].isDown = false;
	keyState[keyCode].nFramesDown = 0;

}

void CKeyboardInputState::OnCharacterPressed(char c)
{
	AddToBuffer(c);
}

void CKeyboardInputState::Update()
{
	for (int i = 0; i < 256; i++)
	{
		keyState[i].isPressed = false;
		keyState[i].isReleased = false;
		if (keyState[i].isDown)
			keyState[i].nFramesDown++;
		else
			keyState[i].nFramesUp++;
	}

	m_inputBuffer[0] = '\0';
	m_bufLen = 0;
}

void CKeyboardInputState::AddToBuffer(char keyCode)
{
	if (m_bufLen + 1 >= sizeof(m_inputBuffer) / sizeof(char))
		return;

	m_bufLen++;
	m_inputBuffer[m_bufLen] = '\0';
	m_inputBuffer[m_bufLen - 1] = keyCode;
}
