#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() :m_Type(EventType::Invalid), m_KeyCode(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key)
	: m_Type(type), m_KeyCode(key)
{
}

bool KeyboardEvent::IsKeyUp() const
{
	return m_Type == EventType::KeyUp;
}

bool KeyboardEvent::IsKeyDown() const
{
	return m_Type == EventType::KeyDown;
}

bool KeyboardEvent::IsValid() const
{
	return m_Type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return m_KeyCode;
}
