/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-----------------------------------------------------------------s--
//
//Header File:	Meridian.hpp
//
//Classes:		MeridianEngine
//					-Load
//					-Run
//					-Unload
//					-Terminate
//					-AddModule
//
===================================================================*/

#pragma once

#include <vector>
using std::vector;
using std::is_base_of;

namespace Meridian
{
	//Forward declarations
	class IModule;

	/*====================================================================================================================
	The engine object itself; responsible for initialising all modules, running the game loop, and releasing all modules.
	====================================================================================================================*/
	class MeridianEngine
	{
	public:///Constructors/Destructors

		MeridianEngine();
		virtual ~MeridianEngine();

	public:///Member Functions

			/*Start up the engine which will start itself up and all modules in the order that 
			they were added.*/
		void Load();

			/*Run the engine's internal gameloop which will take care of updating and rendering 
			each module.*/
		void Run();

			/*Close down the engine which will release any resources created (Different to allocated 
			memory, which is taken care of by the dtor) in Load and close all modules in the same order 
			that they were added.*/
		void Unload();

			/*Terminate the engine. This will stop the game loop, so that the program can progress and 
			hit Unload (Which should be written after Run)*/
		void Terminate();

			/*Add the module to the game engine to be processed and used. This module will perform actions 
			relative to others in the order which it was added to the engine.*/
		template <typename T>
		void AddModule()
		{
			static_assert(std::is_base_of<IModule, T>::value, "T must derive from IModule");

			T* module = new T;
			m_modules.push_back(module);
		}

	private:///Member Fields
		bool m_isRunning;
		vector<IModule *> m_modules;
	};
}