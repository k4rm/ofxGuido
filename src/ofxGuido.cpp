#include "ofxGuido.h"


// loads fonts from the system
ofxGuido::ofxGuido(GuidoLayoutSettings& layoutSettings)
{
	guido = new GuidoComponent();
	guido->GuidoInit(OF_TTF_SERIF.c_str(), "guido2");
	guido->setGuidoLayoutSettings(layoutSettings);
}


bool ofxGuido::compile_string(const string& gstr)
{
	int err = guido->setGMNCode(gstr.c_str());
	return !(err == -1);
}

void ofxGuido::getPageFormat(GuidoPageFormat& format)
{
	GuidoGetPageFormat(guido->getGRHandler(), 1, &format);
}

void ofxGuido::draw_cache(int x, int y) {
	if (guido)
		guido->getDevice()->drawCache.draw(x, y);
}

void ofxGuido::draw(int x, int y, int w, int h) {
	if (guido)
		guido->draw(x, y, w, h);
}

void ofxGuido::markVoice() {
	if (!guido) return;
	GuidoDate date, duration;
	date.num = 1;
	date.denom = 4;
//	duration.num = 1;
//	duration.denom = 4;
	GuidoDuration(guido->fGRHandler, &duration);
	GuidoErrCode status = GuidoMarkVoice(guido->fARHandler, 1, date, duration, 255, 0, 0);
	if(status < 0) {
		ofLog() << GuidoGetErrorString(status);
	}
}

void ofxGuido::setSize(int w, int h) {
	if (guido) {
		guido->setSize(w, h);
		guido->getDevice()->NotifySize(w, h);
	}
}
