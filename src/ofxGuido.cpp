#include "ofxGuido.h"

#include "GUIDOEngine.h"
#include "GUIDOParse.h"
#include "openFrameworksDevice.h"
#include "openFrameworksSystem.h"

static openFrameworksSystem s_system;
static bool s_inited = false;

//------------------------------------------------------------------------------
ofxGuido::ofxGuido() {
	if(!s_inited) {
		ofxGuido::init();
	}
	GuidoGetDefaultLayoutSettings(&layoutSettings);
	device = new openFrameworksDevice();
}

//------------------------------------------------------------------------------
// loads fonts from the system
ofxGuido::ofxGuido(GuidoLayoutSettings& layoutSettings) {
	if(!s_inited) {
		ofxGuido::init();
	}
	setLayoutSettings(layoutSettings);
	device = new openFrameworksDevice(&s_system);
}

//------------------------------------------------------------------------------
ofxGuido::~ofxGuido() {
	if(arHandler) {GuidoFreeAR(arHandler);}
	if(grHandler) {GuidoFreeGR(grHandler);}
	if(device) {delete device;}
}

//------------------------------------------------------------------------------
bool ofxGuido::loadString(const std::string &gmn) {
	ARHandler arh;
	GuidoErrCode err = guidoNoErr;
	GuidoParser *parser = GuidoOpenParser();
	arh = GuidoString2AR(parser, gmn.c_str());
	if(!arh) {
		int l, c;
		const char *errstr;
		err = GuidoParserGetErrorCode(parser, l, c, &errstr);
		if (err != guidoNoErr) {
			ofLogError("ofxGuido") << "could not load string "
				<< ": line " << l << "," << c << " " << errstr;
			GuidoCloseParser(parser);
			return err;
		}
	}
	updateGR(arh);
	GuidoCloseParser(parser);
	return err;
}

//------------------------------------------------------------------------------
bool ofxGuido::loadFile(const std::string &gmnFile) {
	ARHandler arh;
	GuidoErrCode err = guidoNoErr;
	GuidoParser *parser = GuidoOpenParser();
	arh = GuidoFile2AR(parser, gmnFile.c_str());
	if(!arh) {
		int l, c;
		const char *errstr;
		err = GuidoParserGetErrorCode(parser, l, c, &errstr);
		if (err != guidoNoErr) {
			ofLogError("ofxGuido") << "could not load file " << gmnFile
				<< ": line " << l << "," << c << " " << errstr;
			GuidoCloseParser(parser);
			return err;
		}
	}
	updateGR(arh);
	GuidoCloseParser(parser);
	return err;
}

//------------------------------------------------------------------------------
void ofxGuido::draw(int x, int y, int w, int h) {
	if(!grHandler) return;

	ofSetColor(scoreColor);
	GuidoOnDrawDesc desc;
	desc.handle = grHandler;
	desc.hdc = (VGDevice *)device;
	desc.page = currentPage;
	desc.scrollx = x;
	desc.scrolly = 0;
	desc.isprint = false;
	desc.updateRegion.left = x;
	desc.updateRegion.top = y;
	desc.updateRegion.right = w;
	desc.updateRegion.bottom = h;
	desc.updateRegion.erase = true;
	desc.sizex = w;
	desc.sizey = h;

	//GuidoUpdateGR(fGRHandler, &fSettings);
	GuidoErrCode err = GuidoOnDraw(&desc);
	if (err != guidoNoErr) {
		ofLogError("ofxGuido") << "error in draw(): " << GuidoGetErrorString(err);
	}
}

//------------------------------------------------------------------------------
//void ofxGuido::draw_cache(int x, int y) {
//	if (guido)
//		guido->getDevice()->drawCache.draw(x, y);
//}

//------------------------------------------------------------------------------
ofFbo &ofxGuido::getFbo() {
	return device->drawCache;
}

//------------------------------------------------------------------------------
void ofxGuido::setLayoutSettings(const GuidoLayoutSettings& layoutSettings) {
	this->layoutSettings = layoutSettings;
	if(grHandler) {
		GuidoUpdateGR(grHandler, &layoutSettings);
	}
}

//------------------------------------------------------------------------------
GuidoLayoutSettings &ofxGuido::getLayoutSettings() {
	return layoutSettings;
}

//------------------------------------------------------------------------------
GuidoPageFormat &ofxGuido::getPageFormat(int page) {
	GuidoGetPageFormat(grHandler, 1, &pageFormat);
	return pageFormat;
}

//------------------------------------------------------------------------------
//void ofxGuido::markVoice() {
//	if (!guido) return;
//	GuidoDate date, duration;
//	date.num = 1;
//	date.denom = 4;
////	duration.num = 1;
////	duration.denom = 4;
//	GuidoDuration(guido->fGRHandler, &duration);
//	GuidoErrCode status = GuidoMarkVoice(guido->fARHandler, 1, date, duration, 255, 0, 0);
//	if(status < 0) {
//		ofLog() << GuidoGetErrorString(status);
//	}
//}

//------------------------------------------------------------------------------
void ofxGuido::setSize(int w, int h) {
	if (guido) {
		guido->setSize(w, h);
		guido->getDevice()->NotifySize(w, h);
	}
}

//------------------------------------------------------------------------------
bool ofxGuido::init(const std::string &textFont, const std::string &muiscFont) {
	if(!s_inited) {
		GuidoInitDesc desc;
		desc.graphicDevice = &device;
		desc.textFont =  (textFont == "" ? OF_TTF_SERIF.c_str() : textFont.c_str());
		desc.musicFont = (musicFont == "" ? "guido2" : musicFont.c_str());
		GuidoErrCode err = ::GuidoInit(&desc);
		if(err != guidoNoErr) {
			ofLogError("ofxGuido") << "could not init: " << GuidoGetErrorString(err);
			return false;
		}
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
		if(GRHandler) {
			GuidoFreeGR(GRHandler);
		}
		GRHandler = grh;
		ARHandler = arh;
		currentPage = 1;
		if(resize) {
			GuidoResizePageToMusic(GRHandler);
		}
	}
	else {
		GuidoFreeAR(arh);
		ofLogError("ofxGuido") << "could not update GR: " << GuidoGetErrorString(err);
		return false;
	}
	return true;
}

// PRIVATE
