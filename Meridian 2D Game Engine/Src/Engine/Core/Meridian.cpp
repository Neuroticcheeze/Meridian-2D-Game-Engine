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

#include "..\Managers\ModuleInput.hpp"
#include "..\Managers\ModuleResource.hpp"
#include "..\Managers\ModuleGraphics.hpp"
#include "..\Managers\ModuleAudio.hpp"

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
	//Delete any modules to release memory when this engine gets deleted.
	delete m_inputManager;
	delete m_resourceManager;
	delete m_graphicsManager;
}

void MeridianEngine::Load()
{
	if (m_isInitialised)
		return;

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

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_monitorSize = vec2(mode->width, mode->height);

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		Unload();
		return;
	}

	//Since by this point, we've successfully started everything up, we can now create and initialise all modules.

	(m_inputManager = new InputManager())->Initialise(this);
	(m_resourceManager = new ResourceManager())->Initialise(this);
	(m_graphicsManager = new GraphicsManager())->Initialise(this);
	(m_audioManager = new AudioManager())->Initialise(this);

	if (m_preHook != nullptr)
		m_preHook(this);
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

		static int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		m_prevViewport = m_nowViewport;
		m_nowViewport.z = static_cast<float>(w);
		m_nowViewport.w = static_cast<float>(h);

		//------------------------------------------------
		m_audioManager->Update(this, m_deltaTime);

		m_graphicsManager->Update(this, m_deltaTime);
		m_graphicsManager->Render(this);

		if (m_loopHook != nullptr)
			m_loopHook(this, m_deltaTime);
		
		m_inputManager->Update(this, m_deltaTime);
		//------------------------------------------------

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

	//Run the hook and give it free reign with the engine before breaking anything.
	if (m_postHook != nullptr)
		m_postHook(this);

	//Finalise all attached modules.
	m_inputManager->Finalise(this);
	m_resourceManager->Finalise(this);
	m_graphicsManager->Finalise(this);
	m_audioManager->Finalise(this);

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

void MeridianEngine::SetViewport(const ViewChangeState & p_whatToChange, const vec4 & p_value)
{
	switch (p_whatToChange)
	{
	case ViewChangeState::VIEWPORT_ALL:
		SetViewport(ViewChangeState::VIEWPORT_POSITION, p_value);
		SetViewport(ViewChangeState::VIEWPORT_SIZE, p_value);
		break;
	case ViewChangeState::VIEWPORT_POSITION:
		m_nowViewport.x = p_value.x;
		m_nowViewport.y = p_value.y;
		break;
	case ViewChangeState::VIEWPORT_SIZE:
		//if (vec2(p_value.zw) == m_monitorSize)
		//else//TODO: add fullscreen mode like this?
			glfwSetWindowSize(m_window, static_cast<int>(p_value.z), static_cast<int>(p_value.w));
		break;
	}
}

MeridianEngine::ViewChangeState MeridianEngine::ViewportWhatChanged() const
{
	if (m_prevViewport.x != m_nowViewport.x || m_prevViewport.y != m_nowViewport.y)
		return ViewChangeState::VIEWPORT_POSITION;

	if (m_prevViewport.z != m_nowViewport.z || m_prevViewport.w != m_nowViewport.w)
		return ViewChangeState::VIEWPORT_SIZE;

	if ((m_prevViewport.z != m_nowViewport.z || m_prevViewport.w != m_nowViewport.w) && (m_prevViewport.x != m_nowViewport.x || m_prevViewport.y != m_nowViewport.y))
		return ViewChangeState::VIEWPORT_ALL;

	return ViewChangeState::VIEWPORT_NOTHING;
}

void MeridianEngine::HookToPreEvent(OnEnginePre p_event) { m_preHook = p_event; }
void MeridianEngine::HookToLoopEvent(OnEngineLoop p_event) { m_loopHook = p_event; }
void MeridianEngine::HookToPostEvent(OnEnginePost p_event) { m_postHook = p_event; }