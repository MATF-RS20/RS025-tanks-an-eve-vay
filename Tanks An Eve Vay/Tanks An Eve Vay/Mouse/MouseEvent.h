#pragma once

struct MousePoint
{
	int x;
	int y;
};

class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};
private:
	EventType m_Type;
	int m_X;
	int m_Y;
public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	bool isValid() const;
	EventType GetEventType() const;
	MousePoint GetMousePosition() const;
	int GetMouseX() const;
	int GetMouseY() const;
};