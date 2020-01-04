#include "Engine.h"
#include "GameManager.h"
bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	if (!m_RenderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height))
	{
		return false;
	}
	if (!m_Graphics.Initialize(m_RenderWindow.GetHWND(), width, height))
	{
		return false;
	}
	return true;
}

bool Engine::ProcessMessages()
{
	return m_RenderWindow.ProcessMessages();
}

void Engine::Update()
{
	while (!m_Keyboard.CharBuffer_IsEmpty())
	{
		unsigned char ch = m_Keyboard.ReadNextChar();
	}

	while (!m_Keyboard.EventBuffer_IsEmpty())
	{
		KeyboardEvent currentEvent = m_Keyboard.ReadNextEvent();
		unsigned char keycode = currentEvent.GetKeyCode();

		if (!currentEvent.IsKeyDown())
		{
			continue;
		}

		switch (keycode)
		{
		case VK_LEFT:
			GameManager::MovePlayer(1, Vector2f(-0.1f, 0.0f));
			break;
		case VK_RIGHT:
			GameManager::MovePlayer(1, Vector2f(0.1f, 0.0f));
			break;
		}
	}

	while (!m_Mouse.EventBuffer_IsEmpty())
	{
		MouseEvent me = m_Mouse.ReadNextEvent();
	}
}

void Engine::RenderFrame()
{
	m_Graphics.RenderFrame();
}
