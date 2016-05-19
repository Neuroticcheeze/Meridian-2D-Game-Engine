/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	Module.hpp
//
//Objects:		IModule
//					-Initialise
//					-Update
//					-Render
//					-Finalise
//
===================================================================*/

#pragma once

namespace Meridian
{
	/*============================================================
	  Base structure for a component object of the game engine.
	============================================================*/
	class IModule
	{
	public:///Friendly Internal Forward Declarations
	
		friend class MeridianEngine;

	public:///Constructor/Destructor

		IModule();
		virtual ~IModule();

	protected:///Member Functions

			/*Initialise this module and load anything it needs to run long-term.*/
		virtual void Initialise(MeridianEngine * p_engine) = 0;

			/*Update this module inside the engine's gameloop.*/
		virtual void Update(MeridianEngine * p_engine, const float & p_dt) = 0;

			/*Immediately render anything this module has to inside the OpenGL clear-swap space*/
		virtual void Render(MeridianEngine * p_engine) = 0;

			/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		virtual void Finalise(MeridianEngine * p_engine) = 0;
	};
}
