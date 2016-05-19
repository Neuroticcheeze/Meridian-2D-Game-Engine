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

using namespace Meridian;

AssetTexture::AssetTexture()
{
}

AssetTexture::~AssetTexture()
{

}

#ifdef _DEBUG

void AssetTexture::Load(RawProperty * p_resources)
{
	const char * path = p_resources[0].m_tag == RawProperty::STR ? p_resources[0].str : nullptr;


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