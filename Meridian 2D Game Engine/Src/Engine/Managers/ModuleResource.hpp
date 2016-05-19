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
//
===================================================================*/

#pragma once

#include "..\Core\Module.hpp"

#include <glm/vec2.hpp>
using glm::uvec2;

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

			/*Creates an asset from some resource data defined in the doc. (For example: filepaths, width, size).
			What is required varies for each type of asset.*/
		template<typename T, typename ... Args>
		void CreateAsset(Args ... p_args)
		{

		}
		
	private:///Member Fields
	};
}