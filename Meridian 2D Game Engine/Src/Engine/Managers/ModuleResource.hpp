/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleResource.hpp
//
//Objects:		ResourceManager
//					-Implements IModule
//					-LoadResources
//					-GetPath
//					-GetAsset
//					-CreateAsset
//
===================================================================*/

#pragma once

#include "..\Core\Module.hpp"
#include "..\Core\Asset.hpp"

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <unordered_map>
using std::unordered_map;

#ifdef _DEBUG
#define RESOURCE_PATH "Res/"
#else
#define RESOURCE_PATH "data/"
#endif

typedef unsigned char byte;

namespace Meridian
{
	/*=============================================================================
	Responsible for controlling how textures, sounds, etc, are read and stored in 
	the game as well as providing an interface to create assets and retrieve them
	from the game's directory at lightning speed.
	=============================================================================*/
	class ResourceManager : public IModule
	{
	public:///Friendly Internal Forward Declarations

		friend class MeridianEngine;

	private:///Constructors/Destructors

		ResourceManager();
		~ResourceManager();

	private:///Member Functions

			/*Initialise this module and load anything it needs to run long-term.*/
		void Initialise(MeridianEngine * p_engine);

			/*Update this module inside the engine's gameloop.*/
		void Update(MeridianEngine * p_engine, const float & p_dt);

			/*Immediately render anything this module has to inside the OpenGL clear-swap space*/
		void Render(MeridianEngine * p_engine);

			/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		void Finalise(MeridianEngine * p_engine);

	public:///Resource utilities

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

			static_assert(std::is_base_of<IAsset, T>::value, "T must derive from IAsset");

			auto it = m_loadedAssets.find(string(p_identifier));

			if (it != m_loadedAssets.end())
				return false;	//TODO: print error message to prompt a delete of this asset before this can work.

			vector<RawProperty> properties;

			Args_CreateAsset(properties, p_args ...);

			T * asset = new T();
			asset->Load(properties.data());

			m_loadedAssets[string(p_identifier)] = asset;

			return true;
#endif
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

	private:///Member Fields

		string m_rootPath;

		unordered_map<string, IAsset*> m_loadedAssets;
	};
}