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

#include <assert.h>
#include <glfw3.h>
#include <Windows.h>

using namespace Meridian;

MeridianEngine::MeridianEngine() :
	m_isRunning(false)
{

}

MeridianEngine::~MeridianEngine()
{

}

void MeridianEngine::Load()
{

}

void MeridianEngine::Run(const GameLoopMode & p_mode)
{
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
		m_targetFramerate = lpDevMode.dmDisplayFrequency;
	}




	float m_target = 1000.0F / m_targetFramerate;
	float m_invTarget = m_targetFramerate / 1000.0F;

	float m_startTime = 0, m_finishTime = 0;
	float m_remainder = 0, m_duration = 0, m_deltaTime = 0;

	while (m_isRunning)
	{
		m_startTime = (float)glfwGetTime();

		//Loop
		printf("Capped FPS: %f, True FPS: %f\n", min(1.0F / m_duration, m_targetFramerate), 1.0F / m_duration);
		for (auto module : m_modules)
		{
			module->Update(m_deltaTime);
		}

		m_finishTime = (float)glfwGetTime();

		m_duration = m_finishTime - m_startTime;

		m_remainder = m_target - m_duration;

		if (m_remainder > 0)
		{
			Sleep(m_remainder);
		}

		else
		{
			m_deltaTime = m_duration * m_invTarget;
		}
	}
}

void MeridianEngine::Unload()
{
}

void MeridianEngine::Terminate()
{
	m_isRunning = false;
}