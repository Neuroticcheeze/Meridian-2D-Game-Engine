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

void GraphicsManager::Initialise(MeridianEngine * p_engine)
{

}

void GraphicsManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
}

void GraphicsManager::Render(MeridianEngine * p_engine)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(p_engine->GetWindow());
}

void GraphicsManager::Finalise(MeridianEngine * p_engine)
{

}

GLuint GraphicsManager::CreateShader(const char ** p_source, const GLenum & p_type)
{
	return 0;
}

void GraphicsManager::DeleteShader(GLuint & p_handle)
{

}

GLuint GraphicsManager::CreateProgram(const vector<GLuint> & p_shaders)
{
	return 0;
}

void GraphicsManager::DeleteProgram(GLuint & p_handle)
{

}

GraphicsManager::FrameBufferObject GraphicsManager::CreateFrameBufferObject(const unsigned int & p_width, const unsigned int & p_height)
{
	FrameBufferObject fbo;
	auto & fbHandle = fbo.m_handle;
	fbo.m_dimensions = uvec2(p_width, p_height);

	Attachment colour0 = Attachment();
	auto & att_colour0 = colour0.m_handle;
	colour0.m_format = GL_RGBA8;
	colour0.m_type = Attachment::Type::ATT_COLOUR;

	Attachment __depth = Attachment();
	auto & att___depth = __depth.m_handle;
	__depth.m_format = GL_DEPTH_COMPONENT24;
	__depth.m_type = Attachment::Type::ATT_DEPTH;

	//RGBA8 RenderBuffer, 24 bit depth RenderBuffer, 256x256
	glGenFramebuffers(1, &fbHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fbHandle);

	//Create and attach a color buffer
	glGenRenderbuffers(1, &att_colour0);
	//We must bind color_rb before we call glRenderbufferStorage
	glBindRenderbuffer(GL_RENDERBUFFER, att_colour0);
	//The storage format is RGBA8
	glRenderbufferStorage(GL_RENDERBUFFER, colour0.m_format, p_width, p_height);
	//Attach color buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, att_colour0);
	//-------------------------

	//Create and attach a depth buffer
	glGenRenderbuffers(1, &att___depth);
	glBindRenderbuffer(GL_RENDERBUFFER, att___depth);
	glRenderbufferStorage(GL_RENDERBUFFER, __depth.m_format, p_width, p_height);
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, att___depth);
	//-------------------------

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	fbo.m_attachments.push_back(colour0);
	fbo.m_attachments.push_back(__depth);

	return fbo;
}

void GraphicsManager::BeginFrameBufferObject(const FrameBufferObject & p_handle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, p_handle.m_handle);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//-------------------------
	glViewport(0, 0, p_handle.m_dimensions.x, p_handle.m_dimensions.y);
}

void GraphicsManager::DeleteFrameBufferObject(FrameBufferObject & p_handle)
{
	//Delete resources
	for (auto & attachment : p_handle.m_attachments)
	{
		glDeleteRenderbuffers(1, &attachment.m_handle);
	}

	//Bind 0, which means render to back buffer, as a result, fb is unbound
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &p_handle.m_handle);
}