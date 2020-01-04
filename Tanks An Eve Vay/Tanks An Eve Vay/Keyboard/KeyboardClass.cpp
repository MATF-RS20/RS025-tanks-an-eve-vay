#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
	for (unsigned i = 0; i < 256; i++)
	{
		m_KeyState[i] = false;
	}
}

bool KeyboardClass::IsKeyPressed(const unsigned char keycode)
{
	return m_KeyState[keycode];
}

bool KeyboardClass::EventBuffer_IsEmpty()
{
	return m_EventBuffer.empty();
}

bool KeyboardClass::CharBuffer_IsEmpty()
{
	return m_CharBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadNextEvent()
{
	if (!EventBuffer_IsEmpty())
	{
		KeyboardEvent e = m_EventBuffer.front();
		m_EventBuffer.pop();
		return e;
	}
	else
	{
		return KeyboardEvent();
	}
}

unsigned char KeyboardClass::ReadNextChar()
{
	if (!CharBuffer_IsEmpty())
	{
		unsigned char k = m_CharBuffer.front();
		m_CharBuffer.pop();
		return k;
	}
	else
	{
		return 0u;
	}
}

void KeyboardClass::OnKeyDown(const unsigned char key)
{
	m_KeyState[key] = true;
	m_EventBuffer.push(KeyboardEvent(KeyboardEvent::EventType::KeyDown, key));
}

void KeyboardClass::OnKeyUp(const unsigned char key)
{
	m_KeyState[key] = false;
	m_EventBuffer.push(KeyboardEvent(KeyboardEvent::EventType::KeyUp, key));
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	m_CharBuffer.push(key);
}
