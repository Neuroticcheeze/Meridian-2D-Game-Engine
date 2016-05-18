/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleGraphics.hpp
//
//Classes:		GraphicsManager
//					-Implements IModule
//
===================================================================*/

//TODO: Add interface with mouse and keyboard. Eventually for handheld devices, etc.

#pragma once

#include "..\Core\Module.hpp"

#include <glm/vec2.hpp>
using glm::uvec2;

namespace Meridian
{
	/*=============================================================================
	Responsible for coordinating how everything from text to lighting functions in 
	Meridian, as well as providing the ability to tailor the system to your game.
	=============================================================================*/
	class GraphicsManager : public IModule
	{
	public:///Friendly Internal Forward Declarations

		friend class MeridianEngine;

	private:///Constructors/Destructors

		GraphicsManager();
		~GraphicsManager();

	private:///Member Functions

			/*Initialise this module and load anything it needs to run long-term.*/
		void Initialise(MeridianEngine * p_engine);

			/*Update this module inside the engine's gameloop.*/
		void Update(MeridianEngine * p_engine, const float & p_dt);

			/*Immediately render anything this module has to inside the OpenGL clear-swap space*/
		void Render(MeridianEngine * p_engine);

			/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		void Finalise(MeridianEngine * p_engine);

	public:///Graphics utilities
		
	private:///Member Fields
	};
}