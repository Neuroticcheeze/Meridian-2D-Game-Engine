/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	ModuleGraphics.cpp
//
===================================================================*/

#include "ModuleGraphics.hpp"
#include "..\Core\Meridian.hpp"
#include <assert.h>
#include <gl_core_4_4.h>
#include <glfw3.h>

using namespace Meridian;

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{

}

GLuint program;
GLuint vao, count;
void GraphicsManager::Initialise(MeridianEngine * p_engine)
{
	GLuint vertShader = CreateShader(
		"#version 410\n"
		""
		"layout (location = 0) in vec2 lPosition;"
		""
		"void main()"
		"{"
		"	gl_Position = vec4(lPosition, -1.0, 1.0);"
		"}"
		, GL_VERTEX_SHADER);

	GLuint fragShader = CreateShader(
		"#version 410\n"
		""
		"out vec4 oColour;"
		""
		"void main()"
		"{"
		"	oColour = vec4(1.0, 1.0, 0.0, 1.0);"
		"}"
		, GL_FRAGMENT_SHADER);

	program = CreateProgram({ vertShader, fragShader });
	VertexArrayObject _vao = CreateVertexArrayObject();
	vao = _vao.m_vao;
	count = _vao.m_indexCount;
}

void GraphicsManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
}

void GraphicsManager::Render(MeridianEngine * p_engine)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 100, 100);
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(p_engine->GetWindow());
}

void GraphicsManager::Finalise(MeridianEngine * p_engine)
{

}

GLuint GraphicsManager::CreateShader(const char * p_source, const GLenum & p_type)
{
	GLuint shader = glCreateShader(p_type);

	// Get strings for glShaderSource.
	glShaderSource(shader, 1, &p_source, NULL);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		printf("Shader failed to compile: %s\n", &errorLog[0]);

		// Exit with failure.
		glDeleteShader(shader); // Don't leak the shader.

		return 0;
	}

	return shader;
	// Shader compilation is successful.
}

void GraphicsManager::DeleteShader(GLuint & p_handle)
{
	glDeleteShader(p_handle);
}

GLuint GraphicsManager::CreateProgram(const vector<GLuint> & p_shaders)
{
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	GLuint program = glCreateProgram();

	//Attach our shaders to our program
	for (const GLuint & shader : p_shaders)
	{
		glAttachShader(program, shader);
	}

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(program);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		return 0;
	}

	//Always detach shaders after a successful link.
	for (const GLuint & shader : p_shaders)
	{
		glDetachShader(program, shader);
	}

	return program;
}

void GraphicsManager::DeleteProgram(GLuint & p_handle)
{
	glDeleteProgram(p_handle);
}

GraphicsManager::FrameBufferObject GraphicsManager::CreateFrameBufferObject(
	const unsigned int & p_width, 
	const unsigned int & p_height, 
	const vector<GraphicsManager::Attachment> & p_attachments, 
	const GraphicsManager::Attachment * p_depthAttachment)
{
	//Make sure the size of the fbo is at least 1 pixel, and there is at least a single colour attachment.
	assert(p_width * p_height * p_attachments.size() > 0);

	FrameBufferObject fbo;
	auto & fbHandle = fbo.m_handle;
	fbo.m_dimensions = uvec2(p_width, p_height);
	fbo.m_attachments = p_attachments;

	//RGBA8 RenderBuffer, 24 bit depth RenderBuffer, 256x256
	glGenFramebuffers(1, &fbHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fbHandle);



	GLint maxDrawBuf = 0;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuf);
	assert(maxDrawBuf >= static_cast<int>(fbo.m_attachments.size()));

	//Create colour attachments on the video card.
	int j = -1;
	for (Attachment & att : fbo.m_attachments)
	{
		//No depth attachments allowed here.
		assert(att.m_type != Attachment::Type::ATT_DEPTH);

		//Create and attach a color buffer
		glGenRenderbuffers(1, &att.m_handle);
		//We must bind color_rb before we call glRenderbufferStorage
		glBindRenderbuffer(GL_RENDERBUFFER, att.m_handle);
		//The storage format is RGBA8
		glRenderbufferStorage(GL_RENDERBUFFER, att.m_format, p_width, p_height);
		//Attach color buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ++j, GL_RENDERBUFFER, att.m_handle);
		//-------------------------
	}

	//If the suer wants a depth buffer attached...
	if (fbo.m_hasDepthBuffer = p_depthAttachment != nullptr)
	{
		Attachment __depth = Attachment(*p_depthAttachment);
		auto & att___depth = __depth.m_handle;
		
		assert(__depth.m_type == Attachment::Type::ATT_DEPTH);

		//Create and attach a depth buffer
		glGenRenderbuffers(1, &att___depth);
		glBindRenderbuffer(GL_RENDERBUFFER, att___depth);
		glRenderbufferStorage(GL_RENDERBUFFER, __depth.m_format, p_width, p_height);
		//-------------------------
		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, att___depth);
		//-------------------------

		fbo.m_attachments.push_back(__depth);
	}

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return fbo;
}

void GraphicsManager::ResizeFrameBufferObject(
	const unsigned int & p_width,
	const unsigned int & p_height,
	FrameBufferObject & p_handle)
{
	Attachment * depth = nullptr;

	if (p_handle.m_hasDepthBuffer)
	{
		depth = &p_handle.m_attachments.back();
		p_handle.m_attachments.pop_back();
	}

	DeleteFrameBufferObject(p_handle, true);
	p_handle = CreateFrameBufferObject(p_width, p_height, p_handle.m_attachments, depth);
}

void GraphicsManager::BeginFrameBufferObject(const FrameBufferObject & p_handle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, p_handle.m_handle);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT * static_cast<int>(p_handle.m_hasDepthBuffer));//Clears depth only if there's a depth buffer attached.
	//-------------------------
	glViewport(0, 0, p_handle.m_dimensions.x, p_handle.m_dimensions.y);
}

void GraphicsManager::EndFrameBufferObject()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsManager::DeleteFrameBufferObject(FrameBufferObject & p_handle, const bool & p_keepShell)
{
	//Delete resources
	for (auto & attachment : p_handle.m_attachments)
	{
		glDeleteRenderbuffers(1, &attachment.m_handle);
	}

	//Bind 0, which means render to back buffer, as a result, fb is unbound
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &p_handle.m_handle);

	if (!p_keepShell)
	{
		p_handle.m_attachments.clear();
		p_handle.m_dimensions = uvec2(0);
		p_handle.m_handle = 0;
		p_handle.m_hasDepthBuffer = false;
	}
}

GraphicsManager::VertexArrayObject GraphicsManager::CreateVertexArrayObject()
{
	vector<float> vertices = 
	{
		-1, -1,
		1, -1,
		1, 1
		-1, 1
	};

	vector<unsigned int> indices =
	{
		0, 3, 2, 
		0, 2, 1
	};

	VertexArrayObject vao;

	glGenVertexArrays(1, &vao.m_vao); // Create our Vertex Array Object  
	glBindVertexArray(vao.m_vao); // Bind our Vertex Array Object so we can use it  

	glGenBuffers(1, &vao.m_vbo); // Generate our Vertex Buffer Object  
	glBindBuffer(GL_ARRAY_BUFFER, vao.m_vbo); // Bind our Vertex Buffer Object  
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW  


	//Create the IBO for the triangle
	//16 bit indices
	//We could have actually made one big IBO for both the quad and triangle.
	glGenBuffers(1, &vao.m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer  

	glBindVertexArray(0); // Disable our Vertex Buffer Object

	return vao;
}