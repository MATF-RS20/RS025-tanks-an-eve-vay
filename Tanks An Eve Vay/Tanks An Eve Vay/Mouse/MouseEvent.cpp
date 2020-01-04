#include "MouseEvent.h"

MouseEvent::MouseEvent()
	:m_Type(EventType::Invalid),m_X(0),m_Y(0)
{
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y)
	:m_Type(type),m_X(x),m_Y(y)
{
}

bool MouseEvent::isValid() const
{
	return m_Type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetEventType() const
{
	return m_Type;
}

MousePoint MouseEvent::GetMousePosition() const
{
	return{ m_X,m_Y };
}

int MouseEvent::GetMouseX() const
{
	return m_X;
}

int MouseEvent::GetMouseY() const
{
	return m_Y;
}
