#pragma once
class KeyboardEvent
{
public:
	enum EventType
	{
		KeyDown,
		KeyUp,
		Invalid
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsKeyDown() const;
	bool IsKeyUp() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

private:
	EventType m_Type;
	unsigned char m_KeyCode;

};