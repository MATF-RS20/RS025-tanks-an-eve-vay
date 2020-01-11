#include "WindowContainer.h"

RenderWindow::RenderWindow()
{
}

bool RenderWindow::Initialize(WindowContainer* pWindowContainer,HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
	m_HInstance = hInstance;
	m_Width = width;
	m_Height = height;
	m_WindowTitle = windowTitle;
	m_WindowTitleWide = StringConverter::StringToWide(windowTitle);
	m_WindowClass = windowClass;
	m_WindowClassWide = StringConverter::StringToWide(windowClass);

	RegisterWindowClass();

	m_Handle = CreateWindowEx(
		0,
		m_WindowClassWide.c_str(),
		m_WindowTitleWide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		0,
		m_Width,
		m_Height,
		NULL,
		NULL,
		m_HInstance,
		pWindowContainer
	);

	if (m_Handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEx" + m_WindowTitle);
		return false;
	}

	ShowWindow(m_Handle, SW_SHOW);
	SetForegroundWindow(m_Handle);
	SetFocus(m_Handle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	MSG message;
	ZeroMemory(&message, sizeof(MSG));
	while (PeekMessage(&message,
		m_Handle,
		0,
		0,
		PM_REMOVE
		))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	if (message.message == WM_NULL)
	{
		if (!IsWindow(m_Handle))
		{
			m_Handle = nullptr;
			UnregisterClass(m_WindowClassWide.c_str(), m_HInstance);
			return false;
		}
	}
	return true;
}

HWND RenderWindow::GetHWND() const
{
	return m_Handle;
}

RenderWindow::~RenderWindow()
{
	if (m_Handle != NULL)
	{
		UnregisterClass(m_WindowClassWide.c_str(), m_HInstance);
		DestroyWindow(m_Handle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			ErrorLogger::Log("Critical Error: pWindow is null during WM_NCCRREATW");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->m_HInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->m_WindowClassWide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}
