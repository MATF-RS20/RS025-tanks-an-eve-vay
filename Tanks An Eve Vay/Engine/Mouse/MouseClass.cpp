#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::LRelease, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnRightPressed(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::RPress, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnRightReleased(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::RRelease, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::MPress, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::MRelease, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnWheelUp(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::WheelUp, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnWheelDown(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::WheelDown, x, y);
	m_EventBuffer.push(me);
}

void MouseClass::OnMouseMove(int x, int y)
{
	m_LeftDown = true;
	MouseEvent me(MouseEvent::EventType::Move, x, y);
	m_EventBuffer.push(me);
}

bool MouseClass::IsLeftDown()
{
	return m_LeftDown;
}

bool MouseClass::IsMiddleDown()
{
	return m_MiddleDown;
}

bool MouseClass::IsRightDown()
{
	return m_RightDown;
}

int MouseClass::GetPosX()
{
	return m_X;
}

int MouseClass::GetPosY()
{
	return m_Y;
}

MousePoint MouseClass::GetPosition()
{
	return { m_X,m_Y };
}

bool MouseClass::EventBuffer_IsEmpty()
{
	return m_EventBuffer.empty();
}

MouseEvent MouseClass::ReadNextEvent()
{
	if (!EventBuffer_IsEmpty())
	{
		MouseEvent e = m_EventBuffer.front();
		m_EventBuffer.pop();
		return e;
	}
	else
	{
		return MouseEvent();
	}
}