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
//				UniformDirectory
//					-Get
//
//				Attribute
//					-Create
//
===================================================================*/

#pragma once

#include <gl_core_4_4.h>
#include <glfw3.h>

#include <vector>
using std::vector;

#include <map>
using std::map;
using std::pair;

#include <string>
using std::string;

#include <glm/vec2.hpp>
using glm::uvec2;

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
		class UniformDirectory;
		struct Attribute;

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

		UniformDirectory GenerateProgramUniformDirectory(GLuint & p_handle, const vector<const char*> & p_aliases);

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

		/*Generate a vertex array object given a vertex buffer, an index buffer, and a list of attributes.*/
		template <typename T>
		VertexArrayObject CreateVertexArrayObject(const vector<T> p_vertices, const vector<GLubyte> & p_indices, const vector<Attribute> & p_attributes)
		{
			VertexArrayObject vao;

			vao.m_indexCount = p_indices.size();

			vao.m_indexType = GL_UNSIGNED_BYTE;

			glGenVertexArrays(1, &vao.m_vao); // Create our Vertex Array Object  
			glBindVertexArray(vao.m_vao); // Bind our Vertex Array Object so we can use it  

			glGenBuffers(1, &vao.m_vbo); // Generate our Vertex Buffer Object  
			glBindBuffer(GL_ARRAY_BUFFER, vao.m_vbo); // Bind our Vertex Buffer Object  
			glBufferData(GL_ARRAY_BUFFER, p_vertices.size() * sizeof(T), p_vertices.data(), GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW  
																														 //We could have actually made one big IBO for both the quad and triangle.
			glGenBuffers(1, &vao.m_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.m_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_indices.size() * sizeof(GLubyte), p_indices.data(), GL_STATIC_DRAW);

			glBindVertexBuffer(0, vao.m_vbo, 0, sizeof(T));

			int j = -1, offset = 0;
			for (auto & attr : p_attributes)
			{
				glEnableVertexAttribArray(++j);
				glVertexAttribFormat(j, attr.m_count, attr.m_type, attr.m_normalised, offset);
				glVertexAttribBinding(j, 0);

				unsigned int typesize = 0;

				switch (attr.m_type)
				{
				case GL_BOOL:			typesize = sizeof(char);	break;
				case GL_BYTE:			typesize = sizeof(char);	break;
				case GL_UNSIGNED_BYTE:	typesize = sizeof(char);	break;
				case GL_SHORT:			typesize = sizeof(short);	break;
				case GL_UNSIGNED_SHORT: typesize = sizeof(short);	break;
				case GL_INT:			typesize = sizeof(int);		break;
				case GL_UNSIGNED_INT:	typesize = sizeof(int);		break;
				case GL_FLOAT:			typesize = sizeof(float);	break;
				default: assert(false);
				}

				offset += attr.m_count * typesize;
			}

			glBindVertexArray(0);

			return vao;
		}


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
		/*Create an attachment to be used for a framebuffer object 
		of type colour.*/
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

		/*Create an attachment to be used for a framebuffer object 
		of type depth. Only one of these can exist per frame buffer 
		object.*/
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
	A wrapper to store handles to an OpenGL VAO, VBO, and IBO. It 
	represents a mesh or model used by ogl.
	===============================================================*/
	struct GraphicsManager::VertexArrayObject
	{
		GLuint m_vao, m_vbo, m_ibo, m_indexCount, m_indexType;
	};

	/*===================================================================
	An object to store mappings between handles to uniform locations for 
	the specified shader program, alongside their human-friendly names.
	===================================================================*/
	class GraphicsManager::UniformDirectory
	{
	private:///Friendly Classes

		friend class GraphicsManager;

	public:

		/*Get the uniform location paired to the specified alias, or 0 if none exists.*/
		inline GLint Get(const char * p_alias) const
		{
			auto it = m_mapping.find(string(p_alias));

			return it == m_mapping.end() ? -1 : it->second;
		}

	private:

		map<string, GLint> m_mapping;
	};


	/*===============================================================
	An object to store the information needed to define a new attribute
	for a vertex structure inside a vertex buffer object; telling OpenGL
	how each part of the vertex is laid out inside video memory.
	===============================================================*/
	struct GraphicsManager::Attribute
	{
		static Attribute Create(const unsigned int & p_count, const GLenum & p_type, const bool & p_normalised = false)
		{
			Attribute attr;

			attr.m_count = p_count;
			attr.m_type = p_type;
			attr.m_normalised = p_normalised;

			return attr;
		}

		unsigned int m_count;
		GLenum m_type;
		bool m_normalised;
	};
}