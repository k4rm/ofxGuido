#include "GuidoComponent.h"

/*
 * Simple wrapper for Guido engine library
 * http://guidolib.sourceforge.net/
 */

class openFrameworksDevice;

class ofxGuido {

public:

	ofxGuido();
	ofxGuido(GuidoLayoutSettings &layoutSettings);
	virtual ~ofxGuido();

	/// \brief loads and parses score from a GMN string
	/// \param gmn score as string in Gudio Music Notation format
	/// \return true on success
	bool loadString(const std::string &gmn);

	/// \brief loads and parses score from a GMN file
	/// \param gmnFile path to text file score in Gudio Music Notation format
	/// \return true on success
	bool loadFile(const std::string &gmnFile);

	void setLayoutSettings(const GuidoLayoutSettings &settings);
	GuidoLayoutSettings &getLayoutSettings();

	/// \brief get the page format for a given page
	/// \param page index of page with desired format, default 1
	/// \returns format struct on sucess or empty format struct on error
	GuidoPageFormat &getPageFormat(int page=1);

//	void draw_cache(int x, int y);
	void draw(int x, int y, int w, int h);

	/// \brief get the underlying FBO used for score renderering
	ofFbo &getFbo();

//	// not working yet...
//	void markVoice();

	void setWidth(int w) { if (guido) guido->setWidth(w); }
	void setHeight(int h) { if (guido) guido->setHeight(h); }
	int getWidth() { if (guido) return guido->getWidth(); else return 0; }
	int getHeight() { if (guido) return guido->getHeight(); else return 0; }
	void setSize(int w, int h);

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

	openFrameworksDevice *device = nullptr; //< GUIDO draw device
	GuidoLayoutSettings	layoutSettings;
	GuidoPageFormat pageFormat; //< last retrieved page format
	ARHandler arHandler = nullptr;
	GRHandler grHandler = nullptr;
	bool resize = false; //< resize page to fit music?
	ofColor scoreColor;
	int currentPage = 1; //< current page in the score, min 1

	bool ofxGudio::updateGR(ARHandler arh);

private:

	// prevent copy constructor and operator= from being generated
	ofxGuido (const ofxGuido&);
	const ofxGuido& operator= (const ofxGuido&);
};
