#ifndef MIDIRL_H
#define MIDIRL_H

/*
Midi Realtime Controller

Created by VDX
Copyright (C) 2009 VDX (That's me)

This stuff is free code, for free software... use it, do whatever with it :) 
I don't know what a GNU licence is :D , I do this for fun.
May others who know write a proper text here :)

*/

#include "Master.h"
#include "../UI/MasterUI.h"

#include <FL/Fl_ask.h>

#include "ParamID.h"
#include "../Misc/XMLwrapper.h"


//global pointers
//int* LearningMidi;
//MidiRL* midiRL;

class MidiRL
{
public:
    
    MidiRL();    
    ~MidiRL();

	void init(Master* master_, MasterUI* masterui_);
	int  AddComponent(ParamID *parID);
	void UpdateComponent(ParamID *parID, int n);
	void UpdateCompPars(int n, int newch, int newctrl, float newmin, float newmax,int newenabled);
	void UpdateCompValue(int n, unsigned char MidiVal, int setbyGUI);
	void EnableComponent(int n);
	void DisableComponent(int n);
	void LoadGUI();

	void  GetParameterName(int index, char* pname);	
	void  SetParameter(int index, float value, int b_scale);
	float GetParameter(int index);
	int   FindComp(ParamID* par);

	void cleanup();
	void cleanupUI();

	int* LearningMidi;
	MidiRL* midiRL;
    
	void Update_Realtime_Parameter(int MidiCh,int MidiCtrl, unsigned char MidiVal);

	void add2XML(XMLwrapper *xml);
	void getfromXML(XMLwrapper *xml);

	int updating;
	int updn;

private:
	
	float ScaleMidiVal(unsigned char MidiVal, ParamID* par);
	void ADDsynthTree(ParamID* par, unsigned char MidiVal);
	void PADsynthTree(ParamID* par, unsigned char MidiVal);
	void SUBsynthTree(ParamID* par, unsigned char MidiVal);
	void GlobalTree(ParamID* par, unsigned char MidiVal);
	void FxTree(ParamID* par, unsigned char MidiVal);

	


	Master* master;
	MasterUI* masterui;

	int nComp;

	ParamID *comp[256];
    
};
#endif
