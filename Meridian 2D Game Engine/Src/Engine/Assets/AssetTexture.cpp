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

const AssetTexture AssetTexture::MISSING_TEXTURE(2, 2, 4, { 0, 0, 0, 255, 255, 0, 255, 255, 255, 0, 255, 255, 0, 0, 0, 255 });

AssetTexture::AssetTexture(const unsigned short & p_width, const unsigned short & p_height, const byte & p_channels, const vector<byte> p_pixelData) :

	IAsset(0),

	m_width(p_width),
	m_height(p_height),
	m_channels(p_channels),
	m_pixelData(p_pixelData),
	m_glTextureHandle(0)
{
}

AssetTexture::~AssetTexture()
{
	m_pixelData.clear();
	m_width = m_height = m_channels = 0;
}

#ifdef _DEBUG

void AssetTexture::Load(const RawProperty * p_resources)
{
	string path = p_resources[0].m_tag == RawProperty::STR ? p_resources[0].str : nullptr;

	int width, height, bitdepth;

	byte * data = stbi_load(path.c_str(), &width, &height, &bitdepth, 0);

	if (data == nullptr)
	{
		//TODO: error message
		*this = AssetTexture::MISSING_TEXTURE;
	}

	if (width > 0 && width < 4096 && height > 0 && height < 4096)
	{
		m_width = static_cast<unsigned short>(width);
		m_height = static_cast<unsigned short>(height);
		m_channels = static_cast<byte>(bitdepth);
		m_pixelData = vector<byte>(data, data + width * height * bitdepth);
	}

	else
	{
		stbi_image_free(data);
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

	memcpy_s(p_buffer.Data(), sizeof(m_width), &m_width, sizeof(m_width));
	memcpy_s(p_buffer.Data(sizeof(m_width)), sizeof(m_height), &m_height, sizeof(m_height));
	memcpy_s(p_buffer.Data(sizeof(m_width) + sizeof(m_height)), sizeof(m_channels), &m_channels, sizeof(m_channels));
	memcpy_s(p_buffer.Data(sizeof(m_width) + sizeof(m_height) + sizeof(m_channels)), m_channels * m_width * m_height, m_pixelData.data(), m_channels * m_width * m_height);
}

#endif

void AssetTexture::Decode(const SerialBuffer & p_buffer)
{
	int reqSize_header =
		sizeof(m_width) +					//Width
		sizeof(m_height) +					//Height
		sizeof(m_channels);					//Number of colour channels
	
	assert(p_buffer.Size() >= reqSize_header);
	
	memcpy_s(&m_width, sizeof(m_width), p_buffer.C_Data(0), sizeof(m_width));
	memcpy_s(&m_height, sizeof(m_height), p_buffer.C_Data(sizeof(m_width)), sizeof(m_height));
	memcpy_s(&m_channels, sizeof(m_channels), p_buffer.C_Data(sizeof(m_width) + sizeof(m_height)), sizeof(m_channels));

	int reqSize_data = m_channels * m_width * m_height; //Pixel data

	assert(p_buffer.Size() >= reqSize_data);

	m_pixelData.resize(reqSize_data);
	memcpy_s(m_pixelData.data(), reqSize_data, p_buffer.C_Data(sizeof(m_width) + sizeof(m_height) + sizeof(m_channels)), reqSize_data);
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