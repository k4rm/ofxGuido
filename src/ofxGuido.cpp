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
#include "ofxGuido.h"

#include "ofTrueTypeFont.h"
#include "ofGraphics.h"
#include "ofLog.h"

#include "GUIDOParse.h"
#include "openFrameworksDevice.h"
#include "openFrameworksSystem.h"

static openFrameworksSystem s_system;
static openFrameworksDevice s_device(&s_system);
static bool s_inited = false;

//------------------------------------------------------------------------------
ofxGuido::ofxGuido() : color(ofColor::black) {
	if(!s_inited) {
		ofxGuido::init();
	}
	GuidoGetDefaultLayoutSettings(&layoutSettings);
}

//------------------------------------------------------------------------------
// loads fonts from the system
ofxGuido::ofxGuido(GuidoLayoutSettings& layoutSettings) : color(ofColor::black) {
	if(!s_inited) {
		ofxGuido::init();
	}
	setLayoutSettings(layoutSettings);
}

//------------------------------------------------------------------------------
ofxGuido::~ofxGuido() {
	if(arHandler) {GuidoFreeAR(arHandler);}
	if(grHandler) {GuidoFreeGR(grHandler);}
}

// LOAD

//------------------------------------------------------------------------------
bool ofxGuido::loadString(const std::string &gmn) {
	ARHandler arh;
	GuidoErrCode err = guidoNoErr;
	GuidoParser *parser = GuidoOpenParser();
	arh = GuidoString2AR(parser, gmn.c_str());
	if(!arh) {
		int l, c;
		const char *errstr;
		std::stringstream msg("could not load string");
		err = GuidoParserGetErrorCode(parser, l, c, &errstr);
		if(err != guidoNoErr) {
			msg << ": line " << l << "," <<  c << " " << errstr;
		}
		ofLogError("ofxGuido") << msg.str();
		GuidoCloseParser(parser);
		return false;
	}
	updateGR(arh);
	GuidoCloseParser(parser);
	return true;
}

//------------------------------------------------------------------------------
bool ofxGuido::loadFile(const std::string &gmnFile) {
	ARHandler arh;
	GuidoErrCode err = guidoNoErr;
	GuidoParser *parser = GuidoOpenParser();
	arh = GuidoFile2AR(parser, ofToDataPath(gmnFile).c_str());
	if(!arh) {
		int l, c;
		const char *errstr;
		std::stringstream msg("could not load file " + gmnFile);
		err = GuidoParserGetErrorCode(parser, l, c, &errstr);
		if(err != guidoNoErr) {
			msg << ": line " << l << "," <<  c << " " << errstr;
		}
		ofLogError("ofxGuido") << msg.str();
		GuidoCloseParser(parser);
		return false;
	}
	updateGR(arh);
	GuidoCloseParser(parser);
	return true;
}

//------------------------------------------------------------------------------
void ofxGuido::clear() {
	if(arHandler) {
		GuidoFreeAR(arHandler);
		arHandler = nullptr;
	}
	if(grHandler) {
		GuidoFreeGR(grHandler);
		grHandler = nullptr;
	}
	s_device.BeginDraw();
	s_device.EndDraw();
	//GuidoGetDefaultLayoutSettings(&layoutSettings);
	//GuidoGetDefaultPageFormat(&pageFormat);
	setPage(1);
}

// SCORE

//------------------------------------------------------------------------------
int ofxGuido::getPage() const {
	return page;
}

//------------------------------------------------------------------------------
bool ofxGuido::setPage(int page) {
	if(page  < 1 || page > pageCount()) {
		return false;
	}
	this->page = page;
	redraw();
	return true;
}

//------------------------------------------------------------------------------
int ofxGuido::pageCount() const {
	return grHandler ? GuidoGetPageCount(grHandler) : 0;
}

//------------------------------------------------------------------------------
int ofxGuido::voiceCount() const {
	return arHandler ? GuidoCountVoices(arHandler) : 0;
}

// SETTINGS

//------------------------------------------------------------------------------
void ofxGuido::setResize(bool resize) {
	if(this->resize == resize) {return;}
	this->resize = resize;
	redraw();
}

//------------------------------------------------------------------------------
bool ofxGuido::getResize() {
	return resize;
}

//------------------------------------------------------------------------------
void ofxGuido::setSize(float w, float h) {
	s_device.NotifySize(w, h);
	redraw();
}

//------------------------------------------------------------------------------
void ofxGuido::setColor(const ofColor &color) {
	this->color = color;
	s_device.SetFontColor(openFrameworksDevice::ofColor2Color(color));
	redraw();
}

//------------------------------------------------------------------------------
ofColor &ofxGuido::getColor() {
	return color;
}

//------------------------------------------------------------------------------
void ofxGuido::setLayoutSettings(const GuidoLayoutSettings& layoutSettings) {
	this->layoutSettings = layoutSettings;
	if(grHandler) {
		GuidoUpdateGR(grHandler, &layoutSettings);
		redraw();
	}
}

//------------------------------------------------------------------------------
GuidoLayoutSettings &ofxGuido::getLayoutSettings() {
	return layoutSettings;
}

//------------------------------------------------------------------------------
void ofxGuido::setPageFormat(const GuidoPageFormat &format) {
	if(grHandler) {
		GuidoGrParameters parameters;
		parameters.layoutSettings = layoutSettings;
		parameters.pageFormat = format;
		GuidoUpdateGRParameterized(grHandler, &parameters);
		redraw();
	}
}

//------------------------------------------------------------------------------
GuidoPageFormat &ofxGuido::getPageFormat(int page) {
	GuidoGetPageFormat(grHandler, 1, &pageFormat);
	return pageFormat;
}

// DRAW

//------------------------------------------------------------------------------
void ofxGuido::draw(float x, float y, float w, float h) const {
	s_device.drawCache.draw(x, y, w, h);
}

//------------------------------------------------------------------------------
float ofxGuido::getWidth() const {
	return s_device.drawCache.getWidth();
}

//------------------------------------------------------------------------------
float ofxGuido::getHeight() const {
	return s_device.drawCache.getHeight();
}

//------------------------------------------------------------------------------
void ofxGuido::setAnchorPercent(float xPct, float yPct) {
	s_device.drawCache.setAnchorPercent(xPct, yPct);
}

//------------------------------------------------------------------------------
void ofxGuido::setAnchorPoint(float x, float y) {
	s_device.drawCache.setAnchorPercent(x, y);
}

//------------------------------------------------------------------------------
void ofxGuido::resetAnchor() {
	s_device.drawCache.setAnchorPercent(0, 0);
}

//------------------------------------------------------------------------------
ofFbo &ofxGuido::getFbo() {
	return s_device.drawCache;
}

/// \section Util

//------------------------------------------------------------------------------
bool ofxGuido::init(const std::string &textFont, const std::string &musicFont) {
	if(!s_inited) {
		GuidoInitDesc desc;
		desc.graphicDevice = &s_device;
		desc.textFont =  (textFont == "" ? OF_TTF_SERIF.c_str() : textFont.c_str());
		desc.musicFont = (musicFont == "" ? "guido2" : musicFont.c_str());
		GuidoErrCode err = ::GuidoInit(&desc);
		if(err != guidoNoErr) {
			ofLogError("ofxGuido") << "could not init: " << GuidoGetErrorString(err);
			return false;
		}
		s_device.NotifySize(ofGetWidth(), ofGetHeight());
		s_inited = true;
	}
	return true;
}

// PROTECTED

//------------------------------------------------------------------------------
bool ofxGuido::updateGR(ARHandler arh) {
	GRHandler grh;
	GuidoErrCode err = GuidoAR2GR(arh, &layoutSettings, &grh);
	if(err == guidoNoErr) {
		if(grHandler) {
			GuidoFreeGR(grHandler);
		}
		arHandler = arh;
		grHandler = grh;
		int pageCount = GuidoGetPageCount(grHandler);
		if(page > pageCount) {
			page = pageCount;
		}
		if(resize) {
			GuidoResizePageToMusic(grHandler);
		}
		redraw();
	}
	else {
		GuidoFreeAR(arh);
		ofLogError("ofxGuido") << "could not update GR: " << GuidoGetErrorString(err);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
void ofxGuido::redraw() const {
	if(!grHandler) return;

	ofSetColor(color);
	GuidoOnDrawDesc desc;
	desc.handle = grHandler;
	desc.hdc = (VGDevice *)&s_device;
	desc.page = page;
	desc.scrollx = 0;
	desc.scrolly = 0;
	desc.isprint = false;
	desc.updateRegion.left = 0;
	desc.updateRegion.top = 0;
	desc.updateRegion.right = getWidth();
	desc.updateRegion.bottom = getHeight();
	desc.updateRegion.erase = true;
	desc.sizex = getWidth();
	desc.sizey = getHeight();

	GuidoErrCode err = GuidoOnDraw(&desc);
	if(err != guidoNoErr) {
		ofLogError("ofxGuido") << "redraw error: " << GuidoGetErrorString(err);
	}
}

// PRIVATE
