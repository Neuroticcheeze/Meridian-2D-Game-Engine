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
//					-Size
//					-Data
//					-C_Data
//					-Reallocate
//
//				RawProperty
//
===================================================================*/

#pragma once

typedef unsigned char byte;

#include <assert.h>
#include <Windows.h>

namespace Meridian
{
	//External Forward Declarations
	class SerialBuffer;
	struct RawProperty;

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
			/*Used in debug mode to read in raw resources (.png, .xml, .mp3, etc), including information 
			about width, size, etc. It then fill this asset's members just like Decode does (instead from 
			the resource file). Once a resource file has been generated during development, this asset 
			will get its data from there instead.*/
		virtual void Load(RawProperty * p_resources) = 0;

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
		
	public:///Constructors/Destructor

		SerialBuffer();
		SerialBuffer(const SerialBuffer & p_other);
		~SerialBuffer();

	public:///Member Functions

			/*Get the size of this buffer in bytes.*/
		inline const int & Size() const
		{
			return m_size;
		}

			/*Get a ptr to the data contained in this buffer with the 
			specified offset. The offset must be inside the size of the buffer.*/
		inline byte * Data(const unsigned int & p_offset = 0)
		{
			if (m_size == 0)
				return nullptr;

			assert(p_offset < m_size);

			return m_buffer + p_offset;
		}

			/*Get a const ptr to the data contained in this buffer with the 
			specified offset. The offset must be inside the size of the buffer.
			This is a read-only equivalent of Data.*/
		inline const byte * C_Data(const unsigned int & p_offset = 0) const
		{
			if (m_size == 0)
				return nullptr;

			assert(p_offset < m_size);

			return m_buffer + p_offset;
		}

			/*Reallocate this buffer, effectively deleting any previous 
			data and resizing the buffer to the specified required size.*/
		void Reallocate(const unsigned int & p_requiredSize)
		{
			free(m_buffer);

			m_buffer = p_requiredSize > 0 ? 
				static_cast<byte*>(malloc(m_size = p_requiredSize)) : 
				nullptr;
		}

	private:///Member Fields

		byte * m_buffer;
		unsigned int m_size;
	};

	/*==================================================================
	Used as a piece of information, whether a filepath, a number, colour
	format, to define the asset it's used for.
	==================================================================*/
	struct RawProperty
	{
		enum { C_STR, CHAR, INT, FLOAT, U_INT, SHORT, U_SHORT, LONG, U_LONG } m_tag;
		union
		{
			const char* c_str;
			char c;
			int i;
			unsigned int u_i;
			short s;
			unsigned short u_s;
			long l;
			unsigned long u_l;
		};
	};
}
