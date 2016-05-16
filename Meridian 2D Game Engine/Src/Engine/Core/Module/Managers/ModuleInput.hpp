/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleInput.hpp
//
//Classes:		InputManager
//					-Implements IModule
//
===================================================================*/

#pragma once

#include "..\Module.hpp"

namespace Meridian
{
	/*==============================================================================================================
	Responsible for listening to user input and providing a clear and detailed interface to the rest of the engine.
	==============================================================================================================*/
	class InputManager : public IModule
	{
	public:///Constructors/Destructors

		InputManager();
		~InputManager();

	public:///Member Functions

		/*Initialise this module and load anything it needs to run long-term.*/
		void Initialise();

		/*Update this module inside the engine's gameloop.*/
		void Update(const float & p_dt);

		/*Immediately render anything this module has to inside the OpenGL clear-swap space*/
		void Render();

		/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		void Finalise();
		
	private:
	};
}