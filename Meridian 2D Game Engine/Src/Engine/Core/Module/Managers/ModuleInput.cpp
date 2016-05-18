/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	ModuleInput.cpp
//
===================================================================*/

#include "ModuleInput.hpp"
#include <glfw3.h>

using namespace Meridian;

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Initialise()
{

}

void InputManager::Update(const float & p_dt)
{
	static double x, y;

	//TODO: make a way to access the window anywhere! OR make this check if the render manager is implemented, otherwise, don't compile this piece.
	glfwGetCursorPos(nullptr, &x, &y);

	m_mousePosition.x = static_cast<unsigned int>(x);
	m_mousePosition.y = static_cast<unsigned int>(y);
}

void InputManager::Render()
{

}

void InputManager::Finalise()
{

}

const uvec2 & InputManager::GetMousePosition() const
{
	return m_mousePosition;
}