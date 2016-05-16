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

void MeridianEngine::Run()
{
	while (m_isRunning)
	{
		//TODO: run the game
	}
}

void MeridianEngine::Unload()
{
}

void MeridianEngine::Terminate()
{
	m_isRunning = false;
}