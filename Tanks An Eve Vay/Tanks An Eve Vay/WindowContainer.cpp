#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		
		const bool WasKeyDown = lParam & 0x40000000;
		if (!WasKeyDown)
		{
			m_Keyboard.OnKeyDown(keycode);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		m_Keyboard.OnKeyUp(keycode);
		return 0;

	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		const bool WasKeyDown = lParam & 0x40000000;
		if (!WasKeyDown)
		{
			m_Keyboard.OnKeyPressed(ch);
		}
		return 0;
	}

	//MOUSE EVENTS
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_Mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_Mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_Mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_Mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_Mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_Mouse.OnWheelUp(x,y);
		}
		else 
		{
			m_Mouse.OnWheelDown(x, y);
		}
		return 0;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}



}
