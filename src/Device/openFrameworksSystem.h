/*
	GUIDO Library
	Copyright (C) 2012 Grame
 	Updated by Dan Wilcox (C) ZKM | Hertz-Lab 2019

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.
*/
#pragma once

#include "VGSystem.h"

// --------------------------------------------------------------
// 		openFrameworks implementation of the VGSystem class
// --------------------------------------------------------------
class openFrameworksSystem : public VGSystem
{
	public:
		openFrameworksSystem() {}
		virtual	~openFrameworksSystem() {}

		// - VGDevice services -------------------------------------------------
		virtual VGDevice*		CreateDisplayDevice();
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice();
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );

		// - Font services -----------------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;
};
