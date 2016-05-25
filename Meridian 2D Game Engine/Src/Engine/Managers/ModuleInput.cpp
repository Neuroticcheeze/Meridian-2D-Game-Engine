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
#include "..\Core\Meridian.hpp"
#include <glfw3.h>
#include <glm\glm.hpp>
using glm::max;

using namespace Meridian;

InputManager::KeyState InputManager::m_keyStates[GLFW_MAX_KEYS];

void InputManager::OnKey(GLFWwindow *, int p_keycode, int p_scancode, int p_action, int p_mods)
{
	m_keyStates[p_keycode] = 
		p_action == GLFW_PRESS ? KeyState::DOWN : 
		(p_action == GLFW_RELEASE ? KeyState::UP : 
			m_keyStates[p_keycode]);
}

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
}

void InputManager::Finalise(MeridianEngine * p_engine)
{

}

const uvec2 & InputManager::GetMousePosition() const
{
	return m_mousePosition;
}

const InputManager::KeyState & InputManager::GetKeyState(const int & p_keycode) const
{
	return m_keyStates[p_keycode];
}