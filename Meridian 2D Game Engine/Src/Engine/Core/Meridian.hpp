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

//TODO: Remove?
#define REQUIRE_MODULE(module0) __if_not_exists(module0) { }


#include <vector>
using std::vector;
using std::is_base_of;

namespace Meridian
{
	//External Forward Declarations
	class IModule;

	/*====================================================================================================================
	The engine object itself; responsible for initialising all modules, running the game loop, and releasing all modules.
	====================================================================================================================*/
	class MeridianEngine
	{
	public:///Internal Forward Delarations

		enum class GameLoopMode;

	public:///Constructors/Destructors

		MeridianEngine();
		virtual ~MeridianEngine();

	public:///Member Functions

			/*Start up the engine which will start itself up and all modules in the order that 
			they were added.*/
		void Load();

			/*Run the engine's internal gameloop which will take care of updating and rendering 
			each module.*/
		void Run(const GameLoopMode & p_mode);

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
		bool m_isRunning, m_isInitialised;
		vector<IModule *> m_modules;
	};

	/*====================================================================================================================
	Game loop modes to control how the gameloop works (VARIABLE is always the best one to use).
	//	*- UNBOUND		(Game will run as fast as the computer can, there is no time synchronisation)
	//	*- FIXED		(Game will run at a target framerate and sleep for a fixed amount, deltatime is always 1000/target)
	//	*- VARIABLE		(Game will attempt to run at a target framerate. If the FPS drops below the target, deltatime is used, otherwise the game will wait out the rest of a frame if it finishes early)
	====================================================================================================================*/
	enum class MeridianEngine::GameLoopMode
	{
			/*Game will run as fast as the computer can, 
			there is no time synchronisation*/
		UNBOUND,

			/*Game will run at a target framerate and sleep 
			for a fixed amount, deltatime is always 
			1000/target*/
		FIXED,

			/*Game will attempt to run at a target framerate. 
			If the FPS drops below the target, deltatime is 
			used, otherwise the game will wait out the rest of 
			a frame if it finishes early to match the target*/
		VARIABLE
	};
}