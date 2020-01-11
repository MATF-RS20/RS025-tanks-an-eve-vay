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
		case 'A':
			if(GameManager::MovesLeft() > 0)
			{
				GameManager::ReduceMoves();
				GameManager::AllowMove(LEFT);
			}
			
			break;
		case VK_RIGHT:
		case 'D':
			if (GameManager::MovesLeft() > 0)
			{
				GameManager::ReduceMoves();
				GameManager::AllowMove(RIGHT);
			}
			break;
		case VK_SPACE:
			if (GameManager::CanFire() == true)
			{
				GameManager::SetMovesDefault();
				GameManager::Fire();
			}
			break;
		case 'P':
			GameManager::ChangePlayer();
			break;
		}
	}

	while (!m_Mouse.EventBuffer_IsEmpty())
	{
		MouseEvent mouseEvent = m_Mouse.ReadNextEvent();
		if (mouseEvent.GetEventType() == MouseEvent::EventType::Move)
		{
			Vector2f position(mouseEvent.GetMouseX(), mouseEvent.GetMouseY());
			GameManager::RotateTurret(position);
		}
	}
}

void Engine::RenderFrame()
{
	m_Graphics.RenderFrame();
}
