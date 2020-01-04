#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass
{
public:
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPosition();
	
	bool EventBuffer_IsEmpty();
	MouseEvent ReadNextEvent();

private:
	std::queue<MouseEvent> m_EventBuffer;
	bool m_LeftDown = false;
	bool m_RightDown = false;
	bool m_MiddleDown = false;
	int m_X = 0;
	int m_Y = 0;
};