/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	ModuleResource.cpp
//
===================================================================*/

#include "ModuleResource.hpp"
#include "..\Core\Meridian.hpp"
#include <glfw3.h>

using namespace Meridian;

ResourceManager::ResourceManager()
{
	m_rootPath = RESOURCE_PATH;
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialise(MeridianEngine * p_engine)
{
}

void ResourceManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
}

void ResourceManager::Render(MeridianEngine * p_engine)
{
}

void ResourceManager::Finalise(MeridianEngine * p_engine)
{

}

void ResourceManager::LoadResources()
{

}

string ResourceManager::GetPath(const char * p_subdir) const
{
	return m_rootPath + (p_subdir + (*p_subdir == '/' || *p_subdir == '\\' ? 1 : 0));
}