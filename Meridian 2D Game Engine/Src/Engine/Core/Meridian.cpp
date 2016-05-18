/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	Meridian.cpp
//
===================================================================*/

#include "Meridian.hpp"
#include "Module\Module.hpp"

#include <gl_core_4_4.h>
#include <assert.h>
#include <glfw3.h>
#include <Windows.h>
#include <glm\glm.hpp>
using glm::min;

using namespace Meridian;

MeridianEngine::MeridianEngine() :
	m_isRunning(false),
	m_isInitialised(false)
{

}

MeridianEngine::~MeridianEngine()
{
	//Delete any modules and clear the list to release memory when this engine gets deleted.
	for (auto module : m_modules)
	{
		delete module;
	}

	m_modules.clear();
}

void MeridianEngine::Load()
{
	if (glfwInit() == GL_TRUE)
	{
		m_isInitialised = true;

		//If the window was not successfully created, unload the engine. Since the game loop hasn't been hit yet, unload runs straight away.
		if ((m_window = glfwCreateWindow(640, 480, "Meridian-2D Game", nullptr, nullptr)) == nullptr)
		{
			Unload();
			return;
		}
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		Unload();
		return;
	}

	//Since by this point, we've successfully started everything up, we can now initialise all attached modules.

	for (auto module : m_modules)
	{
		module->Initialise(this);
	}
}

void MeridianEngine::Run(const GameLoopMode & p_mode)
{
	//TODO: Neaten up and make input run as fast as possible, update run at a fixed framerate, and render fixed at the refresh rate.
	//TODO: Make p_mode actually DO something...

	m_isRunning = true;

	float m_targetFramerate;

	DEVMODE lpDevMode;
	memset(&lpDevMode, 0, sizeof(DEVMODE));
	lpDevMode.dmSize = sizeof(DEVMODE);
	lpDevMode.dmDriverExtra = 0;

	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode) == 0)
	{
		m_targetFramerate = 60; // default value if cannot retrieve from user settings.
	}
	else
	{
		m_targetFramerate = static_cast<float>(lpDevMode.dmDisplayFrequency);
	}

	float m_target = 1000.0F / m_targetFramerate;
	float m_invTarget = m_targetFramerate / 1000.0F;

	float m_startTime = 0, m_finishTime = 0;
	float m_remainder = 0, m_duration = 0, m_deltaTime = 0;

	while (m_isRunning && !glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		m_startTime = (float)glfwGetTime();

		//===============Loop===============
		///printf("Capped FPS: %f, True FPS: %f\n", min(1.0F / m_duration, m_targetFramerate), 1.0F / m_duration);
		for (auto module : m_modules)
		{
			module->Update(this, m_deltaTime);
			module->Render(this);
		}
		//==================================

		m_finishTime = (float)glfwGetTime();

		m_duration = m_finishTime - m_startTime;

		m_remainder = m_target - m_duration;

		if (m_remainder > 0)
		{
			Sleep(static_cast<DWORD>(m_remainder));
		}

		else
		{
			m_deltaTime = m_duration * m_invTarget;
		}
	}
}

void MeridianEngine::Unload()
{
	//If we're not initialised, then we cannot finalise anything.
	if (!m_isInitialised)
		return;


	//Finalise all attached modules.
	for (auto module : m_modules)
	{
		module->Finalise(this);
	}

	//Kill the window and terminate glfw.
	glfwDestroyWindow(m_window);
	glfwTerminate();

	//No longer initialised since we've undone everything initialise did.
	m_isInitialised = false;
}

void MeridianEngine::Terminate()
{
	//Kill the game loop by the next iteration.
	m_isRunning = false;
}