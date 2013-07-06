/*
Parameter Identification

Created by VDX
Copyright (C) 2009 VDX (That's me)

This stuff is free code, for free software... use it, do whatever with it :) 
I don't know what a GNU licence is :D , I do this for fun.
May others who know write a proper text here :)

*/

#include "ParamID.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


ParamID::ParamID(){
	//this->code = 0;
	//this->group = 0;
	//this->index = 0;
	//this->location = 0;
	strName = 0;
	enabled = 1;

	ch = 1;
	ctrl = 1;
	min = 0;
	max = 0;
	MidiVal = 0;

    
};


ParamID::~ParamID(){
    if (strName) delete [] strName;
};


void ParamID::SetName(const char* NewName) {
	
	strName = new char[100];
	sprintf(strName,NewName);

}
