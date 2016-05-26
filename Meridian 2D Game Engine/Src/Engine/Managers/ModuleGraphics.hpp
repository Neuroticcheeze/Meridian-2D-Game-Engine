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
//					-BindProgram
//					-DeleteProgram
//					-CreateFrameBufferObject
//					-BeginFrameBufferObject
//					-BindFrameBufferObjectTexture
//					-EndFrameBufferObject
//					-DeleteFrameBufferObject
//					-CreateVertexBufferObject
//					-DrawVertexBufferObject
//					-DeleteVertexBufferObject
//					-CreateTexture
//					-BindTexture
//					-DeleteTexture
//
//				AttachmentType
//
//				Attachment
//					-CreateColour
//					-CreateDepth
//
//				FrameBufferObject
//
//				VertexBufferObject
//
===================================================================*/

#pragma once

#include <vector>
using std::vector;

#include <glm/vec2.hpp>
using glm::uvec2;

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace Meridian
{
	///External Forward Declarations
	class AssetTexture;

	/*=============================================================================
	Responsible for coordinating how everything from text to lighting functions in 
	Meridian, as well as providing the ability to tailor the system to your game.
	=============================================================================*/
	class GraphicsManager
	{
	public:///Internal Forward Declarations

		struct Attachment;
		struct FrameBufferObject;
		struct VertexArrayObject;

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

		/*Create a shader to later be linked to a program.*/
		GLuint CreateShader(const char * p_source, const GLenum & p_type);

		/*Delete and release the video memory holding the specified shader.*/
		void DeleteShader(GLuint & p_handle);

		/*Create a program to have shaders later linked to it.*/
		GLuint CreateProgram(const vector<GLuint> & p_shaders);

		/*Bind the program to be used.*/
		void BindProgram(GLuint & p_handle);//TODO: Add ability to give uniform locations and new values.

		/*Delete a shader program. This will not delete the individual 
		attached shaders as far as non-gl doc is concerned.*/
		void DeleteProgram(GLuint & p_handle);

		/*Create a framebuffer object given the width and height, at least one colour attachment, 
		and a possible depth attachment (Not required). Do note that all attachments in p_attachments
		must be of type COLOUR only.*/
		FrameBufferObject CreateFrameBufferObject(
			const unsigned int & p_width, 
			const unsigned int & p_height, 
			const vector<GraphicsManager::Attachment> & p_attachments, 
			const GraphicsManager::Attachment * p_depthAttachment = nullptr);

		/*Recreate this FBO with the new size.*/
		void ResizeFrameBufferObject(
			const unsigned int & p_width,
			const unsigned int & p_height, 
			FrameBufferObject & p_handle);

		/*Bind this FBO to be rendered to by any subsequent render calls. 
		This will also clear the buffer automatically.*/
		void BeginFrameBufferObject(const FrameBufferObject & p_handle);

		/*Bind this FBO to be used in rendering as a texture. It will map
		all attachments except for the depth attachment to the specified
		list of opengl texture units.*/
		void BindFrameBufferObjectTexture(const FrameBufferObject & p_handle, const vector<GLuint> & p_units);

		/*Go back to the screen buffer.*/
		void EndFrameBufferObject();

		/*Delete the frame buffer object and any attachments.*/
		void DeleteFrameBufferObject(FrameBufferObject & p_handle, const bool & p_keepShell = false); 

		/*Generate a vertex array object*/
		VertexArrayObject CreateVertexArrayObject();//TODO: Make this more customisable!

		/*Draw the specified vertex array object in the provided draw mode.*/
		void DrawVertexArrayObject(const VertexArrayObject & p_handle, const GLenum & p_drawMode);

		/*Delete the vertex array object from video memory.*/
		void DeleteVertexArrayObject(const VertexArrayObject & p_handle);

		/*Create a new texture from AssetTexture.*/
		void CreateTexture(AssetTexture & p_assetTexture, const GLenum & p_filterMode, const GLenum & p_wrapMode);

		/*Bind the specified AssetTexture to be used in rendering given a texture unit to be bound to starting from 0, not GL_TEXTURE0.*/
		void BindTexture(const AssetTexture & p_assetTexture, const GLuint & p_textureUnit);

		/*Delete the resources "owned" by AssetTexture from video memory.*/
		void DeleteTexture(AssetTexture & p_assetTexture);

	private:///Member Fields
	};

	/*=============================================================================
	The type of attachment appended to an FBO. Either a colour or depth attachment.
	=============================================================================*/
	enum class AttachmentType
	{
		ATT_COLOUR,
		ATT_DEPTH
	};

	/*=============================================================================
	A wrapper around an OpenGL FBO-Attachment. This stores what type of attachment 
	this is, along with the handle given to it by ogl, as well as the format it 
	abides by.
	=============================================================================*/
	struct GraphicsManager::Attachment
	{
		static Attachment CreateColour(const GLuint & p_format, const GLuint & p_wrapMode, const GLuint & p_filterMode)
		{
			Attachment att;

			att.m_handle		= 0;
			att.m_format		= p_format;
			att.m_tHandle		= 0;
			att.m_tWrapMode		= p_wrapMode;
			att.m_tFilterMode	= p_filterMode;
			att.m_type			= AttachmentType::ATT_COLOUR;

			return att;
		}

		static Attachment CreateDepth(const GLuint & p_format)
		{
			Attachment att;

			att.m_handle = 0;
			att.m_format = p_format;
			att.m_type = AttachmentType::ATT_DEPTH;

			return att;
		}

		GLuint
			m_handle,
			m_format,
			m_tHandle,
			m_tWrapMode,
			m_tFilterMode;
		AttachmentType
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
		bool m_hasDepthBuffer;
	};

	/*===============================================================
	A weapper to store handles to an OpenGL VAO, VBO, and IBO. It 
	represents a mesh or model used by ogl.
	===============================================================*/
	struct GraphicsManager::VertexArrayObject
	{
		GLuint m_vao, m_vbo, m_ibo, m_indexCount, m_indexType;
	};
}