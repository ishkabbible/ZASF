/*
Midi Realtime Controller

Created by VDX
Copyright (C) 2009 VDX (That's me)

This stuff is free code, for free software... use it, do whatever with it :) 
I don't know what a GNU licence is :D , I do this for fun.
May others who know write a proper text here :)

*/

#include "../Output/VSTaudiooutput.h"
#include "MidiRL.h"

//int* LearningMidi;

MidiRL::MidiRL(){

	int i;

	master = NULL;
	masterui = NULL;

	nComp = 0;
	updating = 0;
	updn = 0;

	for (i=0; i<255; i++)
		comp[i] = NULL;
    
};

void MidiRL::init(Master* master_, MasterUI* masterui_){
	master = master_;
	masterui = masterui_;
	LearningMidi = (master->LearningMidi);
	//cleanup();
	if (masterui) cleanupUI();
	
}

void MidiRL::cleanup(){
	
	int i;

	for (i=0; i<nComp; i++)
		if (comp[i]) {
			delete comp[i];
			comp[i] = NULL;
		};

	nComp = 0;
}

void MidiRL::cleanupUI() {

	if (masterui)
		if (masterui->midilearn)
			masterui->midilearn->cleanup();
}

MidiRL::~MidiRL(){
	int i;


	for (i=0; i<255; i++)
		if (comp[i]){
			delete comp[i];
			comp[i] = NULL;
		};
    
};

int MidiRL::AddComponent(ParamID *parID){

	if (updating)
		UpdateComponent(parID,updn);
	else
	{
		//add here a new component
		//*************************
		comp[nComp] = new ParamID;

		comp[nComp]->ch = -1; //not set
		comp[nComp]->code = parID->code;
		comp[nComp]->ctrl = -1;
		comp[nComp]->enabled = 1;
		comp[nComp]->group = parID->group;
		comp[nComp]->index = parID->index;
		comp[nComp]->location = parID->location;
		comp[nComp]->max = parID->maxval;
		comp[nComp]->min = parID->minval;
		comp[nComp]->maxval = parID->maxval;
		comp[nComp]->minval = parID->minval;
		comp[nComp]->nPart = parID->nPart;
		comp[nComp]->nvoice = parID->nvoice;
		comp[nComp]->strName = parID->strName;
		comp[nComp]->type = parID->type;
		comp[nComp]->MidiVal = parID->MidiVal;


		//*************************



	if (masterui)
		if (masterui->midilearn) {
			masterui->midilearn->AddComponent(parID);
		};

	nComp++;
	};
	return (nComp-1);

};

void MidiRL::UpdateComponent(ParamID *parID, int n){
	n--;

		comp[n]->ch = -1; //not set
		comp[n]->code = parID->code;
		comp[n]->ctrl = -1;
		comp[n]->enabled = 1;
		comp[n]->group = parID->group;
		comp[n]->index = parID->index;
		comp[n]->location = parID->location;
		comp[n]->max = parID->maxval;
		comp[n]->min = parID->minval;
		comp[n]->maxval = parID->maxval;
		comp[n]->minval = parID->minval;
		comp[n]->nPart = parID->nPart;
		comp[n]->nvoice = parID->nvoice;
		comp[n]->strName = parID->strName;
		comp[n]->type = parID->type;
		comp[n]->MidiVal = parID->MidiVal;



	if (masterui)
		if (masterui->midilearn) {
			masterui->midilearn->UpdateComponent(parID,n);
		};
};

void MidiRL::EnableComponent(int n){
	n--;
	comp[n]->enabled = 1;
};

void MidiRL::DisableComponent(int n){
	n--;
	comp[n]->enabled = 0;
};

void MidiRL::UpdateCompPars(int n, int newch, int newctrl, float newmin, float newmax, int newenabled) {
	if (n>nComp-1) return;
	//n--;  //zero based
	comp[n]->ch = newch;
	comp[n]->ctrl = newctrl;
	comp[n]->min = newmin;
	comp[n]->max = newmax;
	comp[n]->enabled = newenabled;
};


void MidiRL::UpdateCompValue(int n, unsigned char MidiVal, int setbyGUI) {
	ParamID* par;
	MasterUI* masterui_;

	if (n>nComp-1) return;

	comp[n]->MidiVal = MidiVal;


	//identify the parameter... 
	par = comp[n];	

	if (par->ctrl > 0) {
		//means MIDI CC, and not host automation, right??
		SetParameter(n,(float)(MidiVal)/127,0);		
		return;
	};

	//so... doing the host automation 

	if (setbyGUI) {
		if (master->b_writingAutomation ==-1) {
			master->b_writingAutomation = n;
			master->theVST->beginEdit(n);
		};


		master->theVST->setParameterAutomated(n,GetParameter(n));

	};

	//if (!setbyGUI) {
	if (par->enabled) {		
		//disable UI update
		masterui_ = masterui;
		masterui = NULL;
//*******************************************************************************************
//  PARAMETER IDENTIFICATION TREE  *
//**********************************
switch (par->type)
{	
case S_ADD:
	ADDsynthTree(par,MidiVal);
	break; //type

case S_PAD:
	PADsynthTree(par,MidiVal);
	break; //type

case S_FX:
	FxTree(par,MidiVal);
	break; //type

case S_SUB:
	SUBsynthTree(par,MidiVal);

case S_GLOBAL:
	GlobalTree(par,MidiVal);
	break;
	
} //type


//********************************************************************************************
//re enable GUI update
masterui = masterui_;
}; //par->enabled
	//}; //set by GUI


};


int MidiRL::FindComp(ParamID* par) {

	int out;
	int i;
	out = -1;


	for (i = 0; i<nComp; i++){	  
		  if (comp[i]->code == par->code)			  
				  if (comp[i]->enabled == par->enabled)
					  if (comp[i]->group == par->group)
						  if (comp[i]->index == par->index)
							  if (comp[i]->maxval == par->maxval)
								  if (comp[i]->minval == par->minval)
									  if (comp[i]->nPart == par ->nPart)
										  if (comp[i]->nvoice == par->nvoice)
											  if (comp[i]->type == par->type)
											  {out = i; break;};

									 


	}; //for

	return out;
};

//VST Parameters
/*
void  GetParameterName(int index, char* pname);
void  SetParameter(int index, float value);
float GetParameter(int index);
*/



void MidiRL::GetParameterName(int index, char *pname){
if (index > 256) return;
if (index > nComp-1) {
	strcpy(pname, "[Unused]");

	return;
};

//comp[index]->strName;
strcpy(pname, (const char*)comp[index]->strName);


};

void MidiRL::SetParameter(int index, float value,int b_scale) {
if (index > 256) return;
if (index > nComp-1) return;

unsigned char MidiVal;
ParamID* par;
float oldmin;
float oldmax;
int oldenable;

MidiVal = (unsigned char)(value*127);


//identify the parameter... 
par = comp[index];		
	oldmin = par->min;
	oldmax = par->max;
	oldenable = par->enabled;

if (!b_scale) {	
	par->max = par->maxval;
	par->min = par->minval;
	par->enabled = 1;
};

if (par->enabled) {
//*******************************************************************************************
//  PARAMETER IDENTIFICATION TREE  *
//**********************************
switch (par->type)
{	
case S_ADD:
	ADDsynthTree(par,MidiVal);
	break; //type

case S_PAD:
	PADsynthTree(par,MidiVal);
	break; //type

case S_FX:
	FxTree(par,MidiVal);
	break; //type

case S_SUB:
	SUBsynthTree(par,MidiVal);

case S_GLOBAL:
	GlobalTree(par,MidiVal);
	break;
	
} //type


//********************************************************************************************
}; //par->enabled

	par->min = oldmin;
	par->max = oldmax;
	par->enabled = oldenable;


}; //function set params

float MidiRL::GetParameter(int index) {

	if (index > 256) return 0;
	if (index > nComp-1) return 0;

	unsigned char MidiVal;
	ParamID* par;

	//MidiVal = (unsigned char)(value*127);
	par = comp[index];

	if (par->enabled) {

		MidiVal = par->MidiVal;
		return (float)MidiVal/127;

	}; //par is enabled


return 0; //update here
};




float MidiRL::ScaleMidiVal(unsigned char MidiVal, ParamID* par) {
	//scale 0...127 to min...max	
	float min;
	float max;
	float tmp;
	float out;

	min = par->min;
	max = par->max;

	if (max < min) {	//inverted scaling
		MidiVal = (127-MidiVal);
		tmp = min;
		min = max;
		max = tmp;
	}

	out = (MidiVal* (max-min)/(127.0f))+min;
	return out;
}

void MidiRL::ADDsynthTree(ParamID* par, unsigned char MidiVal){
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//  PARAMETER IDENTIFICATION TREE  *
//**********************************

	//ADDsynth
    //can be global or voice par or oscil
	switch (par->location)
	{
	case W_GLOBAL:

		switch (par->group)
		{
		case G_AMPENV:
			switch (par->code)
			{
			case 1:
				//Global Amp Env, A.dt
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpEnvelope->
						PA_dt = ScaleMidiVal(MidiVal,par);					
				}

				if (masterui){	
				if (masterui->partui->adnoteui)
				if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){					
					masterui->partui->adnoteui->ampenv->refresh();
					masterui->partui->adnoteui->ampenv->freeedit->redraw();
					}
				}
				break;

			case 2:
				//Global Amp Env, D.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpEnvelope->
						PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->ampenv->refresh();
						masterui->partui->adnoteui->ampenv->freeedit->redraw();
					}
				break;

			case 3:
				//Global Amp Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->ampenv->refresh();
						masterui->partui->adnoteui->ampenv->freeedit->redraw();
					};				
				break;

			case 4:
				//Global Amp Env, S.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpEnvelope->
					PS_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->ampenv->refresh();
						masterui->partui->adnoteui->ampenv->freeedit->redraw();
					};				
				break;

			case 5:
				//Global Amp Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->ampenv->refresh();						
					};				
				break;

			} // code

			break; //group

		case G_FREQENV:

			switch(par->code)
			{
			case 1:
				//Global Freq Env, A.Val;
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->freqenv->refresh();
						masterui->partui->adnoteui->freqenv->freeedit->redraw();
					};				
				break;

			case 2:
				//Global Freq Env, A.dt;
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->freqenv->refresh();
						masterui->partui->adnoteui->freqenv->freeedit->redraw();
					};				
				break;		

			case 3:
				//Global Freq Env, R.val;
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->freqenv->refresh();
						masterui->partui->adnoteui->freqenv->freeedit->redraw();
					};				
				break;

			case 4:
				//Global Freq Env, R.dt;
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->freqenv->refresh();
						masterui->partui->adnoteui->freqenv->freeedit->redraw();
					};				
				break;

			case 5:
				//Global Freq Env, Strech;
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->freqenv->refresh();					
					};				
				break;
			} //code
			break; //group

		case G_FILTENV:

			switch (par->code)
			{
			case 1:
				//Global Filter Env, A.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
						masterui->partui->adnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 2:
				//Global Filter Env, A.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
						masterui->partui->adnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 3:
				//Global Filter Env, D.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					PD_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
						masterui->partui->adnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 4:
				//Global Filter Env, D.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
						masterui->partui->adnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 5:
				//Global Filter Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
						masterui->partui->adnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 6:
				//Global Filter Env, R.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
						masterui->partui->adnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 7:
				//Global Filter Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
						masterui->partui->adnoteui->filterenv->refresh();
					};				
				break;
			} //code
			break; //group

		case G_AMPLFO:

			switch (par->code)
			{
			case 1:
				//Global Amp LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			case 2:
				//Global Amp LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			case 4:
				//Global Amp LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			case 3:
				//Global Amp LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			case 5:
				//Global Amp LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			case 6:
				//Global Amp LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			case 7:
				//Global Amp LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->amplfo->refresh();							
					};	
				break;

			} //code
			break; //group

		case G_FREQLFO:

			switch (par->code)
			{
			case 1:
				//Global Freq LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			case 2:
				//Global Freq LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			case 4:
				//Global Freq LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			case 3:
				//Global Freq LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			case 5:
				//Global Freq LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			case 6:
				//Global Freq LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			case 7:
				//Global Freq LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->freqlfo->refresh();							
					};	
				break;

			} //code
			break; //group

		case G_FILTLFO:

			switch (par->code)
			{
			case 1:
				//Global Filter LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			case 2:
				//Global Filter LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			case 4:
				//Global Filter LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			case 3:
				//Global Filter LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			case 5:
				//Global Filter LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			case 6:
				//Global Filter LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			case 7:
				//Global Filter LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterlfo->refresh();							
					};	
				break;

			} //code
			break; //group


		case G_FILTPARAM:

			switch (par->code)
			{
			case 1:
				//Global Filter Params, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterui->refresh();							
					};	
				break;

			case 2:
				//Global Filter Params, Q
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->
						Pq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterui->refresh();							
					};	
				break;

			case 3:
				//Global Filter Params, VelAmp
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PFilterVelocityScale = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterui->refresh();							
					};	
				break;

			case 4:
				//Global Filter Params, VelSns
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PFilterVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterui->refresh();							
					};	
				break;

			case 5:
				//Global Filter Params, FreqTr
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->
						Pfreqtrack = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterui->refresh();							
					};	
				break;


			case 6:
				//Global Filter Params, Gain
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->
						Pgain = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->filterui->refresh();							
					};	
				break;

			}  //code
			break; //group

		case G_GLOBAL:

			switch (par->code)
			{
			case 1:
				//Global Params, Volume
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PVolume = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->b_changed = 10;
				}
				break;

			case 2:
				//Global Params, Vel.Sns
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PAmpVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].padpars->b_changed = 10;
				}
				break;

			case 3:
				//Global Params, Pan
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PPanning = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->b_changed = 10;
				}
				break;

			} // code

			if (masterui)
					if (masterui->partui->adnoteui)
					if (masterui->partui->adnoteui->ADnoteGlobalParameters->visible()){
							masterui->partui->adnoteui->refresh_g();
					};	
			break; //group

			

		} // group
		break;  //location

	case W_VOICE:

		switch (par->group)
		{
		case G_AMPENV:
			switch (par->code)
			{
			case 1:
				//Voice Amp Env, A.dt
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpEnvelope->
						PA_dt = ScaleMidiVal(MidiVal,par);					
				}

				if (masterui)
				if (masterui->partui->adnoteui->advoice)
				if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
					masterui->partui->adnoteui->advoice->voiceampenvgroup->refresh();
					masterui->partui->adnoteui->advoice->voiceampenvgroup->freeedit->redraw();
				}				
				break;

			case 2:
				//Voice Amp Env, D.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpEnvelope->
						PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voiceampenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voiceampenvgroup->freeedit->redraw();
					};
				break;

			case 3:
				//Voice Amp Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voiceampenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voiceampenvgroup->freeedit->redraw();
					};			
				break;

			case 4:
				//Voice Amp Env, S.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpEnvelope->
					PS_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voiceampenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voiceampenvgroup->freeedit->redraw();
					};		
				break;

			case 5:
				//Voice Amp Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voiceampenvgroup->refresh();						
					};			
				break;

			} // code

			break; //group

		case G_FREQENV:

			switch(par->code)
			{
			case 1:
				//Voice Freq Env, A.Val;
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->freeedit->redraw();
					};		
				break;

			case 2:
				//Voice Freq Env, A.dt;
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->freeedit->redraw();
					};			
				break;		

			case 3:
				//Voice Freq Env, R.val;
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->freeedit->redraw();
					};			
				break;

			case 4:
				//Voice Freq Env, R.dt;
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->freeedit->redraw();
					};				
				break;

			case 5:
				//Voice Freq Env, Strech;
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){					
						masterui->partui->adnoteui->advoice->voicefreqenvgroup->refresh();						
					};			
				break;
			} //code
			break; //group

		case G_FILTENV:

			switch (par->code)
			{
			case 1:
				//Voice Filter Env, A.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->freeedit->redraw();
					};				
				break;

			case 2:
				//Voice Filter Env, A.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->freeedit->redraw();
					};			
				break;

			case 3:
				//Voice Filter Env, D.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					PD_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->freeedit->redraw();
					};		
				break;

			case 4:
				//Voice Filter Env, D.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->freeedit->redraw();
					};		
				break;

			case 5:
				//Voice Filter Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->freeedit->redraw();
					};			
				break;

			case 6:
				//Voice Filter Env, R.val
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->freeedit->redraw();
					};			
				break;

			case 7:
				//Voice Filter Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
						masterui->partui->adnoteui->advoice->voicefilterenvgroup->refresh();						
					};			
				break;
			} //code
			break; //group

		case G_AMPLFO:

			switch (par->code)
			{
			case 1:
				//Voice Amp LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			case 2:
				//Voice Amp LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			case 4:
				//Voice Amp LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			case 3:
				//Voice Amp LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			case 5:
				//Voice Amp LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			case 6:
				//Voice Amp LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			case 7:
				//Voice Amp LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voiceamplfogroup->refresh();							
					};	
				break;

			} //code
			break; //group

		case G_FREQLFO:

			switch (par->code)
			{
			case 1:
				//Voice Freq LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			case 2:
				//Voice Freq LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			case 4:
				//Voice Freq LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			case 3:
				//Voice Freq LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			case 5:
				//Voice Freq LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			case 6:
				//Voice Freq LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			case 7:
				//Voice Freq LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefreqlfogroup->refresh();
					};	
				break;

			} //code
			break; //group

		case G_FILTLFO:

			switch (par->code)
			{
			case 1:
				//Voice Filter LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};	
				break;

			case 2:
				//Voice Filter LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};
				break;

			case 4:
				//Voice Filter LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};
				break;

			case 3:
				//Voice Filter LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};
				break;

			case 5:
				//Voice Filter LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};
				break;

			case 6:
				//Voice Filter LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};
				break;

			case 7:
				//Voice Filter LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterlfogroup->refresh();							
					};
				break;

			} //code
			break; //group


		case G_FILTPARAM:

			switch (par->code)
			{
			case 1:
				//Voice Filter Params, Freq
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterparamgroup->refresh();							
					};	
				break;

			case 2:
				//Voice Filter Params, Q
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						Pq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterparamgroup->refresh();							
					};
				break;

/*
			case 3:
				//Voice Filter Params, VelAmp ***DISABLED
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PFilterVelocityScale = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterparamgroup->refresh();							
					};
				break;

			case 4:
				//Voice Filter Params, VelSns ***DISABLED
				if (master){
					master->part[par->nPart]->kit[0].adpars->GlobalPar.
						PFilterVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterparamgroup->refresh();							
					};
				break;

*/

			case 5:
				//Voice Filter Params, FreqTr
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						Pfreqtrack = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterparamgroup->refresh();							
					};
				break;


			case 6:
				//Voice Filter Params, Gain
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						Pgain = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].VoiceFilter->
						b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible()){
							masterui->partui->adnoteui->advoice->voicefilterparamgroup->refresh();							
					};
				break;

			}  //code
			break; //group


		case G_GLOBAL:

			switch (par->code)
			{
			case 1:
				//Voice Global Params, Volume
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].
						PVolume = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].b_changed = 10;
				}
				break;

			case 2:
				//Voice Global Params, Vel.Sns
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].
						PAmpVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].b_changed = 10;
				}
				break;

			case 3:
				//Voice Global Params, Pan
				if (master){
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].
						PPanning = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].b_changed = 10;
				}
				break;

			} // code

			if (masterui)
					if (masterui->partui->adnoteui->advoice)
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible())
							masterui->partui->adnoteui->advoice->refresh_g();
					
					if (masterui->partui->adnoteui->voicelistitem[par->nvoice]->visible())
							masterui->partui->adnoteui->voicelistitem[par->nvoice]->refreshlist_g();
						
			break; //group
			

		} // group
		break;  //location

	case W_OSCIL:

		switch (par->group)
		{
		case G_MAG:

			if (master) {				
				master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].OscilSmp->
					Phmag[par->index] = ScaleMidiVal(MidiVal,par);
				//no preparing here; it is done in update_realtime()
				master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].OscilSmp->
					b_changed = 10;
			}
			if (masterui)
				if (masterui->partui->adnoteui->advoice->oscedit){
					if (masterui->partui->adnoteui->advoice->oscedit->osceditUI->visible())
						masterui->partui->adnoteui->advoice->oscedit->refresh();
					
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible())
						masterui->partui->adnoteui->advoice->redraw();					
				};

			break; //group

		case G_PHASE:

			if (master) {				
				master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].OscilSmp->
					Phphase[par->index] = ScaleMidiVal(MidiVal,par);
				//no preparing here; it is done in update_realtime()
				master->part[par->nPart]->kit[0].adpars->VoicePar[par->nvoice].OscilSmp->
					b_changed = 10;
			}
			if (masterui)
				if (masterui->partui->adnoteui->advoice->oscedit){
					if (masterui->partui->adnoteui->advoice->oscedit->osceditUI->visible())
						masterui->partui->adnoteui->advoice->oscedit->refresh();
					
					if (masterui->partui->adnoteui->advoice->ADnoteVoiceParameters->visible())
						masterui->partui->adnoteui->advoice->redraw();					
				};


			break; //group

		} // group
		break; //location


	} //location	  	
}  //end of function ADDsynthTree


//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************


void MidiRL::PADsynthTree(ParamID *par, unsigned char MidiVal){
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//  PARAMETER IDENTIFICATION TREE  *
//**********************************
	switch (par->location)
	{
	case W_GLOBAL:

		switch (par->group)
		{
		case G_AMPENV:
			switch (par->code)
			{
			case 1:
				//Global Amp Env, A.dt
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpEnvelope->
						PA_dt = ScaleMidiVal(MidiVal,par);					
				}

				if (masterui)	
				if (masterui->partui->padnoteui)
				if (masterui->partui->padnoteui->envlfogroup->visible()){					
					masterui->partui->padnoteui->ampenv->refresh();
					masterui->partui->padnoteui->ampenv->freeedit->redraw();
					}				
				break;

			case 2:
				//Global Amp Env, D.dt
				if (master)
					master->part[par->nPart]->kit[0].padpars->AmpEnvelope->
						PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){					
						masterui->partui->padnoteui->ampenv->refresh();
						masterui->partui->padnoteui->ampenv->freeedit->redraw();
					}				
				break;

			case 3:
				//Global Amp Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].padpars->AmpEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){					
						masterui->partui->padnoteui->ampenv->refresh();
						masterui->partui->padnoteui->ampenv->freeedit->redraw();
					}				
				break;

			case 4:
				//Global Amp Env, S.val
				if (master)
					master->part[par->nPart]->kit[0].padpars->AmpEnvelope->
					PS_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){					
						masterui->partui->padnoteui->ampenv->refresh();
						masterui->partui->padnoteui->ampenv->freeedit->redraw();
					}			
				break;

			case 5:
				//Global Amp Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].padpars->AmpEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){					
						masterui->partui->padnoteui->ampenv->refresh();						
					}			
				break;

			} // code

			break; //group

		case G_FREQENV:

			switch(par->code)
			{
			case 1:
				//Global Freq Env, A.Val;
				if (master)
					master->part[par->nPart]->kit[0].padpars->FreqEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->freqenv->refresh();
						masterui->partui->padnoteui->freqenv->freeedit->redraw();
					};				
				break;

			case 2:
				//Global Freq Env, A.dt;
				if (master)
					master->part[par->nPart]->kit[0].padpars->FreqEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->freqenv->refresh();
						masterui->partui->padnoteui->freqenv->freeedit->redraw();
					};				
				break;		

			case 3:
				//Global Freq Env, R.val;
				if (master)
					master->part[par->nPart]->kit[0].padpars->FreqEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->freqenv->refresh();
						masterui->partui->padnoteui->freqenv->freeedit->redraw();
					};			
				break;

			case 4:
				//Global Freq Env, R.dt;
				if (master)
					master->part[par->nPart]->kit[0].padpars->FreqEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->freqenv->refresh();
						masterui->partui->padnoteui->freqenv->freeedit->redraw();
					};				
				break;

			case 5:
				//Global Freq Env, Strech;
				if (master)
					master->part[par->nPart]->kit[0].padpars->FreqEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->freqenv->refresh();						
					};				
				break;
			} //code
			break; //group

		case G_FILTENV:

			switch (par->code)
			{
			case 1:
				//Global Filter Env, A.val
				if (master)
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();
						masterui->partui->padnoteui->filterenv->freeedit->redraw();
					};				
				break;

			case 2:
				//Global Filter Env, A.dt
				if (master)
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();
						masterui->partui->padnoteui->filterenv->freeedit->redraw();
					};			
				break;

			case 3:
				//Global Filter Env, D.val
				if (master)
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					PD_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();
						masterui->partui->padnoteui->filterenv->freeedit->redraw();
					};			
				break;

			case 4:
				//Global Filter Env, D.dt
				if (master)					
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();
						masterui->partui->padnoteui->filterenv->freeedit->redraw();
					};			
				break;

			case 5:
				//Global Filter Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();
						masterui->partui->padnoteui->filterenv->freeedit->redraw();
					};			
				break;

			case 6:
				//Global Filter Env, R.val
				if (master)
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();
						masterui->partui->padnoteui->filterenv->freeedit->redraw();
					};			
				break;

			case 7:
				//Global Filter Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].padpars->FilterEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
						masterui->partui->padnoteui->filterenv->refresh();						
					};			
				break;
			} //code
			break; //group

		case G_AMPLFO:

			switch (par->code)
			{
			case 1:
				//Global Amp LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			case 2:
				//Global Amp LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			case 4:
				//Global Amp LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			case 3:
				//Global Amp LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			case 5:
				//Global Amp LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			case 6:
				//Global Amp LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			case 7:
				//Global Amp LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].padpars->AmpLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].padpars->AmpLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->amplfo->refresh();							
					};	
				break;

			} //code
			break; //group

		case G_FREQLFO:

			switch (par->code)
			{
			case 1:
				//Global Freq LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			case 2:
				//Global Freq LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			case 4:
				//Global Freq LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			case 3:
				//Global Freq LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			case 5:
				//Global Freq LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			case 6:
				//Global Freq LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			case 7:
				//Global Freq LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].padpars->FreqLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FreqLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->freqlfo->refresh();							
					};	
				break;

			} //code
			break; //group

		case G_FILTLFO:

			switch (par->code)
			{
			case 1:
				//Global Filter LFO, Freq
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			case 2:
				//Global Filter LFO, Depth
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Pintensity = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			case 4:
				//Global Filter LFO, Delay
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Pdelay = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			case 3:
				//Global Filter LFO, Start
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Pstartphase = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			case 5:
				//Global Filter LFO, Strech
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Pstretch = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			case 6:
				//Global Filter LFO, A.rand
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Prandomness = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			case 7:
				//Global Filter LFO, F.rand
				if (master){
					master->part[par->nPart]->kit[0].padpars->FilterLfo->
						Pfreqrand = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.FilterLfo->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterlfo->refresh();							
					};	
				break;

			} //code
			break; //group


		case G_FILTPARAM:

			switch (par->code)
			{
			case 1:
				//Global Filter Params, Freq
				if (master){
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterui->refresh();							
					};	
				break;

			case 2:
				//Global Filter Params, Q
				if (master){
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->
						Pq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterui->refresh();							
					};	
				break;

			case 3:
				//Global Filter Params, VelAmp
				if (master){
					master->part[par->nPart]->kit[0].padpars->
						PFilterVelocityScale = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterui->refresh();							
					};	
				break;

			case 4:
				//Global Filter Params, VelSns
				if (master){
					master->part[par->nPart]->kit[0].padpars->
						PFilterVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].adpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterui->refresh();							
					};	
				break;

			case 5:
				//Global Filter Params, FreqTr
				if (master){
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->
						Pfreqtrack = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterui->refresh();							
					};	
				break;


			case 6:
				//Global Filter Params, Gain
				if (master){
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->
						Pgain = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->filterui->refresh();							
					};	
				break;

			}  //code
			break; //group

		case G_GLOBAL:

			switch (par->code)
			{
			case 1:
				//Global Params, Volume
				if (master){
					master->part[par->nPart]->kit[0].padpars->
						PVolume = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->b_changed = 10;
				}
				break;

			case 2:
				//Global Params, Vel.Sns
				if (master){
					master->part[par->nPart]->kit[0].padpars-> 
						PAmpVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].padpars->b_changed = 10;
				}
				break;

			case 3:
				//Global Params, Pan
				if (master){
					master->part[par->nPart]->kit[0].padpars-> 
						PPanning = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].padpars->b_changed = 10;
				}
				break;



			} //code
				if (masterui)
					if (masterui->partui->padnoteui)
					if (masterui->partui->padnoteui->envlfogroup->visible()){
							masterui->partui->padnoteui->refresh_g();
					};	


			break; //group

			

		} // group
		//break;  //location

	} //location
}




//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************





void MidiRL::FxTree(ParamID *par, unsigned char MidiVal) {
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//  SYNTH EFFECTS PARAMETER IDENTIFICATION TREE  *
//************************************************
	if (par->index >=0){

		if (par->location==W_DYNFILT){

			if (master)
			switch (par->code)
			{
			case 1:
				//freq				
				master->part[par->nPart]->partefx[par->index]->filterpars->
					Pfreq = ScaleMidiVal(MidiVal,par);	
				break;

			case 2:
				//Q
				master->part[par->nPart]->partefx[par->index]->filterpars->
					Pq = ScaleMidiVal(MidiVal,par);
				break;

			case 6:
				//gain
				master->part[par->nPart]->partefx[par->index]->filterpars->
					Pgain = ScaleMidiVal(MidiVal,par);
				break;
			}

			if (masterui)
			if (masterui->partui->inseffectui)
			if (masterui->partui->inseffectui->filterwindow->visible()){
				masterui->partui->inseffectui->groupfilterui->refresh();
			};	


		}
		else
		{

			if (master)
			master->part[par->nPart]->partefx[par->index]->seteffectpar(par->code,
				ScaleMidiVal(MidiVal,par));		
		
	
			if (masterui)
			if (masterui->partui->partfx)
			if (masterui->partui->partfx->visible()){
				masterui->partui->inseffectui->refresh();
			};	
		}
	}

}


//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************



void MidiRL::SUBsynthTree(ParamID *par, unsigned char MidiVal) {

switch (par->group)
		{
		case G_AMPENV:
			switch (par->code)
			{
			case 1:
				//Global Amp Env, A.dt
				if (master){
					master->part[par->nPart]->kit[0].subpars->AmpEnvelope->
						PA_dt = ScaleMidiVal(MidiVal,par);					
				}

				if (masterui)	
				if (masterui->partui->subnoteui)
				if (masterui->partui->subnoteui->ampenv->visible()){					
					masterui->partui->subnoteui->ampenv->refresh();
					masterui->partui->subnoteui->ampenv->freeedit->redraw();
					}				
				break;

			case 2:
				//Global Amp Env, D.dt
				if (master)
					master->part[par->nPart]->kit[0].subpars->AmpEnvelope->
						PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->ampenv->visible()){					
						masterui->partui->subnoteui->ampenv->refresh();
						masterui->partui->subnoteui->ampenv->freeedit->redraw();
					}			
				break;

			case 3:
				//Global Amp Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].subpars->AmpEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->ampenv->visible()){					
						masterui->partui->subnoteui->ampenv->refresh();
						masterui->partui->subnoteui->ampenv->freeedit->redraw();
					}				
				break;

			case 4:
				//Global Amp Env, S.val
				if (master)
					master->part[par->nPart]->kit[0].subpars->AmpEnvelope->
					PS_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->ampenv->visible()){					
						masterui->partui->subnoteui->ampenv->refresh();
						masterui->partui->subnoteui->ampenv->freeedit->redraw();
					}		
				break;

			case 5:
				//Global Amp Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].subpars->AmpEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->ampenv->visible()){					
						masterui->partui->subnoteui->ampenv->refresh();
					}		
				break;

			} // code

			break; //group

		case G_FREQENV:

			switch(par->code)
			{
			case 1:
				//Global Freq Env, A.Val;
				if (master)
					master->part[par->nPart]->kit[0].subpars->FreqEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->freqenvelopegroup->visible()){					
						masterui->partui->subnoteui->freqenvelopegroup->refresh();
						masterui->partui->subnoteui->freqenvelopegroup->freeedit->redraw();
					}			
				break;

			case 2:
				//Global Freq Env, A.dt;
				if (master)
					master->part[par->nPart]->kit[0].subpars->FreqEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->freqenvelopegroup->visible()){					
						masterui->partui->subnoteui->freqenvelopegroup->refresh();
						masterui->partui->subnoteui->freqenvelopegroup->freeedit->redraw();
					}				
				break;		

			case 3:
				//Global Freq Env, R.val;
				if (master)
					master->part[par->nPart]->kit[0].subpars->FreqEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->freqenvelopegroup->visible()){					
						masterui->partui->subnoteui->freqenvelopegroup->refresh();
						masterui->partui->subnoteui->freqenvelopegroup->freeedit->redraw();
					}		
				break;

			case 4:
				//Global Freq Env, R.dt;
				if (master)
					master->part[par->nPart]->kit[0].subpars->FreqEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->freqenvelopegroup->visible()){					
						masterui->partui->subnoteui->freqenvelopegroup->refresh();
						masterui->partui->subnoteui->freqenvelopegroup->freeedit->redraw();
					}			
				break;

			case 5:
				//Global Freq Env, Strech;
				if (master)
					master->part[par->nPart]->kit[0].subpars->FreqEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->freqenvelopegroup->visible()){					
						masterui->partui->subnoteui->freqenvelopegroup->refresh();
						masterui->partui->subnoteui->freqenvelopegroup->freeedit->redraw();
					}			
				break;
			} //code
			break; //group

		case G_FILTENV:

			switch (par->code)
			{
			case 1:
				//Global Filter Env, A.val
				if (master)
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();
						masterui->partui->subnoteui->filterenv->freeedit->redraw();
					}		
				break;

			case 2:
				//Global Filter Env, A.dt
				if (master)
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();
						masterui->partui->subnoteui->filterenv->freeedit->redraw();
					}		
				break;

			case 3:
				//Global Filter Env, D.val
				if (master)
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					PD_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();
						masterui->partui->subnoteui->filterenv->freeedit->redraw();
					}		
				break;

			case 4:
				//Global Filter Env, D.dt
				if (master)					
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					PD_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();
						masterui->partui->subnoteui->filterenv->freeedit->redraw();
					}		
				break;

			case 5:
				//Global Filter Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();
						masterui->partui->subnoteui->filterenv->freeedit->redraw();
					}		
				break;

			case 6:
				//Global Filter Env, R.val
				if (master)
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();
						masterui->partui->subnoteui->filterenv->freeedit->redraw();
					}	
				break;

			case 7:
				//Global Filter Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].subpars->GlobalFilterEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterenv->visible()){					
						masterui->partui->subnoteui->filterenv->refresh();						
					}	
				break;
			} //code
			break; //group

		case G_BANDENV:

			switch (par->code)
			{
			case 1:
				//Global Filter Env, A.val
				if (master)
					master->part[par->nPart]->kit[0].subpars->BandWidthEnvelope->
					PA_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->bandwidthenvelopegroup->visible()){					
						masterui->partui->subnoteui->bandwidthenvelopegroup->refresh();
						masterui->partui->subnoteui->bandwidthenvelopegroup->freeedit->redraw();
					}		
				break;

			case 2:
				//Global Filter Env, A.dt
				if (master)
					master->part[par->nPart]->kit[0].subpars->BandWidthEnvelope->
					PA_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->bandwidthenvelopegroup->visible()){					
						masterui->partui->subnoteui->bandwidthenvelopegroup->refresh();
						masterui->partui->subnoteui->bandwidthenvelopegroup->freeedit->redraw();
					}		
				break;
		

			case 4:
				//Global Filter Env, R.dt
				if (master)
					master->part[par->nPart]->kit[0].subpars->BandWidthEnvelope->
					PR_dt = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->bandwidthenvelopegroup->visible()){					
						masterui->partui->subnoteui->bandwidthenvelopegroup->refresh();
						masterui->partui->subnoteui->bandwidthenvelopegroup->freeedit->redraw();
					}		
				break;

			case 3:
				//Global Filter Env, R.val
				if (master)
					master->part[par->nPart]->kit[0].subpars->BandWidthEnvelope->
					PR_val = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->bandwidthenvelopegroup->visible()){					
						masterui->partui->subnoteui->bandwidthenvelopegroup->refresh();
						masterui->partui->subnoteui->bandwidthenvelopegroup->freeedit->redraw();
					}	
				break;

			case 5:
				//Global Filter Env, Strech
				if (master)
					master->part[par->nPart]->kit[0].subpars->BandWidthEnvelope->
					Penvstretch = ScaleMidiVal(MidiVal,par);

				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->bandwidthenvelopegroup->visible()){					
						masterui->partui->subnoteui->bandwidthenvelopegroup->refresh();						
					}	
				break;
			} //code
			break; //group




		case G_FILTPARAM:

			switch (par->code)
			{
			case 1:
				//Global Filter Params, Freq
				if (master){
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->
						Pfreq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterui->visible()){
							masterui->partui->subnoteui->filterui->refresh();							
					};	
				break;

			case 2:
				//Global Filter Params, Q
				if (master){
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->
						Pq = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterui->visible()){
							masterui->partui->subnoteui->filterui->refresh();							
					};	
				break;

			case 3:
				//Global Filter Params, VelAmp
				if (master){
					master->part[par->nPart]->kit[0].subpars->
						PGlobalFilterVelocityScale = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].subpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterui->visible()){
							masterui->partui->subnoteui->filterui->refresh();							
					};	
				break;

			case 4:
				//Global Filter Params, VelSns
				if (master){
					master->part[par->nPart]->kit[0].subpars-> 
						PGlobalFilterVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].subpars->GlobalPar.GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterui->visible()){
							masterui->partui->subnoteui->filterui->refresh();							
					};	
				break;

			case 5:
				//Global Filter Params, FreqTr
				if (master){
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->
						Pfreqtrack = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterui->visible()){
							masterui->partui->subnoteui->filterui->refresh();							
					};
				break;


			case 6:
				//Global Filter Params, Gain
				if (master){
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->
						Pgain = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->GlobalFilter->b_changed = 10;
				}
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->filterui->visible()){
							masterui->partui->subnoteui->filterui->refresh();							
					};
				break;

			}  //code
			break; //group

		case G_GLOBAL:

			switch (par->code)
			{
			case 1:
				//Global Params, Volume
				if (master){
					master->part[par->nPart]->kit[0].subpars->
						PVolume = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->b_changed = 10;
				}
				break;

			case 2:
				//Global Params, Vel.Sns
				if (master){
					master->part[par->nPart]->kit[0].subpars-> 
						PAmpVelocityScaleFunction = ScaleMidiVal(MidiVal,par);					
					//master->part[par->nPart]->kit[0].padpars->b_changed = 10;
				}
				break;

			case 3:
				//Global Params, Pan
				if (master){
					master->part[par->nPart]->kit[0].subpars-> 
						PPanning = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->b_changed = 10;
				}
				break;

			case 4:
				//Global Params, BWD
				if (master){
					master->part[par->nPart]->kit[0].subpars-> 
						Pbandwidth = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->b_changed_bw = 10;
				}
				break;

			case 5:
				//Global Params, BWD.Scale
				if (master){
					master->part[par->nPart]->kit[0].subpars-> 
						Pbwscale = ScaleMidiVal(MidiVal,par);					
					master->part[par->nPart]->kit[0].subpars->b_changed_bw = 10;
				}
				break;

			} //code
				if (masterui)
					if (masterui->partui->subnoteui)
					if (masterui->partui->subnoteui->SUBparameters->visible()){
							masterui->partui->subnoteui->refresh_g();
					};	


			break; //group
			

		} // group




}




//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************


void MidiRL::GlobalTree(ParamID *par, unsigned char MidiVal) {

	if ((par->location == W_GLOBAL) && (par->group == G_GLOBAL)) {

		switch (par->code)
		{
		case 1:
			//volume
			if (master)
				master->part[par->nPart]->setPvolume(ScaleMidiVal(MidiVal,par));			
			break;

		case 2:
			//pan
			if (master)
				master->part[par->nPart]->setPpanning(ScaleMidiVal(MidiVal,par));
			break;

		case 3:
			//Pvelsns
			if (master)
				master->part[par->nPart]->Pvelsns = ScaleMidiVal(MidiVal,par);
			break;

		case 4:
			//Vel.Ofs
			if (master)
				master->part[par->nPart]->Pveloffs = ScaleMidiVal(MidiVal,par);
			break;
		}

		if (masterui)
			if (masterui->partui)
			if (masterui->partui->partgroupui->visible()){
				masterui->partui->refresh();
			};	

	}


}



void MidiRL::Update_Realtime_Parameter(int MidiCh,int MidiCtrl, unsigned char MidiVal){

	int crtComp;
	ParamID *par;

	

	//sweep assigned parameters
	for (crtComp=0; crtComp<nComp; crtComp++)	  
	  if (comp[crtComp]->ctrl == MidiCtrl)
	  if (comp[crtComp]->enabled)
      if (comp[crtComp]->ch == MidiCh)
	  {
		  //identify the parameter... long buggy stuff
		  par = comp[crtComp];		  

//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
//  PARAMETER IDENTIFICATION TREE  *
//**********************************
switch (par->type)
{	
case S_ADD:
	ADDsynthTree(par,MidiVal);
	break; //type

case S_PAD:
	PADsynthTree(par,MidiVal);
	break; //type

case S_FX:
	FxTree(par,MidiVal);
	break; //type

case S_SUB:
	SUBsynthTree(par,MidiVal);

case S_GLOBAL:
	GlobalTree(par,MidiVal);
	break;
	
} //type


//********************************************************************************************
	  } //crtComp sweep

}



void MidiRL::add2XML(XMLwrapper *xml){

	int crtComp;
	ParamID *par;

	xml->addpar("nComp",nComp);

	//sweep assigned parameters
	for (crtComp=0; crtComp<nComp; crtComp++) {
		  
		  par = comp[crtComp];		

		  //so we have a parameter ID to save to xml
		  

		  xml->beginbranch("Param",crtComp);

			//par->ch 
			xml->addpar("ch",par->ch);
			//par->code 
			xml->addpar("code",par->code);
			//par->ctrl 
			xml->addpar("ctrl",par->ctrl);
			//par->enabled 
			xml->addpar("enabled",par->enabled);
			//par->group
			xml->addpar("group",(int)par->group);
			//par->index 
			xml->addpar("index",par->index);
			//par->location 
			xml->addpar("location",(int)par->location);
			//par->max !float
			xml->addparreal("max",par->max);
			//par->maxval !float
			xml->addparreal("maxval",par->maxval);
			//par->min !float
			xml->addparreal("min",par->min);
			//par->minval !float
			xml->addparreal("minval",par->minval);
			//par->nPart 
			xml->addpar("nPart",par->nPart);
			//par->nvoice 
			xml->addpar("nvoice",par->nvoice);
			//par->strName 
			xml->addparstr("strName",par->strName);
			//par->type 
			xml->addpar("type",(int)par->type);

			
		  xml->endbranch();


	} //for

	xml->beginbranch("CheckButtons");
		//custom chk
			xml->addpar("turbo",master->b_turbo);
			xml->addpar("multich",master->b_multiCh);
	xml->endbranch();
}


void MidiRL::LoadGUI() {
	
	int crtComp;
	ParamID *par;
	char* mystr;
	
	if (masterui) {
		if (masterui->midilearn) 			
			masterui->midilearn->cleanup ();
	} else return;


	if (nComp == 0) return;

	//sweep assigned parameters
	for (crtComp=0; crtComp<nComp; crtComp++) {
		  


		//we have read a param ID
		//add it to database
		par = comp[crtComp];
		//add it to UI
		if (par)
			if (masterui)
		if (masterui->midilearn) {			
			masterui->midilearn->AddComponentComplete(par);
		};
	}	//for
		
		


};

void MidiRL::getfromXML(XMLwrapper *xml){

	int crtComp;
	ParamID *par;
	char* mystr;

	cleanup();

	if (masterui)
		if (masterui->midilearn) {			
			masterui->midilearn->cleanup ();
		};


	nComp = xml->getpar("nComp",0,0,255);

	//sweep assigned parameters
	for (crtComp=0; crtComp<nComp; crtComp++) {
		  
		xml->enterbranch("Param",crtComp);
		  par = new ParamID;

		  //ch
		  par->ch = xml->getpar("ch",-1,-1,127);
		  //code
		  par->code = xml->getpar("code",-1,-1,127);
		  //ctrl
		  par->ctrl = xml->getpar("ctrl",-1,-1,127);
		  //enabled
		  par->enabled = xml->getpar("enabled",-1,-1,127);
		  //group
		  par->group = (GroupType)xml->getpar127("group",0);
		  //index
		  par->index = xml->getpar("index",-1,-1,127);
		  //location
		  par->location = (WhereType)xml->getpar127("location",0);
		  //max !float
		  par->max = xml->getparreal("max",0);
		  //maxval !float
		  par->maxval = xml->getparreal("maxval",0);
		  //min !float
		  par->min = xml->getparreal("min",0);
		  //minval !float
		  par->minval = xml->getparreal("minval",0);
		  //nPart
		  par->nPart = xml->getpar("nPart",-1,-1,127);
		  //nvoice
		  par->nvoice = xml->getpar("nvoice",-1,-1,127);
		  //strName
		  mystr = new char[100];
		  xml->getparstr("strName",mystr,100);
		  par->SetName((const char*)mystr);
		  delete [] mystr;
		  //type
		  par->type = (SynthType)xml->getpar127("type",0);

	    xml->exitbranch();




		//we have read a param ID
		//add it to database
		comp[crtComp] = par;
		//add it to UI
		if (masterui)
		if (masterui->midilearn) {			
			masterui->midilearn->AddComponentComplete(par);
		};
	}	//for


	xml->enterbranch("CheckButtons");
		master->b_turbo = xml->getpar("turbo",0,0,1);
		master->b_multiCh = xml->getpar("multich",0,0,1);
	xml->exitbranch();

	//refresh all parameters? naah
}