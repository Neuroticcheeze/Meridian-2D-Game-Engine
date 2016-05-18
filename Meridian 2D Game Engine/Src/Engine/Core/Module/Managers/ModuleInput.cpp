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
#include "..\..\Meridian.hpp"
#include <glfw3.h>
#include <glm\glm.hpp>
using glm::max;

using namespace Meridian;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{

}

void InputManager::Initialise(MeridianEngine * p_engine)
{

}

void InputManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
	static double x, y;

	glfwGetCursorPos(p_engine->GetWindow(), &x, &y);

	m_mousePosition.x = static_cast<unsigned int>(max(x, 0.0));
	m_mousePosition.y = static_cast<unsigned int>(max(y, 0.0));
}

void InputManager::Render(MeridianEngine * p_engine)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(p_engine->GetWindow());
}

void InputManager::Finalise(MeridianEngine * p_engine)
{

}

const uvec2 & InputManager::GetMousePosition() const
{
	return m_mousePosition;
}