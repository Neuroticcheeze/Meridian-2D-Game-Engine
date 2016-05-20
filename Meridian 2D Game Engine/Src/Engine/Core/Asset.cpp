/*===================================================================
//				(Copyright, 2016) Jesse Farag.
//	Distributed under the Meridian Technologies License, Version 1.0.
//			(See included file MERIDIAN_LICENSE.txt)
-------------------------------------------------------------------
//
//Source File:	Asset.cpp
//
===================================================================*/

#include "Asset.hpp"

using namespace Meridian;

IAsset::IAsset(const byte & p_id) :
	m_id(p_id)
{

}

IAsset::~IAsset()
{

}

const byte & IAsset::ID() const
{
	return m_id;
}