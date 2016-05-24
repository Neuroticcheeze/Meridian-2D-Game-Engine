/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-----------------------------------------------------------------s--
//
//Header File:	Meridian.hpp
//
//Objects:		MeridianEngine
//					-Load
//					-Run
//					-Unload
//					-Terminate
//					-GetInputManager
//					-GetResourceManager
//					-GetGraphicsManager
//					-GetWindow
//					-HookToPreEvent
//					-HookToLoopEvent
//					-HookToPostEvent
//
===================================================================*/

#pragma once

//TODO: Remove?
#define REQUIRE_MODULE(module0) __if_not_exists(module0) { }


#include <vector>
using std::vector;
using std::is_base_of;

//=============== Third-Party Declarations ===============

struct GLFWwindow;

//========================================================

namespace Meridian
{
	//External Forward Declarations
	class InputManager;
	class ResourceManager;
	class GraphicsManager;

	class MeridianEngine;

	//Engine Event Callbacks
	typedef void(*OnEnginePre)(MeridianEngine * p_engine);
	typedef void(*OnEngineLoop)(MeridianEngine * p_engine, const float & p_dt);
	typedef void(*OnEnginePost)(MeridianEngine * p_engine);

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

		inline InputManager * GetInputManager() { return m_inputManager; }
		inline ResourceManager * GetResourceManager() { return m_resourceManager; }
		inline GraphicsManager * GetGraphicsManager() { return m_graphicsManager; }

			/*Get a handle to the main game window.*/
		inline GLFWwindow * GetWindow()
		{
			return m_window;
		}
			/*Hook a callback function to the initialisation event inside the engine.*/
		void HookToPreEvent(OnEnginePre p_event);

			/*Hook a callback function to the game-loop update event inside the engine.*/
		void HookToLoopEvent(OnEngineLoop p_event);

			/*Hook a callback function to the finalisation event inside the engine.*/
		void HookToPostEvent(OnEnginePost p_event);


	private:///Member Fields

		///Modules
		InputManager * m_inputManager;
		ResourceManager * m_resourceManager;
		GraphicsManager * m_graphicsManager;

		bool m_isRunning, m_isInitialised;
		GLFWwindow * m_window;

		OnEnginePre m_preHook;
		OnEngineLoop m_loopHook;
		OnEnginePost m_postHook;
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