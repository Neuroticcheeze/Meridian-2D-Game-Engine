/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	ModuleGraphics.cpp
//
===================================================================*/

#include "ModuleGraphics.hpp"
#include "..\Core\Meridian.hpp"
#include <glfw3.h>

using namespace Meridian;

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{

}

void GraphicsManager::Initialise(MeridianEngine * p_engine)
{

}

void GraphicsManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
}

void GraphicsManager::Render(MeridianEngine * p_engine)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(p_engine->GetWindow());
}

void GraphicsManager::Finalise(MeridianEngine * p_engine)
{

}