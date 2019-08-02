/*
	openFrameworks Guido interface
 	Thomas Coffy (C) IRCAM 2014
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

#include "ofConstants.h"
#include "ofGraphicsBaseTypes.h"

#include "GUIDOEngineAdapter.h"

/// \brief simple wrapper for GUIDO Engine score library
/// parses Guido Music Notation strings or files and renders to an FBO
/// see  http://guidolib.sourceforge.net/ for more info about Guido
/// note: currently limited to instance at a time
class ofxGuido : public ofBaseDraws {

public:

	ofxGuido();
	ofxGuido(GuidoLayoutSettings &layoutSettings);
	virtual ~ofxGuido();

/// \section Load

	/// \brief loads and parses score from a GMN string
	/// \param gmn score as string in Gudio Music Notation format
	/// \return true on success
	bool loadString(const std::string &gmn);

	/// \brief loads and parses score from a GMN file
	/// \param gmnFile path to text file score in Gudio Music Notation format
	/// \return true on success
	bool loadFile(const std::string &gmnFile);

	/// \brief clear current score
	void clear();

/// \section Score

	/// \brief get the first displayed page in the score, 1-indexed
	int getPage() const;

	/// \brief set the first displayed page of the score, 1-indexed
	/// \return true if the page index is valid, otherwise false
	bool setPage(int page);

	/// \brief get the number of pages in the score
	int pageCount() const;

	/// \brief get the number of voices of the score
	int voiceCount() const;

    /// \brief mark notes for a voice by color within a give time range
    /// \param voice voice index within the score, 1-indexed
    /// \param start start time location
    /// \param duration time duration
    /// \param color color to apply to marked notes
    bool markVoice(int voice, const GuidoDate &start,
                   const GuidoDate &duration, const ofColor &color);

/// \section Settings

	/// \brief set the size of the render area & redraw
	/// \param w width in pixels
	/// \param h height in pixels
	void setSize(float w, float h);

	/// \brief resize page to fit score? (default false)
	/// \param resize enable or disable resizing
	void setResize(bool resize);

	/// \brief get the resize setting
	/// \return current resize setting
	bool getResize();

	/// \brief set score render color
	/// \param color scroe render color
	void setColor(const ofColor &color);

	/// \brief get the current score render color
	/// \return current score render color
	ofColor &getColor();

	/// \brief set the graphical layout settings & redraw
	/// \param settings new layout settings to apply
	void setLayoutSettings(const GuidoLayoutSettings &settings);

	/// \brief get the current graphical layout settings
	/// \return layout settings reference
	GuidoLayoutSettings &getLayoutSettings();

	/// \brief set the page format & redraw
	/// \param format new page format to apply
	void setPageFormat(const GuidoPageFormat &format);

	/// \brief get the page format for a given page
	/// \param page index of page with desired format, default 1
	/// \returns format struct on success or empty format struct on error
	GuidoPageFormat &getPageFormat(int page=1);

    /// \brief access to the underlying Guido ARHandler
    /// \returns current ARHandler or nullptr if not created
    ARHandler getARHandler();

    /// \brief access to the underlying Guido GRHandler
    /// \returns current GRHandler or nullptr if not created
    GRHandler getGRHandler();

/// \section Draw

	void draw(float x, float y, float w, float h) const;

	float getWidth() const;
	float getHeight() const;

	void setAnchorPercent(float xPct, float yPct);
	void setAnchorPoint(float x, float y);
	void resetAnchor();

	/// \brief get the underlying FBO used for score renderering
	/// \return render FBO
	ofFbo &getFbo();

/// \section Util

	/// \brief init GUIDO Engine with optional system font names
	///
	/// this is called automatically in the ofxGuido constructor so call this
	/// before creating an ofxGudio instance if you want manual control
	///
	/// \param textFont text font name, uses default "Times" font when ""
	/// \param musicFont music font name, uses default gudio2.ttf font when ""
	/// \return true on success
	static bool init(const std::string &textFont="", const std::string &muiscFont="");

protected:

	ARHandler arHandler = nullptr; //< abstract representation
	GRHandler grHandler = nullptr; //< graphical representation
	GuidoLayoutSettings	layoutSettings; //< current layout settings
	GuidoPageFormat pageFormat; //< last retrieved page format
	bool resize = false; //< resize page to fit music?
	ofColor color; //< score draw color
	int page = 1; //< current page in the score, 1-indexed

	/// update the score GR from a give AR
	bool updateGR(ARHandler arh);

	/// redraws score GR to fbo
	void redraw() const;

private:

	// prevent copy constructor and operator= from being generated
	ofxGuido(const ofxGuido&);
	const ofxGuido& operator=(const ofxGuido&);
};
