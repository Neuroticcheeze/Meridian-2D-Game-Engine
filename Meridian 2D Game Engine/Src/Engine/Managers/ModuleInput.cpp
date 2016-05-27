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

void InputManager::OnKey(GLFWwindow *, int p_keycode, int p_scancode, int p_action, int p_mods)//TODO: finish this functionality
{
	m_keyStates[p_keycode] = 
		p_action == GLFW_PRESS ? KeyState::TAP : 
		(p_action == GLFW_RELEASE ? KeyState::RELEASE : 
			m_keyStates[p_keycode]);
}

InputManager::InputManager()
{
	for (int p_keycode = 0; p_keycode < GLFW_MAX_KEYS; ++p_keycode)
	{
		m_keyStates[p_keycode] = KeyState::UP;
	}
}

InputManager::~InputManager()
{

}

void InputManager::Initialise(MeridianEngine * p_engine)
{
	glfwSetKeyCallback(p_engine->GetWindow(), InputManager::OnKey);
}

void InputManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
	static double x, y;

	glfwGetCursorPos(p_engine->GetWindow(), &x, &y);

	m_mousePosition.x = static_cast<unsigned int>(max(x, 0.0));
	m_mousePosition.y = static_cast<unsigned int>(max(y, 0.0));

	for (int p_keycode = 0; p_keycode < GLFW_MAX_KEYS; ++p_keycode)
	{
		auto & s = m_keyStates[p_keycode];
		if (s == KeyState::TAP)
			m_keyStates[p_keycode] = KeyState::DOWN;

		if (s == KeyState::RELEASE)
			m_keyStates[p_keycode] = KeyState::UP;
	}
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