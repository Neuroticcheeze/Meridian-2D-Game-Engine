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
#include "..\Assets\AssetTexture.hpp"
#include <assert.h>

using namespace Meridian;

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{

}

GLuint program, wobbleProgram;
GraphicsManager::UniformDirectory dir;
GraphicsManager::VertexArrayObject vao;
GraphicsManager::FrameBufferObject fbo;
AssetTexture* t;
#include "ModuleResource.hpp"
#include <glm\common.hpp>
using glm::vec2;
using glm::vec3;
void GraphicsManager::Initialise(MeridianEngine * p_engine)
{
	{
		GLuint vertShader = CreateShader(
			"#version 410\n"
			""
			"layout (location = 0) in vec2 lPosition;"
			"layout (location = 1) in vec3 lColour;"
			"layout (location = 2) in vec2 lTexCoords;"
			""
			"out vec3 ioColour;"
			"out vec2 ioTexCoords;"
			""
			"void main()"
			"{"
			"	ioColour = lColour;"
			"	ioTexCoords = lTexCoords;"
			"	gl_Position = vec4(lPosition * 0.5, 0.0, 1.0);"
			"}"
			, GL_VERTEX_SHADER);

		GLuint fragShader = CreateShader(
			"#version 410\n"
			""
			"uniform sampler2D uTexture;"
			""
			"out vec4 oColour;"
			"in vec2 ioTexCoords;"
			""
			"void main()"
			"{"
			"	oColour = texture(uTexture, ioTexCoords);"
			"}"
			, GL_FRAGMENT_SHADER);

		program = CreateProgram({ vertShader, fragShader });
	}

	{
		GLuint vertShader = CreateShader(
			"#version 410\n"
			""
			"layout (location = 0) in vec2 lPosition;"
			"layout (location = 1) in vec3 lColour;"
			"layout (location = 2) in vec2 lTexCoords;"
			""
			"out vec3 ioColour;"
			"out vec2 ioTexCoords;"
			""
			"void main()"
			"{"
			"	ioColour = lColour;"
			"	ioTexCoords = lTexCoords;"
			"	gl_Position = vec4(lPosition * vec2(1, -1), 0.0, 1.0);"
			"}"
			, GL_VERTEX_SHADER);

		GLuint fragShader = CreateShader(
			"#version 410\n"
			""
			"uniform sampler2D uTexture;"
			"uniform float wobbleTime;"
			""
			"in vec3 ioColour;"
			"in vec2 ioTexCoords;"
			""
			"out vec4 oColour;"
			""
			"void main()"
			"{"
			"	float time = sin(wobbleTime * 2 + ioTexCoords.x * 20);"
			"	oColour = (texture(uTexture, ioTexCoords + vec2(0.02, 0) + vec2(0, time * 0.05)) + texture(uTexture, ioTexCoords + vec2(-0.02, 0) + vec2(0, time * 0.05)) + texture(uTexture, ioTexCoords + vec2(0, 0.02) + vec2(0, time * 0.05)) + texture(uTexture, ioTexCoords + vec2(0, -0.02) + vec2(0, time * 0.05)) + texture(uTexture, ioTexCoords + vec2(0, time * 0.05))) / 5;"
			"}"
			, GL_FRAGMENT_SHADER);

		wobbleProgram = CreateProgram({ vertShader, fragShader });
	}

	struct Vert
	{
		Vert(vec2 _x, vec3 _y, vec2 _z) : x(_x), y(_y), z(_z) {}
		vec2 x;
		vec3 y;
		vec2 z;
	};
	vao = CreateVertexArrayObject<Vert>(
	{
		Vert(vec2(-1, -1), vec3(1,0,0), vec2(0, 0)),
		Vert(vec2(-1, +1), vec3(1,1,0), vec2(0, 1)),
		Vert(vec2(+1, +1), vec3(0,0,1), vec2(1, 1)),
		Vert(vec2(+1, -1), vec3(0.7F,0,1), vec2(1, 0)),
	}, 
	{
		0, 1, 2, 
		0, 2, 3,
	}, 
	{
		Attribute::Create(2, GL_FLOAT),
		Attribute::Create(3, GL_FLOAT),
		Attribute::Create(2, GL_FLOAT),
	});
	
	fbo = CreateFrameBufferObject(640, 480, 
	{
		Attachment::CreateColour(GL_RGB8, GL_CLAMP_TO_EDGE, GL_LINEAR)
	}, 
	&Attachment::CreateDepth(GL_DEPTH_COMPONENT24));

	auto resourceManager = p_engine->GetResourceManager();
	resourceManager->CreateAsset<Meridian::AssetTexture>("wup", resourceManager->GetPath("tree.jpg"));
	resourceManager->SaveResources();
	resourceManager->LoadResources();
	resourceManager->GetAsset("wup", &t);
	CreateTexture(*t, GL_NEAREST, GL_REPEAT);

	dir = GenerateProgramUniformDirectory(wobbleProgram, {"wobbleTime"});
}

void GraphicsManager::Update(MeridianEngine * p_engine, const float & p_dt)
{
}

void GraphicsManager::Render(MeridianEngine * p_engine)
{

	if (p_engine->ViewportWhatChanged() == Meridian::MeridianEngine::ViewChangeState::VIEWPORT_SIZE)
	{
		vec4 vp = p_engine->GetViewport();
		glViewport(0, 0, (unsigned int)vp.z, (unsigned int)vp.w);
		ResizeFrameBufferObject((unsigned int)vp.z, (unsigned int)vp.w, fbo);
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BeginFrameBufferObject(fbo);
	BindProgram(program);
	BindTexture(*t, 0);
	DrawVertexArrayObject(vao, GL_TRIANGLES);


	if (p_engine->ViewportWhatChanged() == Meridian::MeridianEngine::ViewChangeState::VIEWPORT_SIZE)
	{
		vec4 vp = p_engine->GetViewport();
		glViewport(0, 0, (unsigned int)vp.z, (unsigned int)vp.w);
	}
	EndFrameBufferObject();
	BindProgram(wobbleProgram);
	glUniform1f(dir.Get("wobbleTime"), (float)glfwGetTime());
	BindFrameBufferObjectTexture(fbo, { 0 });
	DrawVertexArrayObject(vao, GL_TRIANGLES);


	glfwSwapBuffers(p_engine->GetWindow());
}

void GraphicsManager::Finalise(MeridianEngine * p_engine)
{
	DeleteVertexArrayObject(vao);
	DeleteProgram(program);
	DeleteProgram(wobbleProgram);
	DeleteFrameBufferObject(fbo);
	DeleteTexture(*t);
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

GraphicsManager::UniformDirectory GraphicsManager::GenerateProgramUniformDirectory(GLuint & p_handle, const vector<const char*> & p_aliases)
{
	UniformDirectory ud;

	for (const char * alias : p_aliases)
	{
		ud.m_mapping.insert(pair<string, GLuint>(string(alias), glGetUniformLocation(p_handle, alias)));
	}

	return ud;
}

void GraphicsManager::BindProgram(GLuint & p_handle)
{
	glUseProgram(p_handle);
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
		assert(att.m_type != AttachmentType::ATT_DEPTH);

		////Create and attach a color buffer
		//glGenRenderbuffers(1, &att.m_handle);
		////We must bind color_rb before we call glRenderbufferStorage
		//glBindRenderbuffer(GL_RENDERBUFFER, att.m_handle);
		////The storage format is RGBA8
		//glRenderbufferStorage(GL_RENDERBUFFER, att.m_format, p_width, p_height);
		////Attach color buffer to FBO
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ++j, GL_RENDERBUFFER, att.m_handle);
		////-------------------------

		//***************************************************************************************************************************************************************************************************************************
		//																		ABOVE: RenderBufferVersion /\								BELOW: TextureVersion \/
		//***************************************************************************************************************************************************************************************************************************

		glGenTextures(1, &att.m_tHandle);
		glBindTexture(GL_TEXTURE_2D, att.m_tHandle);
		glTexStorage2D(GL_TEXTURE_2D, 1, att.m_format, p_width, p_height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, att.m_tFilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, att.m_tFilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, att.m_tWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, att.m_tWrapMode);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ++j, att.m_tHandle, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//If the suer wants a depth buffer attached...
	if (fbo.m_hasDepthBuffer = p_depthAttachment != nullptr)
	{
		Attachment __depth = Attachment(*p_depthAttachment);
		auto & att___depth = __depth.m_handle;
		
		assert(__depth.m_type == AttachmentType::ATT_DEPTH);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT * static_cast<int>(p_handle.m_hasDepthBuffer));//Clears depth only if there's a depth buffer attached.
}

void GraphicsManager::BindFrameBufferObjectTexture(const FrameBufferObject & p_handle, const vector<GLuint> & p_units)
{
	assert((p_handle.m_attachments.size() - static_cast<unsigned int>(p_handle.m_hasDepthBuffer)) <= p_units.size());

	int t = -1;

	for (auto & attachment : p_handle.m_attachments)
	{
		glActiveTexture(GL_TEXTURE0 + ++t);
		glBindTexture(GL_TEXTURE_2D, attachment.m_tHandle);
	}
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
		glDeleteTextures(1, &attachment.m_tHandle);
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

void GraphicsManager::DrawVertexArrayObject(const VertexArrayObject & p_handle, const GLenum & p_drawMode)
{
	glBindVertexArray(p_handle.m_vao);
	glDrawElements(p_drawMode, p_handle.m_indexCount, p_handle.m_indexType, 0);
}

void GraphicsManager::DeleteVertexArrayObject(const VertexArrayObject & p_handle)
{

}

void GraphicsManager::CreateTexture(AssetTexture & p_assetTexture, const GLenum & p_filterMode, const GLenum & p_wrapMode)
{
	GLuint format, internalFormat;

	assert(p_assetTexture.m_channels >= 1 && p_assetTexture.m_channels <= 4);

	switch (p_assetTexture.m_channels)
	{
	case 1:	
		format = GL_RGB;
		internalFormat = GL_R8;
		break;
	case 2:	
		format = GL_RGB;
		internalFormat = GL_RG8;
		break;
	case 3:
		format = GL_RGB;
		internalFormat = GL_RGB8;
		break;
	case 4:
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
		break;
	}

	glGenTextures(1, &p_assetTexture.m_glTextureHandle);
	glBindTexture(GL_TEXTURE_2D, p_assetTexture.m_glTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, p_assetTexture.m_width, p_assetTexture.m_height, 0, format, GL_UNSIGNED_BYTE, p_assetTexture.m_pixelData.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_wrapMode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GraphicsManager::BindTexture(const AssetTexture & p_assetTexture, const GLuint & p_textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + p_textureUnit);
	glBindTexture(GL_TEXTURE_2D, p_assetTexture.m_glTextureHandle);
}

void GraphicsManager::DeleteTexture(AssetTexture & p_assetTexture)
{
	glDeleteTextures(1, &p_assetTexture.m_glTextureHandle);
}