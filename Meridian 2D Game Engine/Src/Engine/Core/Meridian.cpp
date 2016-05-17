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
#include <assert.h>
#include <glfw3.h>

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

	// The amount of time we want to simulate each step, in milliseconds
	// (written as implicit frame-rate)
	float m_target = 60.0F;

	static float m_timeDelta = 1000.0F / m_target;
	static float m_timeAccumulator = 0;

	while (m_isRunning)
	{
		float m_timeSimulatedThisIteration = 0;
		float m_startTime = glfwGetTime();

		while (m_timeAccumulator >= m_timeDelta)
		{
			printf("Delta time %f", m_timeDelta);

			//stepGameState(m_timeDelta)
			m_timeAccumulator -= m_timeDelta;
			m_timeSimulatedThisIteration += m_timeDelta;
		}

		//stepAnimation(m_timeSimulatedThisIteration)

		//renderFrame() // OpenGL frame drawing code goes here
		//handleUserInput()

		m_timeAccumulator += glfwGetTime() - m_startTime;
	}
}

void MeridianEngine::Unload()
{
}

void MeridianEngine::Terminate()
{
	m_isRunning = false;
}