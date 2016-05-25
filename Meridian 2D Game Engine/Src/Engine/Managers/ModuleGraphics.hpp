/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	ModuleGraphics.hpp
//
//Objects:		GraphicsManager
//					-CreateShader
//					-DeleteShader
//					-CreateProgram
//					-DeleteProgram
//					-CreateFrameBufferObject
//					-BeginrameBufferObject
//					-DeleteFrameBufferObject
//
//				Attachment
//
//				FrameBufferObject
//
//				Type (enum)
//
===================================================================*/

//TODO: Add interface with mouse and keyboard. Eventually for handheld devices, etc.

#pragma once

#include <glm/vec2.hpp>
using glm::uvec2;

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace Meridian
{
	/*=============================================================================
	Responsible for coordinating how everything from text to lighting functions in 
	Meridian, as well as providing the ability to tailor the system to your game.
	=============================================================================*/
	class GraphicsManager
	{
	private:///Internal Forward Declarations

		struct Attachment;
		struct FrameBufferObject;

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

	public:///Graphics Utilities

	private:///Private Graphics Utilities

		GLuint CreateShader(const char ** p_source, const GLenum & p_type);
		void DeleteShader(GLuint & p_handle);

		GLuint CreateProgram(const vector<GLuint> & p_shaders);
		void DeleteProgram(GLuint & p_handle);

		FrameBufferObject CreateFrameBufferObject(const unsigned int & p_width, const unsigned int & p_height);
		void BeginFrameBufferObject(const FrameBufferObject & p_handle);
		void DeleteFrameBufferObject(FrameBufferObject & p_handle);

	private:///Member Fields
	};

	/*=============================================================================
	A wrapper around an OpenGL FBO-Attachment. This stores what type of attachment 
	this is, along with the handle given to it by ogl, as well as the format it 
	abides by.
	=============================================================================*/
	struct GraphicsManager::Attachment
	{
		enum class Type;

		GLuint
			m_handle,
			m_format;
		Type
			m_type;
	};

	/*=============================================================================
	A wrapper to store handles to an OpenGL FBO. It stores such things as the FBO
	handle, attachments, and the size of the fbo; (By which all attachments are 
	indiscriminantly sized to).
	=============================================================================*/
	struct GraphicsManager::FrameBufferObject
	{
		vector<GraphicsManager::Attachment> m_attachments;
		GLuint m_handle;
		uvec2 m_dimensions;
	};

	/*=============================================================================
	The type of attachment appended to an FBO. Either a colour or depth attachment.
	=============================================================================*/
	enum class GraphicsManager::Attachment::Type
	{
		ATT_COLOUR,
		ATT_DEPTH
	};
}