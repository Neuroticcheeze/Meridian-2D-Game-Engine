/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleAudio.hpp
//
//Objects:		AudioManager
//					-Initialise
//					-Update
//					-Finalise
//
===================================================================*/

#pragma once

namespace Meridian
{
	/*=============================================================================
	Responsible for coordinating audio. Allowing the user to play sounds or music
	in global, or perspetive mode.
	=============================================================================*/
	class AudioManager
	{
	public:///Internal Forward Declarations

	public:///Friendly Internal Forward Declarations

		friend class MeridianEngine;

	private:///Constructors/Destructors

		AudioManager();
		~AudioManager();

	private:///Member Functions

		/*Initialise this module and load anything it needs to run long-term.*/
		void Initialise(MeridianEngine * p_engine);

		/*Update this module inside the engine's gameloop.*/
		void Update(MeridianEngine * p_engine, const float & p_dt);

		/*Finalise this module and unload/delete anything it loaded during the initialise state.*/
		void Finalise(MeridianEngine * p_engine);

	public:///Audio Utilities

	private:///Member Fields

	};
}