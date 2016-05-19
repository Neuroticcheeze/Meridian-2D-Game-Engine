/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	AssetTexture.cpp
//
===================================================================*/

#include "AssetTexture.hpp"
#include "..\Core\Meridian.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Meridian;

const byte checker[12] = { 0, 0, 0, 255, 0, 255, 255, 0, 255, 0, 0, 0 };
const AssetTexture AssetTexture::MISSING_TEXTURE(2, 2, 3, checker);

AssetTexture::AssetTexture() : 
	AssetTexture(0, 0, 0, nullptr)
{

}

AssetTexture::AssetTexture(const AssetTexture & p_other) :
	AssetTexture(p_other.m_width, p_other.m_height, p_other.m_channels, p_other.m_pixelData)
{

}

AssetTexture::AssetTexture(const unsigned short & p_width, const unsigned short & p_height, const byte & p_channels, const byte * p_pixelData) :
	m_width(p_width),
	m_height(p_height),
	m_channels(p_channels)
{
	if (p_pixelData == nullptr)
		m_pixelData = nullptr;

	else
	{
		int size = p_channels * p_height * p_width;
		m_pixelData = static_cast<byte*>(malloc(size));
		memcpy_s(m_pixelData, size, p_pixelData, size);
	}
}

AssetTexture::~AssetTexture()
{
	free(m_pixelData);
	m_pixelData = nullptr;
	m_width = m_height = m_channels = 0;
}

#ifdef _DEBUG

void AssetTexture::Load(const RawProperty * p_resources)
{
	const char * path = p_resources[0].m_tag == RawProperty::STR ? p_resources[0].str : nullptr;

	int width, height, bitdepth;

	m_pixelData = stbi_load(path, &width, &height, &bitdepth, 0);

	if (m_pixelData == nullptr)
	{
		//TODO: error message
		*this = AssetTexture::MISSING_TEXTURE;
	}

	if (width > 0 && width < 4096 && height > 0 && height < 4096)
	{
		m_width = static_cast<unsigned short>(width);
		m_height = static_cast<unsigned short>(height);
		m_channels = static_cast<byte>(bitdepth);
	}

	else
	{
		stbi_image_free(m_pixelData);
	}
}

void AssetTexture::Encode(SerialBuffer & p_buffer) const
{
	int reqSize = 
		sizeof(m_width) +					//Width
		sizeof(m_height) +					//Height
		sizeof(m_channels) +				//Number of colour channels
		m_channels * m_width * m_height;	//Pixel data

	//Set buffer with size needed to store this texture.
	p_buffer.Reallocate(reqSize);

	memcpy_s(p_buffer.Data(0), sizeof(m_width), &m_width, sizeof(m_width));
	memcpy_s(p_buffer.Data(sizeof(m_width)), sizeof(m_height), &m_height, sizeof(m_height));
	memcpy_s(p_buffer.Data(sizeof(m_width) + sizeof(m_height)), sizeof(m_channels), &m_channels, sizeof(m_channels));
}

#endif

void AssetTexture::Decode(const SerialBuffer & p_buffer)
{
	int reqSize =
		sizeof(m_width) +					//Width
		sizeof(m_height) +					//Height
		sizeof(m_channels) +				//Number of colour channels
		m_channels * m_width * m_height;	//Pixel data
	
	assert(p_buffer.Size() == reqSize);

	memcpy_s(&m_width, sizeof(m_width), p_buffer.C_Data(0), sizeof(m_width));
	memcpy_s(&m_height, sizeof(m_height), p_buffer.C_Data(sizeof(m_width)), sizeof(m_height));
	memcpy_s(&m_channels, sizeof(m_channels), p_buffer.C_Data(sizeof(m_width) + sizeof(m_height)), sizeof(m_channels));
}

SerialBuffer::SerialBuffer() :
	m_buffer(nullptr),
	m_size(0)
{

}

SerialBuffer::SerialBuffer(const SerialBuffer & p_other)
{
	Reallocate(p_other.m_size);
	memcpy_s(m_buffer, m_size, p_other.m_buffer, m_size);
}

SerialBuffer::~SerialBuffer()
{
	free(m_buffer);
	m_buffer = nullptr;
}