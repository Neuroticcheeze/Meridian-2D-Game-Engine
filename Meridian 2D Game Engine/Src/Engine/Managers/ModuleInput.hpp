/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleInput.hpp
//
//Objects:		InputManager
//					-GetMousePosition
//					-KeyState
//
===================================================================*/

//TODO: Add interface with mouse and keyboard. Eventually for handheld devices, etc.

#pragma once

#include <glm/vec2.hpp>
using glm::uvec2;

#define GLFW_MAX_KEYS 348 //GLFW has 348 keys

struct GLFWwindow;

namespace Meridian
{
	/*==============================================================================================================
	Responsible for listening to user input and providing a clear and detailed interface to the rest of the engine.
	==============================================================================================================*/
	class InputManager
	{
	public:///Internal Forward Declarations
		
		enum class KeyState;

	public:///Friendly Internal Forward Declarations

		friend class MeridianEngine;

	private:///Constructors/Destructors

		InputManager();
		~InputManager();

	private:///Member Functions

			/*Initialise this module and load anything it needs to run long-term.*/
		void Initialise(MeridianEngine * p_engine);

			/*Update this module inside the engine's gameloop.*/
		void Update(MeridianEngine * p_engine, const float & p_dt);

			/*Immediately render anything this module has to inside the OpenGL clear-swap space*/
		void Render(MeridianEngine * p_engine);

			/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		void Finalise(MeridianEngine * p_engine);

	public:///Input utilities

		const uvec2 & GetMousePosition() const;

		const KeyState & GetKeyState(const int & p_keycode) const;

	private:///Private Member Functions

		static void OnKey(GLFWwindow *, int, int, int, int);
		
	private:///Member Fields

		uvec2 m_mousePosition;

		static KeyState m_keyStates[GLFW_MAX_KEYS];
	};

	/*====================================================
	List of key states that a key can be in at any time.
	====================================================*/
	enum class InputManager::KeyState
	{
		//When the key is currently being held down.
		DOWN,

		//When the key doesn't currently have a finger on it.
		UP,

		//The short moment when a key goes from UP to DOWN.
		TAP,

		//The short moment when a key goes from DOWN to UP.
		RELEASE
	};
}