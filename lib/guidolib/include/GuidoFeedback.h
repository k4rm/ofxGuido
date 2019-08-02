/*
  GUIDO Library
  Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef GuidoFeedback_H
#define GuidoFeedback_H

enum
{
	str_Title = 1,
	str_MusicalPreProcessing1,
	str_MusicalPreProcessing2,
	str_DeletingVoice,
	str_DeletingPages,
	str_ARMusicCreated,
	str_CreatingGR,
	str_RecreatingGraphElements,
	str_CreatingGraphSymbols,
	str_CreatingLinesAndSystems,
	str_CalcLineAndPageBreak,
	str_BeginningOfParse,
	str_ClosingFile,
	kLastFeedbackString
};

static const char * FeedbackStringsTable [] =
{
	"GUIDOLib",
	"Musical pre-processing I. Voice %d of %d",
	"Musical pre-processing II. Voice %d of %d",
	"Deleting Voice %d",
	"Deleting Pages... %d%%",
	"After parse. ARMusic has been created.",
	"Now we are creating the GR.",
	"Recreating Graphical elements.",
	"Creating Graphical symbols... %d%%",
	"Creating lines/systems of music... %d%%",
	"Calculating the optimum line and pagebreak.",	
	"Beginning of parse.",
	"Closing file.",
};

// TODO: Replace SetStatusMessage() and UpdateStatusMessage() by only one
// feedback function such as "void TextOut( int inStringID, int param1 = 0, int param2 = 0 )"
// --------------------------------------------------------------
class GuidoFeedback
{
	public:		
				enum { kProcessing = 1, kIdle }; // For Notify
				enum { kActionCancel = 1 };

		virtual			~GuidoFeedback() { }

		virtual void	Notify( int inStatus ) = 0;

		// - Dialog
		virtual void	CreateDialogWindow( int inTitleStringID = str_Title, int inDialogID = 0 ) = 0;	
		virtual void	InitDialog() = 0;
		virtual void	DestroyDialogWindow() = 0;

		virtual void	SetCancelButtonState( int state ) = 0;

		virtual void	SetStatusMessage( int inStringID, int param1 = 0, int param2 = 0 ) = 0;
		virtual	void	UpdateStatusMessage( int inStringID, int param1 = 0, int param2 = 0 ) = 0;

		virtual void	HandleUserAction( int inAction )  = 0;
		virtual bool	ProgDialogAbort() const = 0;
	
	protected:
		
		virtual const char * GetString( int inStringID ) const;

};

// --------------------------------------------------------------
inline const char *	
GuidoFeedback::GetString( int inStringID ) const
{ 	
	if( inStringID >= 1 && inStringID < kLastFeedbackString )
		return FeedbackStringsTable[ inStringID - 1 ];

	return 0;
}

#endif
