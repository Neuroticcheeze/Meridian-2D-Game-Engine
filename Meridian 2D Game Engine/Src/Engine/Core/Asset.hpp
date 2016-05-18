/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	Asset.hpp
//
//Objects:		IAsset
//					-Load
//					-Encode
//					-Decode
//
//				SerialBuffer
===================================================================*/

#pragma once

typedef unsigned char byte;

#include <assert.h>

namespace Meridian
{
	//External Forward Declarations
	class SerialBuffer;

	/*========================================================================
	Base structure for an intermediate object between the game and saved data.
	==========================================================================*/
	class IAsset
	{
	public:///Friendly Internal Forward Declarations

		friend class ResourceManager;

	protected:///Constructor/Destructor

		IAsset();
		virtual ~IAsset();

	protected:///Member Functions

#ifdef _DEBUG
			/*Used in debug mode to read in raw resources (.png, .xml, .mp3, etc), and fill this 
			asset's members just like Decode does (instead from the resource file). Once a resource 
			file has been generated during development, this asset will get its data from there instead.*/
		virtual void Load(const char ** p_resourcePaths) = 0;

			/*Used in debug mode to serialise its data into a buffer to be written to the game's 
			single resource file to be read in release build by Decode. Since the resource file is 
			never changed in release mode, Encode is not needed.*/
		virtual void Encode(SerialBuffer & p_buffer) const = 0;
#endif
			/*This function is always used to read from the resource file when the game starts up.*/
		virtual void Decode(const SerialBuffer & p_buffer) = 0;
	};


	/*==================================================================
	An object to store a stream of bytes and how many bytes it contains. 
	This object trusts that you do not mess around with its data by 
	deleting/reassigning "buffer".
	==================================================================*/
	class SerialBuffer
	{
		byte * m_buffer;
		unsigned int m_size;

	public:

		SerialBuffer();
		SerialBuffer(const SerialBuffer & p_other);
		~SerialBuffer();

		inline const int & Size() const
		{
			return m_size;
		}

		inline byte * Data(const unsigned int & p_offset = 0)
		{
			if (m_size == 0)
				return nullptr;

			assert(p_offset < m_size);

			return m_buffer + p_offset;
		}

		inline const byte * C_Data(const unsigned int & p_offset = 0) const
		{
			if (m_size == 0)
				return nullptr;

			assert(p_offset < m_size);

			return m_buffer + p_offset;
		}

		void Reallocate(const unsigned int & p_requiredSize)
		{
			free(m_buffer);

			m_buffer = p_requiredSize > 0 ? 
				static_cast<byte*>(malloc(m_size = p_requiredSize)) : 
				nullptr;
		}
	};
}
