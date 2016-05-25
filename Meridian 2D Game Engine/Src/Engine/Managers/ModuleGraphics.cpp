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

GraphicsManager::FrameBufferObject GraphicsManager::CreateFrameBufferObject(const unsigned int & p_width, const unsigned int & p_height, const vector<GraphicsManager::Attachment> & p_attachments, const GraphicsManager::Attachment * p_depthAttachment)
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

void GraphicsManager::BeginFrameBufferObject(const FrameBufferObject & p_handle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, p_handle.m_handle);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT * static_cast<int>(p_handle.m_hasDepthBuffer));//Clears depth only if there's a depth buffer attached.
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