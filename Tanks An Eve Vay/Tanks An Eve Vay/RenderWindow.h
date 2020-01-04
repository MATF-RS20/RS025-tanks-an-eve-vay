#pragma once

#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	RenderWindow();
	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowTitle,std::string windowClass,int width,int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND m_Handle = NULL;
	HINSTANCE m_HInstance = NULL;
	std::string m_WindowTitle = "";
	std::wstring m_WindowTitleWide = L"";
	std::string m_WindowClass = "";
	std::wstring m_WindowClassWide = L"";
	int m_Width = 0;
	int m_Height = 0;
};