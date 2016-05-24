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
using std::ofstream;
using std::streampos;
using std::ios;

#include <direct.h>
#include <chrono>
#include <thread>

#define EX_HEADER_SIZE				4 + 16 + 1
#define EX_HEADER_SIZE_mTYPE		4 + 16
#define EX_HEADER_SIZE_mTYPE_mNAME	4

using namespace Meridian;

ResourceManager::ResourceManager()
{
	m_rootPath = RESOURCE_PATH;
}

ResourceManager::~ResourceManager()
{
	Clear();
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

void ResourceManager::Clear()
{
	for (auto it : m_loadedAssets)
	{
		delete it.second;
	}

	m_loadedAssets.clear();
}

void ResourceManager::SaveResources()
{
#ifdef _DEBUG

	if (_mkdir((m_rootPath + "ro/").c_str()) != 0)
		return;
		//TODO: print error that the directory couldn't be created.

	ofstream file(GetPath("ro/resource.bin"), ofstream::binary | ofstream::trunc);

	//Store

	SerialBuffer buff;

	for (auto it : m_loadedAssets)
	{
		const IAsset & asset = *it.second;

		asset.Encode(buff);

		int size = EX_HEADER_SIZE + buff.Size();
		
		char name[16];
		memcpy_s(name, 16, it.first.c_str(), 16);

		byte type = asset.ID();

		file.write(reinterpret_cast<const char*>(&size), sizeof(size));
		file.write(reinterpret_cast<const char*>(&name), sizeof(name));
		file.write(reinterpret_cast<const char*>(&type), sizeof(type));
		file.write(reinterpret_cast<const char*>(buff.C_Data()), buff.Size());
	}


	file.close();

	Clear();

#endif
}

void ResourceManager::LoadResources()
{
	//TODO: change serialbuffer to not reallocate. Just set the data ptr to "data" over here with an offset. It's faster. duh.

	ifstream file(GetPath("ro/resource.bin"), ifstream::binary);

	if (file.fail())
		return;
		//TODO: print error that resource file couldn't be found.

	//Get the size of the memory block and reset the pointer in the stream to 0
	streampos fsize = 0;
	fsize = file.tellg();
	file.seekg(0, ios::end);
	fsize = file.tellg() - fsize;
	file.clear();
	file.seekg(0, ifstream::beg);

	if (static_cast<int>(fsize) == 0)
	{
		file.close();
		return;
		//TODO: print error that resource file is empty or corrupt.
	}


	//The buffer as chars to be conv'd to floats later
	char * data = static_cast<char*>(malloc(static_cast<size_t>(fsize)));

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

	int currentBlockSize = 0, currentAssetSize;
	char * assetName = nullptr;
	byte assetType;

	SerialBuffer buff;
	
	for (int blockStart = 0; blockStart < static_cast<int>(fsize); blockStart += currentBlockSize)
	{
		//Get how big this block(asset) size is.
		memcpy_s(&currentBlockSize, 4, data + blockStart, 4);

		assert(currentBlockSize < 68157440);//No asset should be bigger than 65 megabytes! (About the size of a 4096 square 4-channel non-compressed image)

		//Get the name of the asset.
		assetName = static_cast<char*>(malloc(16));
		memcpy_s(assetName, 16, data + blockStart + EX_HEADER_SIZE_mTYPE_mNAME, 16);
		*(assetName + 15) = 0;

		//Get the type of the asset.
		memcpy_s(&assetType, 1, data + blockStart + EX_HEADER_SIZE_mTYPE, 1);

		//Get the actual asset header and data, and store it in a SerialBuffer.
		currentAssetSize = currentBlockSize - (EX_HEADER_SIZE);
		buff.Reallocate(currentAssetSize);
		memcpy_s(buff.Data(), currentAssetSize, data + blockStart + EX_HEADER_SIZE, currentAssetSize);

		IAsset * newAsset = nullptr;
		AssetFactoryGenerate(assetType, &newAsset);

		newAsset->Decode(buff);

		m_loadedAssets[string(assetName)] = newAsset;
	}

	//Release the memory we created.
	free(data);
}

string ResourceManager::GetPath(const char * p_subdir) const
{
	return m_rootPath + (p_subdir + (*p_subdir == '/' || *p_subdir == '\\' ? 1 : 0));
}

void ResourceManager::AssetFactoryGenerate(const byte & p_id, IAsset ** p_value)
{
	auto factory = m_factoryTable[p_id];

	if (factory == nullptr)
		return;

	factory(p_value);
}