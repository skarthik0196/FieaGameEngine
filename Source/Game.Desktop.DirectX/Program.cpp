// Game.Desktop.DirectX.cpp : Defines the entry point for the console application.
//
#pragma once
#include "pch.h"
#include<Windows.h>
#include<string>
#include"Renderer.h"

LRESULT CALLBACK WindowProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

void InitializeWindow(HINSTANCE CurrentInstance, const std::wstring &ClassName, const std::wstring WindowTitle, int ShowCommand);

HWND WindowHandle;
WNDCLASSEX WindowClass;
int ClientWidth = 1024;
int ClientHeight = 768;

int WINAPI WinMain(HINSTANCE CurrentInstance, HINSTANCE PreviousInstance, LPSTR CommandLine, int ShowCommand)
{

	//UNREFERENCED_PARAMETER(CurrentInstance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	//UNREFERENCED_PARAMETER(ShowCommand);


	//MessageBox(nullptr, L"Hello, World", L"Game.Desktop.DirectX", MB_ICONEXCLAMATION | MB_OK);

	std::wstring WindowClassName = L"Game.DirectX";

	InitializeWindow(CurrentInstance, WindowClassName, L"Game.DirectX", ShowCommand);
	
	Renderer *DXRenderer= new Renderer();
	
	DXRenderer->InitializeD3D(WindowHandle, ClientWidth, ClientHeight);

	MSG Message;

	while (true)
	{
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
			if (Message.message == WM_QUIT)
			{
				DXRenderer->ShutDownD3D();
				break;
			}
		}
		else
		{
			DXRenderer->RenderFrame();
		}
	}



	return 0;
	//return Message.wParam;
}


void InitializeWindow(HINSTANCE CurrentInstance, const std::wstring &ClassName, const std::wstring WindowTitle, int ShowCommand)
{

	ZeroMemory(&WindowClass, sizeof(WindowClass));
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = CurrentInstance;
	WindowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW+1;
	WindowClass.lpszClassName = ClassName.c_str();

	RegisterClassEx(&WindowClass);

	RECT WindowRect = { 0,0,ClientWidth,ClientHeight };
	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

	WindowHandle = CreateWindow(ClassName.c_str(), WindowTitle.c_str() , WS_OVERLAPPEDWINDOW, 200, 200, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, nullptr, nullptr, CurrentInstance, nullptr);

	int x;
	x = GetLastError();

	if (WindowHandle == nullptr)
	{
		return;
	}
	ShowWindow(WindowHandle, ShowCommand);
	UpdateWindow(WindowHandle);
}

LRESULT CALLBACK WindowProc(HWND WinHandle, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(WinHandle, Message, WParam, LParam);
}

