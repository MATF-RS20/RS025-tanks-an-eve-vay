#pragma once

#include "KeyboardEvent.h"
#include <queue>

class KeyboardClass
{
public:
	KeyboardClass();
	bool IsKeyPressed(const unsigned char keycode);
	bool EventBuffer_IsEmpty();
	bool CharBuffer_IsEmpty();
	KeyboardEvent ReadNextEvent();
	unsigned char ReadNextChar();
	void OnKeyDown(const unsigned char key);
	void OnKeyUp(const unsigned char key);
	void OnKeyPressed(const unsigned char key);


private:
	bool m_autoRepeatKeys = false;
	bool m_autoRepeatChars = false;
	bool m_KeyState[256];
	std::queue<KeyboardEvent> m_EventBuffer;
	std::queue<unsigned char> m_CharBuffer;

};