/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleResource.hpp
//
//Objects:		ResourceManager
//					-Initialise
//					-Finalise
//					-LoadResources
//					-GetPath
//					-GetAsset
//					-CreateAsset
//
===================================================================*/

#pragma once

#include "..\Core\Asset.hpp"

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <map>
using std::map;

#include <functional>
using std::function;

#ifdef _DEBUG
#define RESOURCE_PATH "Res/"
#else
#define RESOURCE_PATH "data/"
#endif

typedef unsigned char byte;

namespace Meridian
{
	//Typedef Function Object
	typedef function<void(IAsset **)> AssetFactory;

	/*=============================================================================
	Responsible for controlling how textures, sounds, etc, are read and stored in 
	the game as well as providing an interface to create assets and retrieve them
	from the game's directory at lightning speed.
	=============================================================================*/
	class ResourceManager
	{
	public:///Friendly Internal Forward Declarations

		friend class MeridianEngine;

	private:///Constructors/Destructors

		ResourceManager();
		~ResourceManager();

	private:///Member Functions

			/*Initialise this module and load anything it needs to run long-term.*/
		void Initialise(MeridianEngine * p_engine);

			/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		void Finalise(MeridianEngine * p_engine);

	public:///Resource utilities

			/*Wipe all loaded assets during runtime. (Leaves the resource file untouched.)*/
		void Clear();

			/*Used only during debug to store any loaded assets in the resource manager to the binary resource file.*/
		void SaveResources();

			/*Used always (debug + release) to load any serialised assets from the binary resource file.*/
		void LoadResources();

			/*Get the path to all of our resources given a subdirectory too.*/
		string GetPath(const char * p_subdir = nullptr) const;

			/*Get the asset by the specified identifier given to it during CreateAsset*/
		template <typename T>
		inline bool GetAsset(const char * p_identifier, T ** p_asset)
		{
			static_assert(std::is_base_of<IAsset, T>::value, "T must derive from IAsset");

			auto it = m_loadedAssets.find(string(p_identifier));

			if (it != m_loadedAssets.end())
			{
				*p_asset = static_cast<T*>(it->second);
				return true;
			}

			return false;
		}

			/*Creates an asset from some resource data defined in the doc. (For example: filepaths, width, size).
			What is required varies for each type of asset. This function does nothing in release mode since the
			engine switches this out for the resource file it creates from your assets during development (debug).*/
		template<typename T, typename ... Args>
		inline bool CreateAsset(const char * p_identifier, Args ... p_args)
		{
#ifdef _DEBUG
			
			assert(strlen(p_identifier) < 16);//Must be at most 15 characters long

			static_assert(std::is_base_of<IAsset, T>::value, "T must derive from IAsset");

			auto it = m_loadedAssets.find(string(p_identifier));

			if (it != m_loadedAssets.end())
				return false;	//TODO: print error message to prompt a delete of this asset before this function can work.

			vector<RawProperty> properties;

			Args_CreateAsset(properties, p_args ...);

			T * asset = new T();
			asset->Load(properties.data());

			//Set the factory table with this type of asset.
			auto & factoryAt = m_factoryTable[asset->ID()];
			if (factoryAt == nullptr) 
				factoryAt = [](IAsset ** p_val) { *p_val = new T(); };

			//Add to the loaded assets map.
			m_loadedAssets[string(p_identifier)] = asset;

#else

			T * asset = new T();

			//Set the factory table with this type of asset.
			auto & factoryAt = m_factoryTable[asset->ID()];
			if (factoryAt == nullptr)
				factoryAt = [](IAsset ** p_val) { *p_val = new T(); };

			delete asset;

#endif

			return true;
		}

#ifdef _DEBUG
	private:///Resource utilities

		template<typename T, typename ... Args>
		inline void Args_CreateAsset(vector<RawProperty> & p_properties, T p_val, Args ... p_args)
		{
			Args_CreateAsset(p_properties, p_val);
			Args_CreateAsset(p_properties, p_args ...);
		}

		template<typename T>
		inline void Args_CreateAsset(vector<RawProperty> & p_properties, T p_val)
		{
			p_properties.push_back(RawProperty().Set(p_val));
		}
#endif

			/*Instantiate a new asset object using the factory table to do it.*/
		void AssetFactoryGenerate(const byte & p_id, IAsset ** p_value);

	private:///Member Fields

		string m_rootPath;

		map<string, IAsset *> m_loadedAssets;

		AssetFactory m_factoryTable[256];
	};
}