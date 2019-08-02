#ifndef __SVGFont__
#define __SVGFont__

/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "VGFont.h"

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/
class VGDevice;
// --------------------------------------------------------------
/** \brief a SVG font class.
*/
class SVGFont : public VGFont
{
	const VGFont *	fFont;
	VGDevice *	fDevice;

	public:
					 SVGFont(const char * name, int size, int properties=kFontNone);
		virtual		~SVGFont();

		virtual	const char *	GetName() const			{ return fFont->GetName(); }
		virtual	int 			GetSize() const			{ return fFont->GetSize(); }
		virtual	int				GetProperties() const	{ return fFont->GetProperties(); }

		virtual	void	GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const;
		virtual	void	GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const;
};

/*! @} */

#endif
