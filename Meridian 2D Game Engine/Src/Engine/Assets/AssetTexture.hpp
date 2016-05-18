/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Header File:	AssetTexture.hpp
//
//Objects:		AssetTexture
//					-Implements IAsset
//
===================================================================*/

//TODO: Add interface with mouse and keyboard. Eventually for handheld devices, etc.

#pragma once

#include "..\Core\Asset.hpp"

namespace Meridian
{
	/*=============================================================================
	Asset for textures that can be drawn with versatile properties such as position,
	scales, rotation, tint, transparency, etc. Or used in many other ways.
	=============================================================================*/
	class AssetTexture : public IAsset
	{
	public:///Friendly Internal Forward Declarations

		friend class ResourceManager;

	private:///Constructors/Destructors

		AssetTexture();
		~AssetTexture();

#ifdef _DEBUG
		/*Used in debug mode to read in raw resources (.png, .xml, .mp3, etc), including information
		about width, size, etc. It then fill this asset's members just like Decode does (instead from
		the resource file). Once a resource file has been generated during development, this asset
		will get its data from there instead.*/
		void Load(RawProperty * p_resources);

		/*Used in debug mode to serialise its data into a buffer to be written to the game's
		single resource file to be read in release build by Decode. Since the resource file is
		never changed in release mode, Encode is not needed.*/
		void Encode(SerialBuffer & p_buffer) const;
#endif
		/*This function is always used to read from the resource file when the game starts up.*/
		void Decode(const SerialBuffer & p_buffer);
		
	private:///Member Fields

		unsigned short m_width;
		unsigned short m_height;
		byte m_channels;

	};
}