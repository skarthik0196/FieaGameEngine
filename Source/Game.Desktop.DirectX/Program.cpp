// Game.Desktop.DirectX.cpp : Defines the entry point for the console application.
//
#pragma once
#include "pch.h"
#include<Windows.h>
#include<string>
#include"Renderer.h"
#include"World.h"
#include"Sector.h"
#include"Entity.h"
#include"TypeManager.h"

using namespace FieaGameEngine;

LRESULT CALLBACK WindowProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);

void InitializeWindow(HINSTANCE CurrentInstance, const std::wstring &ClassName, const std::wstring WindowTitle, int ShowCommand);

HWND WindowHandle;
WNDCLASSEX WindowClass;
int ClientWidth = 1024;
int ClientHeight = 768;

void RegisterTypes()
{
	TypeManager::RegisterType(World::TypeIdClass(), World::GetSignature());
	TypeManager::RegisterType(Sector::TypeIdClass(), Sector::GetSignature());
	TypeManager::RegisterType(Entity::TypeIdClass(), Entity::GetSignature());
}

int WINAPI WinMain(HINSTANCE CurrentInstance, HINSTANCE PreviousInstance, LPSTR CommandLine, int ShowCommand)
{

	//UNREFERENCED_PARAMETER(CurrentInstance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	//UNREFERENCED_PARAMETER(ShowCommand);

	RegisterTypes();

	//MessageBox(nullptr, L"Hello, World", L"Game.Desktop.DirectX", MB_ICONEXCLAMATION | MB_OK);

	std::wstring WindowClassName = L"Game.DirectX";
	std::chrono::high_resolution_clock::time_point PreviousTime;

	InitializeWindow(CurrentInstance, WindowClassName, L"Game.DirectX", ShowCommand);

	Rendering::Renderer *D3DRenderer = new Rendering::Renderer(WindowHandle, ClientWidth, ClientHeight);
	D3DRenderer->InitializeRenderer();
	FieaGameEngine::World W1("W1");
	W1.Update();
	PreviousTime = W1.GetWorldState().GetGameTime().CurrentTime();
	MSG Message;

	while (true)
	{
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
			if (Message.message == WM_QUIT)
			{
				//DXRenderer->ShutDownD3D();
				break;
			}
		}
		else
		{
			W1.Update();
			std::chrono::milliseconds deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(W1.GetWorldState().GetGameTime().CurrentTime() - PreviousTime);
			D3DRenderer->Update(deltaTime);
			PreviousTime = W1.GetWorldState().GetGameTime().CurrentTime();
			
			D3DRenderer->RenderFrame();
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
	//WindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW+1;
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
