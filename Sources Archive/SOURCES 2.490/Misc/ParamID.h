#ifndef PARAMID_H
#define PARAMID_H

/*
Parameter Identification 

Created by VDX
Copyright (C) 2009 VDX (That's me)

This stuff is free code, for free software... use it, do whatever with it :) 
I don't know what a GNU licence is :D , I do this for fun.
May others who know write a proper text here :)

*/

enum SynthType
{S_ADD,S_SUB,S_PAD,S_GLOBAL,S_FX};

enum WhereType
{W_GLOBAL,W_VOICE,W_MODAMP,W_MODFREQ,W_OSCIL,W_FX,W_MODOSCIL,W_DYNFILT};

enum GroupType
{
	G_AMPENV,
	G_AMPLFO,
	G_FILTPARAM,
	G_FILTENV,
	G_FILTLFO,
	G_PUNCH,
	G_FREQENV,
	G_FREQLFO,
	G_GLOBAL,
	G_FX,

	G_BANDENV,
	G_MAG,
	G_PHASE,

	G_FMAMPENV,
	G_FMFREQENV
	

};



class ParamID
{
public:
    
    ParamID();    
    ~ParamID();

int nPart;
int index;
int code;
int nvoice;
SynthType type;
float minval;
float maxval;
char* strName;
unsigned char MidiVal;

WhereType location;
GroupType group;

void SetName(const char* NewName);

int enabled;
int ch;
int ctrl;
float min;
float max;


};
#endif
