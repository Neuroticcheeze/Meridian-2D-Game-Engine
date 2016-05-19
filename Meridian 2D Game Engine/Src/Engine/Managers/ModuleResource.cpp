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
#include <fstream>
using std::ifstream;
using std::streampos;
using std::ios;

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
	ifstream file(GetPath("arch/resource.bin"), ifstream::binary);

	//Get the size of the memory block and reset the pointer in the stream to 0
	std::streampos fsize = 0;
	fsize = file.tellg();
	file.seekg(0, ios::end);
	fsize = file.tellg() - fsize;
	file.clear();
	file.seekg(0, ifstream::beg);

	//The buffer as chars to be conv'd to floats later
	char * data = static_cast<char*>(malloc(fsize));

	if (!file.is_open())
		return;
	//TODO: print error that resource file couldn't be found.

	file.read(data, fsize);
	file.close();

	/*block layout:

	Block Header:
		4 bytes = block size
		16 bytes = asset name
		1 byte = asset type + any flags in last 4 bits?

		Asset Header:
			# bytes for each property spec.
			# bytes for potential "main data block"
	*/
}

string ResourceManager::GetPath(const char * p_subdir) const
{
	return m_rootPath + (p_subdir + (*p_subdir == '/' || *p_subdir == '\\' ? 1 : 0));
}