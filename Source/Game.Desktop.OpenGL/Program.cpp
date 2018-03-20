// Game.Desktop.OpenGL.cpp : Defines the entry point for the console application.
//
#pragma once
#include "pch.h"
#include<Windows.h>
#include<iostream>
#include<string>
#include "SList.h"
//#define GLEW_STATIC
#include<GL/glew.h>
//#define GLFW_DLL
#include <GLFW/glfw3.h>



//Initialize GLFW and GLEW
void InitializeOpenGL(GLFWwindow **Window)
{
	if (!glfwInit())
	{
		std::cout << "Error" << std::endl;
	}

	*Window = glfwCreateWindow(1024, 768, "Game.OpenGL", NULL, NULL);;

	if (!Window)
	{
		std::cout << "Could not open Window with GLFW3" << std::endl;
	}

	//Make OpenGL Context
	glfwMakeContextCurrent(*Window);

	//Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Define Viewport
	glViewport(0, 0, 1024, 768);

	//Get Video Adapter Info
	const GLubyte *Renderer = glGetString(GL_RENDERER);
	const GLubyte *Version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << Renderer << std::endl << "Version: " << Version << std::endl;

}

int WINAPI WinMain(HINSTANCE CurrentInstance, HINSTANCE PreviousInstance, LPSTR CommandLine, int ShowCommand)
{
	UNREFERENCED_PARAMETER(CurrentInstance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(ShowCommand);

	//Check if Library.Shared references work
	FieaGameEngine::SList<std::string> List1;
	//const FieaGameEngine::SList<std::string>& List2=List1;
	FieaGameEngine::SList<std::string>::Iterator It;
	

	bool s;
	s = List1.IsEmpty();
	{
		List1.PushBack("Hello");
	}
	std::string a = List1.Front();
	List1.PushFront("Hollow");
	a = List1.Front();
	a = List1.Back();
	List1.PushBack("World");
	a = List1.Back();
	int x;
	x = List1.Length();

	It=List1.begin();
	a = *It;
	++It;
	a = *It;
	*It = "XXX";
	const FieaGameEngine::SList<std::string>::Iterator& It2 = It;
	a = *It2;


	List1.PopFront();
	s = List1.IsEmpty();
	List1.Clear();

		
	

	//Create Window
	GLFWwindow *Window= glfwCreateWindow(1024, 768, "Game.OpenGL", NULL, NULL);

	InitializeOpenGL(&Window);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Program Loop
	while (!glfwWindowShouldClose(Window))
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(Window);
	}

	glfwTerminate();
    return 0;
}