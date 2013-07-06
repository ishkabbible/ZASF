// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "SUBnoteUI.h"

#include "../Output/VSTaudiooutput.h"
#include "../Misc/MidiRL.h"


//int* LearningMidi;
//MidiRL* midiRL;
//Copyright (c) 2002-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later

void SUBnoteharmonic::cb_mag_i(Fl_Slider* o, void*) {
  int x=0;
if (Fl::event_button1()) x=127-(int)o->value();
   else o->value(127-x);
pars->Phmag[n]=x;
if (pars->Phmag[n]==0) o->selection_color(0);
    else o->selection_color(222);
}
void SUBnoteharmonic::cb_mag(Fl_Slider* o, void* v) {
  ((SUBnoteharmonic*)(o->parent()->user_data()))->cb_mag_i(o,v);
}

void SUBnoteharmonic::cb_bw_i(Fl_Slider* o, void*) {
  int x=64;
if (Fl::event_button1()) x=127-(int)o->value();
   else o->value(x);
pars->Phrelbw[n]=x;
}
void SUBnoteharmonic::cb_bw(Fl_Slider* o, void* v) {
  ((SUBnoteharmonic*)(o->parent()->user_data()))->cb_bw_i(o,v);
}

Fl_Group* SUBnoteharmonic::make_window() {
  { harmonic = new Fl_Group(0, 0, 90, 225);
    harmonic->box(FL_FLAT_BOX);
    harmonic->color(FL_BACKGROUND_COLOR);
    harmonic->selection_color(FL_BACKGROUND_COLOR);
    harmonic->labeltype(FL_NO_LABEL);
    harmonic->labelfont(0);
    harmonic->labelsize(14);
    harmonic->labelcolor(FL_FOREGROUND_COLOR);
    harmonic->user_data((void*)(this));
    harmonic->align(FL_ALIGN_TOP);
    harmonic->when(FL_WHEN_RELEASE);
    { Fl_Slider* o = mag = new Fl_Slider(0, 15, 10, 115);
      mag->tooltip("harmonic\'s magnitude");
      mag->type(4);
      mag->box(FL_FLAT_BOX);
      mag->selection_color((Fl_Color)222);
      mag->maximum(127);
      mag->step(1);
      mag->value(127);
      mag->callback((Fl_Callback*)cb_mag);
      o->value(127-pars->Phmag[n]);
      if (pars->Phmag[n]==0) o->selection_color(0);
    } // Fl_Slider* mag
    { Fl_Slider* o = bw = new Fl_Slider(0, 135, 10, 75);
      bw->tooltip("harmonic\'s bandwidth");
      bw->type(4);
      bw->box(FL_FLAT_BOX);
      bw->selection_color((Fl_Color)222);
      bw->maximum(127);
      bw->step(1);
      bw->value(64);
      bw->callback((Fl_Callback*)cb_bw);
      o->value(127-pars->Phrelbw[n]);
    } // Fl_Slider* bw
    { Fl_Box* o = new Fl_Box(10, 170, 5, 5);
      o->box(FL_FLAT_BOX);
      o->color(FL_DARK2);
      if (n+1==MAX_SUB_HARMONICS) o->hide();
    } // Fl_Box* o
    { Fl_Box* o = new Fl_Box(0, 210, 10, 15, "01");
      o->labelfont(1);
      o->labelsize(9);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
      char tmp[10];snprintf(tmp,10,"%d",n+1);o->label(strdup(tmp));
    } // Fl_Box* o
    { Fl_Box* o = new Fl_Box(0, 0, 10, 15, "01");
      o->labelfont(1);
      o->labelsize(9);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
      char tmp[10];snprintf(tmp,10,"%d",n+1);o->label(strdup(tmp));
    } // Fl_Box* o
    harmonic->end();
  } // Fl_Group* harmonic
  return harmonic;
}

SUBnoteharmonic::SUBnoteharmonic(int x,int y, int w, int h, const char *label):Fl_Group(x,y,w,h,label) {
  n=0;
}

void SUBnoteharmonic::init(SUBnoteParameters *pars_,int n_) {
  pars=pars_;
n=n_;
make_window();
harmonic->show();
end();
}

void SUBnoteharmonic::refresh() {
  mag->value(127-pars->Phmag[n]);
if (pars->Phmag[n]==0) mag->selection_color(0);
bw->value(127-pars->Phrelbw[n]);
}

SUBnoteharmonic::~SUBnoteharmonic() {
  harmonic->hide();
hide();
//delete(harmonic);
}

void SUBnoteUI::cb_Close_i(Fl_Button*, void*) {
  SUBparameters->hide();
}
void SUBnoteUI::cb_Close(Fl_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->user_data()))->cb_Close_i(o,v);
}

void SUBnoteUI::cb_vol_i(Fl_Value_Slider* o, void*) {
	static int nParIndex = -2;
if (nParIndex<0)
  pars->PVolume=(int)o->value();
  pars->b_changed = 10;

  if ((*LearningMidi)||(nParIndex!=-1)||(1)){
	  
	  ParamID *paramID;
	  paramID = new ParamID;
	  {
		  paramID->code = 1;	//Volume
		  paramID->group = G_GLOBAL;
		  paramID->index = -1;
		  paramID->location = W_GLOBAL;
		  paramID->maxval = 127.0;
		  paramID->minval = 0.0;
		  paramID->nPart = pars->nPart;		  
		  paramID->nvoice = -1;
		  paramID->MidiVal = (int) (o->value() *(127/paramID->maxval ));
		  //paramID->SetName((const char*)env->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);


		  char *str1;		  
		  str1 = new char[100];
		  
		  sprintf(str1,"SubNote[Global:Vol:p%d]",paramID->nPart);
		  
		  paramID->SetName((const char*)str1);
		  delete [] str1;
		  
		  paramID->type = S_SUB;
	  }

	  
	  if (*LearningMidi==0) nParIndex = midiRL->FindComp(paramID); else  nParIndex = (*midiRL).AddComponent(paramID);

	  //delete paramID;
  }
  //else
  if (nParIndex != -1) {
	//vst automation
	  midiRL->UpdateCompValue(nParIndex,(int) (o->value()*(1)),1);


  };


}
void SUBnoteUI::cb_vol(Fl_Value_Slider* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_vol_i(o,v);
}

void SUBnoteUI::cb_vsns_i(Fl_Value_Slider* o, void*) {
	static int nParIndex = -2;
if (nParIndex<0)
  pars->PAmpVelocityScaleFunction=(int) o->value();
  
  if ((*LearningMidi)||(nParIndex!=-1)||(1)){
	  
	  ParamID *paramID;
	  paramID = new ParamID;
	  {
		  paramID->code = 2;	//Vel.Sns
		  paramID->group = G_GLOBAL;
		  paramID->index = -1;
		  paramID->location = W_GLOBAL;
		  paramID->maxval = 127.0;
		  paramID->minval = 0.0;
		  paramID->nPart = pars->nPart;		  
		  paramID->nvoice = -1;
		  paramID->MidiVal = (int) (o->value() *(127/paramID->maxval ));
		  //paramID->SetName((const char*)env->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);


		  char *str1;		  
		  str1 = new char[100];
		  
		  sprintf(str1,"SubNote[Global:Vel.Sns:p%d]",paramID->nPart);
		  
		  paramID->SetName((const char*)str1);
		  delete [] str1;
		  
		  paramID->type = S_SUB;
	  }

	  
	  if (*LearningMidi==0) nParIndex = midiRL->FindComp(paramID); else  nParIndex = (*midiRL).AddComponent(paramID);

	  //delete paramID;
  }
  //else
  if (nParIndex != -1) {
	//vst automation
	  midiRL->UpdateCompValue(nParIndex,(int) (o->value()*(1)),1);


  };


}
void SUBnoteUI::cb_vsns(Fl_Value_Slider* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_vsns_i(o,v);
}

void SUBnoteUI::cb_pan_i(WidgetPDial* o, void*) {
	static int nParIndex = -2;
if (nParIndex<0)
  pars->PPanning=(int) o->value();
  pars->b_changed = 10;

  if ((*LearningMidi)||(nParIndex!=-1)||(1)){
	  
	  ParamID *paramID;
	  paramID = new ParamID;
	  {
		  paramID->code = 3;	//Pan
		  paramID->group = G_GLOBAL;
		  paramID->index = -1;
		  paramID->location = W_GLOBAL;
		  paramID->maxval = 127.0;
		  paramID->minval = 0.0;
		  paramID->nPart = pars->nPart;		  
		  paramID->nvoice = -1;
		  paramID->MidiVal = (int) (o->value() *(127/paramID->maxval ));
		  //paramID->SetName((const char*)env->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);


		  char *str1;		  
		  str1 = new char[100];
		  
		  sprintf(str1,"SubNote[Global:Pan:p%d]",paramID->nPart);
		  
		  paramID->SetName((const char*)str1);
		  delete [] str1;
		  
		  paramID->type = S_SUB;
	  }

	  
	  if (*LearningMidi==0) nParIndex = midiRL->FindComp(paramID); else  nParIndex = (*midiRL).AddComponent(paramID);

	  //delete paramID;
  }
  //else
  if (nParIndex != -1) {
	//vst automation
	  midiRL->UpdateCompValue(nParIndex,(int) (o->value()*(1)),1);


  };

}
void SUBnoteUI::cb_pan(WidgetPDial* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_pan_i(o,v);
}

void SUBnoteUI::cb_filterstages_i(Fl_Counter* o, void*) {
  pars->Pnumstages=(int) o->value();
}
void SUBnoteUI::cb_filterstages(Fl_Counter* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_filterstages_i(o,v);
}

void SUBnoteUI::cb_magtype_i(Fl_Choice* o, void*) {
  pars->Phmagtype=(int) o->value();
}
void SUBnoteUI::cb_magtype(Fl_Choice* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_magtype_i(o,v);
}

Fl_Menu_Item SUBnoteUI::menu_magtype[] = {
 {"Linear", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {"-40dB", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {"-60dB", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {"-80dB", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {"-100dB", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {0,0,0,0,0,0,0,0,0}
};

void SUBnoteUI::cb_start_i(Fl_Choice* o, void*) {
  pars->Pstart=(int) o->value();
}
void SUBnoteUI::cb_start(Fl_Choice* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_start_i(o,v);
}

Fl_Menu_Item SUBnoteUI::menu_start[] = {
 {"Zero", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {"RND", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {"Max.", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 11, 0},
 {0,0,0,0,0,0,0,0,0}
};

void SUBnoteUI::cb_freqee_i(Fl_Check_Button* o, void*) {
  pars->PFreqEnvelopeEnabled=o->value();
if (o->value()==0) freqenvelopegroup->deactivate();
    else freqenvelopegroup->activate();
o->show();
freqsettingsui->redraw();
}
void SUBnoteUI::cb_freqee(Fl_Check_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_freqee_i(o,v);
}

void SUBnoteUI::cb_octave_i(Fl_Counter* o, void*) {
  int k=(int) o->value();
if (k<0) k+=16;
pars->PCoarseDetune = k*1024+
   pars->PCoarseDetune%1024;
}
void SUBnoteUI::cb_octave(Fl_Counter* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_octave_i(o,v);
}

void SUBnoteUI::cb_coarsedet_i(Fl_Counter* o, void*) {
  int k=(int) o->value();
if (k<0) k+=1024;
pars->PCoarseDetune = k+
   (pars->PCoarseDetune/1024)*1024;
}
void SUBnoteUI::cb_coarsedet(Fl_Counter* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_coarsedet_i(o,v);
}

void SUBnoteUI::cb_detune_i(Fl_Slider* o, void*) {
  pars->PDetune=(int)o->value()+8192;
detunevalueoutput->do_callback();
}
void SUBnoteUI::cb_detune(Fl_Slider* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_detune_i(o,v);
}

void SUBnoteUI::cb_detunevalueoutput_i(Fl_Value_Output* o, void*) {
  o->value(getdetune(pars->PDetuneType,0,pars->PDetune));
}
void SUBnoteUI::cb_detunevalueoutput(Fl_Value_Output* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_detunevalueoutput_i(o,v);
}

void SUBnoteUI::cb_hz440_i(Fl_Check_Button* o, void*) {
  int x=(int) o->value();
pars->Pfixedfreq=x;
if (x==0) fixedfreqetdial->deactivate();
   else fixedfreqetdial->activate();
}
void SUBnoteUI::cb_hz440(Fl_Check_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_hz440_i(o,v);
}

void SUBnoteUI::cb_fixedfreqetdial_i(WidgetPDial* o, void*) {
  pars->PfixedfreqET=(int) o->value();
}
void SUBnoteUI::cb_fixedfreqetdial(WidgetPDial* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_fixedfreqetdial_i(o,v);
}

void SUBnoteUI::cb_detunetype_i(Fl_Choice* o, void*) {
  pars->PDetuneType=(int) o->value()+1;
detunevalueoutput->do_callback();
}
void SUBnoteUI::cb_detunetype(Fl_Choice* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_detunetype_i(o,v);
}

void SUBnoteUI::cb_stereo_i(Fl_Check_Button* o, void*) {
  pars->Pstereo=(int) o->value();
}
void SUBnoteUI::cb_stereo(Fl_Check_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->user_data()))->cb_stereo_i(o,v);
}

void SUBnoteUI::cb_Clear_i(Fl_Button*, void*) {
  for (int i=0;i<MAX_SUB_HARMONICS;i++){
    h[i]->mag->value(127);
    pars->Phmag[i]=0;
    h[i]->bw->value(64);
    pars->Phrelbw[i]=64;
};
pars->Phmag[0]=127;
h[0]->mag->value(0);
SUBparameters->redraw();
}
void SUBnoteUI::cb_Clear(Fl_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->user_data()))->cb_Clear_i(o,v);
}

void SUBnoteUI::cb_bwee_i(Fl_Check_Button* o, void*) {
  pars->PBandWidthEnvelopeEnabled=o->value();
if (o->value()==0) bandwidthenvelopegroup->deactivate();
    else bandwidthenvelopegroup->activate();
o->show();
bandwidthsettingsui->redraw();
}
void SUBnoteUI::cb_bwee(Fl_Check_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_bwee_i(o,v);
}

void SUBnoteUI::cb_bandwidth_i(Fl_Value_Slider* o, void*) {
	static int nParIndex = -2;
if (nParIndex<0)
  pars->Pbandwidth=(int) o->value();
  pars->b_changed_bw = 10;

  if ((*LearningMidi)||(nParIndex!=-1)||(1)){
	  
	  ParamID *paramID;
	  paramID = new ParamID;
	  {
		  paramID->code = 4;	//Bwd
		  paramID->group = G_GLOBAL;
		  paramID->index = -1;
		  paramID->location = W_GLOBAL;
		  paramID->maxval = 127.0;
		  paramID->minval = 0.0;
		  paramID->nPart = pars->nPart;		  
		  paramID->nvoice = -1;
		  paramID->MidiVal = (int) (o->value() *(127/paramID->maxval ));
		  //paramID->SetName((const char*)env->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);


		  char *str1;		  
		  str1 = new char[100];
		  
		  sprintf(str1,"SubNote[Global:Bwd:p%d]",paramID->nPart);
		  
		  paramID->SetName((const char*)str1);
		  delete [] str1;
		  
		  paramID->type = S_SUB;
	  }

	  
	  if (*LearningMidi==0) nParIndex = midiRL->FindComp(paramID); else  nParIndex = (*midiRL).AddComponent(paramID);

	  //delete paramID;
  }
  //else
  if (nParIndex != -1) {
	//vst automation
	  midiRL->UpdateCompValue(nParIndex,(int) (o->value()*(1)),1);


  };

}
void SUBnoteUI::cb_bandwidth(Fl_Value_Slider* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_bandwidth_i(o,v);
}

void SUBnoteUI::cb_bwidthscale_i(Fl_Value_Slider* o, void*) {
	static int nParIndex = -2;
if (nParIndex<0)
  pars->Pbwscale=(int) o->value()+64; //!!!!+64
  pars->b_changed = 10;

  if ((*LearningMidi)||(nParIndex!=-1)||(1)){
	  
	  ParamID *paramID;
	  paramID = new ParamID;
	  {
		  paramID->code = 5;	//Bwd.Scale
		  paramID->group = G_GLOBAL;
		  paramID->index = -1;
		  paramID->location = W_GLOBAL;
		  paramID->maxval = 127.0;
		  paramID->minval = 0.0;
		  paramID->nPart = pars->nPart;		  
		  paramID->nvoice = -1;
		  paramID->MidiVal = (int) (o->value() *(127/paramID->maxval ));
		  //paramID->SetName((const char*)env->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);
		  //paramID->SetName((const char*)pars->parID->strName);


		  char *str1;		  
		  str1 = new char[100];
		  
		  sprintf(str1,"SubNote[Global:Bwd.Scale:p%d]",paramID->nPart);
		  
		  paramID->SetName((const char*)str1);
		  delete [] str1;
		  
		  paramID->type = S_SUB;
	  }

	  
	  if (*LearningMidi==0) nParIndex = midiRL->FindComp(paramID); else  nParIndex = (*midiRL).AddComponent(paramID);

	  //delete paramID;
  }
  //else
  if (nParIndex != -1) {
	//vst automation
	  midiRL->UpdateCompValue(nParIndex,(int) (o->value()*(1)),1);


  };
}
void SUBnoteUI::cb_bwidthscale(Fl_Value_Slider* o, void* v) {
  ((SUBnoteUI*)(o->parent()->parent()->user_data()))->cb_bwidthscale_i(o,v);
}

void SUBnoteUI::cb_filtere_i(Fl_Check_Button* o, void*) {
  pars->PGlobalFilterEnabled=o->value();
if (o->value()==0) globalfiltergroup->deactivate();
    else globalfiltergroup->activate();
o->show();
globalfiltergroup->redraw();
}
void SUBnoteUI::cb_filtere(Fl_Check_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->user_data()))->cb_filtere_i(o,v);
}

void SUBnoteUI::cb_C_i(Fl_Button*, void*) {
  presetsui->copy(pars);
}
void SUBnoteUI::cb_C(Fl_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->user_data()))->cb_C_i(o,v);
}

void SUBnoteUI::cb_P_i(Fl_Button*, void*) {
  presetsui->paste(pars,this);
}
void SUBnoteUI::cb_P(Fl_Button* o, void* v) {
  ((SUBnoteUI*)(o->parent()->user_data()))->cb_P_i(o,v);
}

Fl_Double_Window* SUBnoteUI::make_window() {
  { SUBparameters = new Fl_Double_Window(735, 390, "SUBsynth Parameters");
    SUBparameters->user_data((void*)(this));
    { Fl_Scroll* o = new Fl_Scroll(5, 140, 435, 245);
      o->type(1);
      o->box(FL_THIN_UP_BOX);
      { Fl_Pack* o = harmonics = new Fl_Pack(10, 145, 425, 235);
        harmonics->type(1);
        for (int i=0;i<MAX_SUB_HARMONICS;i++){h[i]=new SUBnoteharmonic(0,0,15,o->h(),"");h[i]->init(pars,i);}
        harmonics->end();
      } // Fl_Pack* harmonics
      o->end();
    } // Fl_Scroll* o
    { Fl_Button* o = new Fl_Button(625, 365, 105, 20, "Close");
      o->box(FL_THIN_UP_BOX);
      o->labelfont(1);
      o->labelsize(11);
      o->callback((Fl_Callback*)cb_Close);
    } // Fl_Button* o
    { Fl_Group* o = new Fl_Group(5, 5, 215, 135, "AMPLITUDE");
      o->box(FL_THIN_UP_FRAME);
      o->labeltype(FL_EMBOSSED_LABEL);
      o->labelfont(1);
      o->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
      { Fl_Value_Slider* o = vol = new Fl_Value_Slider(10, 25, 140, 15, "Vol");
        vol->tooltip("Volume");
        vol->type(5);
        vol->box(FL_FLAT_BOX);
        vol->labelsize(11);
        vol->maximum(127);
        vol->step(1);
        vol->callback((Fl_Callback*)cb_vol);
        vol->align(FL_ALIGN_RIGHT);
        o->value(pars->PVolume);
      } // Fl_Value_Slider* vol
      { Fl_Value_Slider* o = vsns = new Fl_Value_Slider(10, 45, 140, 15, "V.Sns");
        vsns->tooltip("Velocity Sensing Function (rightmost to disable)");
        vsns->type(5);
        vsns->box(FL_FLAT_BOX);
        vsns->labelsize(11);
        vsns->maximum(127);
        vsns->step(1);
        vsns->callback((Fl_Callback*)cb_vsns);
        vsns->align(FL_ALIGN_RIGHT);
        o->value(pars->PAmpVelocityScaleFunction);
      } // Fl_Value_Slider* vsns
      { WidgetPDial* o = pan = new WidgetPDial(master, 185, 20, 30, 30, "Pan");
        pan->tooltip("Panning (leftmost is Random)");
        pan->box(FL_ROUND_UP_BOX);
        pan->color(FL_BACKGROUND_COLOR);
        pan->selection_color(FL_INACTIVE_COLOR);
        pan->labeltype(FL_NORMAL_LABEL);
        pan->labelfont(0);
        pan->labelsize(10);
        pan->labelcolor(FL_FOREGROUND_COLOR);
        pan->maximum(127);
        pan->step(1);
        pan->callback((Fl_Callback*)cb_pan);
        pan->align(FL_ALIGN_BOTTOM);
        pan->when(FL_WHEN_CHANGED);
        o->value(pars->PPanning);
      } // WidgetPDial* pan
      { EnvelopeUI* o = ampenv = new EnvelopeUI(10, 65, 205, 70, "SUBsynth - Amplitude Envelope");
        ampenv->box(FL_FLAT_BOX);
        ampenv->color((Fl_Color)51);
        ampenv->selection_color(FL_BACKGROUND_COLOR);
        ampenv->labeltype(FL_NORMAL_LABEL);
        ampenv->labelfont(0);
        ampenv->labelsize(14);
        ampenv->labelcolor(FL_FOREGROUND_COLOR);
        ampenv->align(FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
        ampenv->when(FL_WHEN_RELEASE);
        o->init(pars->AmpEnvelope,master);
        ampenv->end();
      } // EnvelopeUI* ampenv
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(495, 325, 235, 35);
      o->box(FL_THIN_UP_FRAME);
      { Fl_Counter* o = filterstages = new Fl_Counter(515, 340, 45, 15, "Filter Stages");
        filterstages->tooltip("How many times the noise is filtered");
        filterstages->type(1);
        filterstages->labelfont(1);
        filterstages->labelsize(10);
        filterstages->minimum(1);
        filterstages->maximum(5);
        filterstages->step(1);
        filterstages->textsize(10);
        filterstages->callback((Fl_Callback*)cb_filterstages);
        filterstages->align(FL_ALIGN_TOP);
        o->value(pars->Pnumstages);
      } // Fl_Counter* filterstages
      { Fl_Choice* o = magtype = new Fl_Choice(585, 340, 65, 15, "Mag.Type");
        magtype->down_box(FL_BORDER_BOX);
        magtype->labelfont(1);
        magtype->labelsize(10);
        magtype->textsize(11);
        magtype->callback((Fl_Callback*)cb_magtype);
        magtype->align(FL_ALIGN_TOP);
        magtype->menu(menu_magtype);
        o->value(pars->Phmagtype);
      } // Fl_Choice* magtype
      { Fl_Choice* o = start = new Fl_Choice(670, 340, 50, 15, "Start");
        start->down_box(FL_BORDER_BOX);
        start->labelfont(1);
        start->labelsize(10);
        start->textsize(11);
        start->callback((Fl_Callback*)cb_start);
        start->align(FL_ALIGN_TOP);
        start->menu(menu_start);
        o->value(pars->Pstart);
      } // Fl_Choice* start
      o->end();
    } // Fl_Group* o
    { freqsettingsui = new Fl_Group(440, 5, 290, 135, "FREQUENCY");
      freqsettingsui->box(FL_THIN_UP_FRAME);
      freqsettingsui->labeltype(FL_EMBOSSED_LABEL);
      freqsettingsui->labelfont(1);
      freqsettingsui->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
      { EnvelopeUI* o = freqenvelopegroup = new EnvelopeUI(445, 65, 205, 70, "SUBsynth - Frequency Envelope");
        freqenvelopegroup->box(FL_FLAT_BOX);
        freqenvelopegroup->color((Fl_Color)51);
        freqenvelopegroup->selection_color(FL_BACKGROUND_COLOR);
        freqenvelopegroup->labeltype(FL_NORMAL_LABEL);
        freqenvelopegroup->labelfont(0);
        freqenvelopegroup->labelsize(14);
        freqenvelopegroup->labelcolor(FL_FOREGROUND_COLOR);
        freqenvelopegroup->align(FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
        freqenvelopegroup->when(FL_WHEN_RELEASE);
        o->init(pars->FreqEnvelope,master);
        if (pars->PFreqEnvelopeEnabled==0) o->deactivate();
        freqenvelopegroup->end();
      } // EnvelopeUI* freqenvelopegroup
      { Fl_Check_Button* o = freqee = new Fl_Check_Button(445, 68, 55, 15, "Enabled");
        freqee->down_box(FL_DOWN_BOX);
        freqee->labelfont(1);
        freqee->labelsize(10);
        freqee->callback((Fl_Callback*)cb_freqee);
        o->value(pars->PFreqEnvelopeEnabled);
      } // Fl_Check_Button* freqee
      { Fl_Counter* o = octave = new Fl_Counter(670, 50, 45, 15, "Octave");
        octave->tooltip("Octave");
        octave->type(1);
        octave->labelsize(10);
        octave->minimum(-8);
        octave->maximum(7);
        octave->step(1);
        octave->textfont(1);
        octave->textsize(11);
        octave->callback((Fl_Callback*)cb_octave);
        octave->align(FL_ALIGN_TOP);
        int k=pars->PCoarseDetune/1024;if (k>=8) k-=16;
        o->value(k);
      } // Fl_Counter* octave
      { Fl_Counter* o = coarsedet = new Fl_Counter(655, 115, 60, 20, "Coarse Det.");
        coarsedet->tooltip("Coarse Detune");
        coarsedet->labelsize(10);
        coarsedet->minimum(-64);
        coarsedet->maximum(63);
        coarsedet->step(1);
        coarsedet->textfont(1);
        coarsedet->textsize(11);
        coarsedet->callback((Fl_Callback*)cb_coarsedet);
        coarsedet->align(FL_ALIGN_TOP);
        int k=pars->PCoarseDetune%1024;if (k>=512) k-=1024;
        o->value(k);
        o->lstep(10);
      } // Fl_Counter* coarsedet
      { Fl_Slider* o = detune = new Fl_Slider(495, 25, 230, 15);
        detune->tooltip("Fine Detune (cents)");
        detune->type(5);
        detune->box(FL_FLAT_BOX);
        detune->minimum(-8192);
        detune->maximum(8191);
        detune->step(1);
        detune->callback((Fl_Callback*)cb_detune);
        o->value(pars->PDetune-8192);
      } // Fl_Slider* detune
      { Fl_Value_Output* o = detunevalueoutput = new Fl_Value_Output(448, 25, 45, 15, "Detune");
        detunevalueoutput->labelsize(10);
        detunevalueoutput->minimum(-5000);
        detunevalueoutput->maximum(5000);
        detunevalueoutput->step(0.01);
        detunevalueoutput->textfont(1);
        detunevalueoutput->textsize(10);
        detunevalueoutput->callback((Fl_Callback*)cb_detunevalueoutput);
        detunevalueoutput->align(FL_ALIGN_TOP_LEFT);
        o->value(getdetune(pars->PDetuneType,0,pars->PDetune));
      } // Fl_Value_Output* detunevalueoutput
      { Fl_Check_Button* o = hz440 = new Fl_Check_Button(555, 45, 50, 15, "440Hz");
        hz440->tooltip("set the base frequency to 440Hz");
        hz440->down_box(FL_DOWN_BOX);
        hz440->labelfont(1);
        hz440->labelsize(10);
        hz440->callback((Fl_Callback*)cb_hz440);
        o->value(pars->Pfixedfreq);
      } // Fl_Check_Button* hz440
      { WidgetPDial* o = fixedfreqetdial = new WidgetPDial(master,610, 45, 15, 15, "Eq.T.");
        fixedfreqetdial->tooltip("How the frequency varies acording to the keyboard (leftmost for fixed frequen\
cy)");
        fixedfreqetdial->box(FL_ROUND_UP_BOX);
        fixedfreqetdial->color(FL_BACKGROUND_COLOR);
        fixedfreqetdial->selection_color(FL_INACTIVE_COLOR);
        fixedfreqetdial->labeltype(FL_NORMAL_LABEL);
        fixedfreqetdial->labelfont(0);
        fixedfreqetdial->labelsize(10);
        fixedfreqetdial->labelcolor(FL_FOREGROUND_COLOR);
        fixedfreqetdial->maximum(127);
        fixedfreqetdial->step(1);
        fixedfreqetdial->callback((Fl_Callback*)cb_fixedfreqetdial);
        fixedfreqetdial->align(FL_ALIGN_RIGHT);
        fixedfreqetdial->when(FL_WHEN_CHANGED);
        o->value(pars->PfixedfreqET);
        if (pars->Pfixedfreq==0) o->deactivate();
      } // WidgetPDial* fixedfreqetdial
      { Fl_Choice* o = detunetype = new Fl_Choice(655, 85, 70, 15, "Detune Type");
        detunetype->down_box(FL_BORDER_BOX);
        detunetype->labelsize(10);
        detunetype->textfont(1);
        detunetype->textsize(10);
        detunetype->callback((Fl_Callback*)cb_detunetype);
        detunetype->align(FL_ALIGN_TOP_LEFT);
        o->add("L35cents");o->add("L10cents");o->add("E100cents");o->add("E1200cents");
        o->value(pars->PDetuneType-1);
      } // Fl_Choice* detunetype
      freqsettingsui->end();
    } // Fl_Group* freqsettingsui
    { Fl_Check_Button* o = stereo = new Fl_Check_Button(440, 325, 55, 35, "Stereo");
      stereo->box(FL_THIN_UP_BOX);
      stereo->down_box(FL_DOWN_BOX);
      stereo->labelfont(1);
      stereo->labelsize(10);
      stereo->callback((Fl_Callback*)cb_stereo);
      o->value(pars->Pstereo);
    } // Fl_Check_Button* stereo
    { Fl_Button* o = new Fl_Button(445, 365, 70, 20, "Clear");
      o->tooltip("Clear the harmonics");
      o->box(FL_THIN_UP_BOX);
      o->labelfont(1);
      o->labelsize(11);
      o->callback((Fl_Callback*)cb_Clear);
    } // Fl_Button* o
    { bandwidthsettingsui = new Fl_Group(220, 5, 220, 135, "BANDWIDTH");
      bandwidthsettingsui->box(FL_THIN_UP_FRAME);
      bandwidthsettingsui->labeltype(FL_EMBOSSED_LABEL);
      bandwidthsettingsui->labelfont(1);
      bandwidthsettingsui->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
      { EnvelopeUI* o = bandwidthenvelopegroup = new EnvelopeUI(225, 65, 205, 70, "SUBsynth - BandWidth Envelope");
        bandwidthenvelopegroup->box(FL_FLAT_BOX);
        bandwidthenvelopegroup->color((Fl_Color)51);
        bandwidthenvelopegroup->selection_color(FL_BACKGROUND_COLOR);
        bandwidthenvelopegroup->labeltype(FL_NORMAL_LABEL);
        bandwidthenvelopegroup->labelfont(0);
        bandwidthenvelopegroup->labelsize(14);
        bandwidthenvelopegroup->labelcolor(FL_FOREGROUND_COLOR);
        bandwidthenvelopegroup->align(FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
        bandwidthenvelopegroup->when(FL_WHEN_RELEASE);
        o->init(pars->BandWidthEnvelope,master);
        if (pars->PBandWidthEnvelopeEnabled==0) o->deactivate();
        bandwidthenvelopegroup->end();
      } // EnvelopeUI* bandwidthenvelopegroup
      { Fl_Check_Button* o = bwee = new Fl_Check_Button(225, 67, 55, 15, "Enabled");
        bwee->down_box(FL_DOWN_BOX);
        bwee->labelfont(1);
        bwee->labelsize(10);
        bwee->callback((Fl_Callback*)cb_bwee);
        o->value(pars->PBandWidthEnvelopeEnabled);
      } // Fl_Check_Button* bwee
      { Fl_Value_Slider* o = bandwidth = new Fl_Value_Slider(225, 40, 115, 15, "Band Width");
        bandwidth->type(5);
        bandwidth->box(FL_FLAT_BOX);
        bandwidth->labelsize(10);
        bandwidth->maximum(127);
        bandwidth->step(1);
        bandwidth->callback((Fl_Callback*)cb_bandwidth);
        bandwidth->align(FL_ALIGN_TOP);
        o->value(pars->Pbandwidth);
      } // Fl_Value_Slider* bandwidth
      { Fl_Value_Slider* o = bwidthscale = new Fl_Value_Slider(345, 40, 90, 15, "B.Width Scale");
        bwidthscale->tooltip("How much I increase the BandWidth according to lower/higher harmonics");
        bwidthscale->type(5);
        bwidthscale->box(FL_FLAT_BOX);
        bwidthscale->labelsize(10);
        bwidthscale->minimum(-64);
        bwidthscale->maximum(63);
        bwidthscale->step(1);
        bwidthscale->callback((Fl_Callback*)cb_bwidthscale);
        bwidthscale->align(FL_ALIGN_TOP);
        o->value(pars->Pbwscale-64);
      } // Fl_Value_Slider* bwidthscale
      bandwidthsettingsui->end();
    } // Fl_Group* bandwidthsettingsui
    { Fl_Group* o = globalfiltergroup = new Fl_Group(440, 140, 290, 185, "FILTER");
      globalfiltergroup->box(FL_THIN_UP_FRAME);
      globalfiltergroup->labeltype(FL_EMBOSSED_LABEL);
      globalfiltergroup->labelfont(1);
      globalfiltergroup->labelsize(13);
      globalfiltergroup->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
      { EnvelopeUI* o = filterenv = new EnvelopeUI(445, 250, 275, 70, "SUBsynth - Filter Envelope");
        filterenv->box(FL_FLAT_BOX);
        filterenv->color((Fl_Color)51);
        filterenv->selection_color(FL_BACKGROUND_COLOR);
        filterenv->labeltype(FL_NORMAL_LABEL);
        filterenv->labelfont(0);
        filterenv->labelsize(14);
        filterenv->labelcolor(FL_FOREGROUND_COLOR);
        filterenv->align(FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
        filterenv->when(FL_WHEN_RELEASE);
        o->init(pars->GlobalFilterEnvelope,master);
        filterenv->end();
      } // EnvelopeUI* filterenv
      { FilterUI* o = filterui = new FilterUI(445, 170, 275, 75, "SUBsynthl - Filter");
        filterui->box(FL_FLAT_BOX);
        filterui->color(FL_LIGHT1);
        filterui->selection_color(FL_BACKGROUND_COLOR);
        filterui->labeltype(FL_NORMAL_LABEL);
        filterui->labelfont(0);
        filterui->labelsize(14);
        filterui->labelcolor(FL_FOREGROUND_COLOR);
        filterui->align(FL_ALIGN_WRAP|FL_ALIGN_INSIDE);
        filterui->when(FL_WHEN_RELEASE);
        o->init(pars->GlobalFilter,&pars->PGlobalFilterVelocityScale,&pars->PGlobalFilterVelocityScaleFunction,master);
        filterui->end();
      } // FilterUI* filterui
      if (pars->PGlobalFilterEnabled==0) o->deactivate();
      globalfiltergroup->end();
    } // Fl_Group* globalfiltergroup
    { Fl_Check_Button* o = filtere = new Fl_Check_Button(445, 145, 85, 20, "Enabled");
      filtere->down_box(FL_DOWN_BOX);
      filtere->labelfont(1);
      filtere->labelsize(11);
      filtere->callback((Fl_Callback*)cb_filtere);
      o->value(pars->PGlobalFilterEnabled);
    } // Fl_Check_Button* filtere
    { Fl_Button* o = new Fl_Button(540, 370, 25, 15, "C");
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)179);
      o->labelfont(1);
      o->labelsize(11);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->callback((Fl_Callback*)cb_C);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(570, 370, 25, 15, "P");
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)179);
      o->labelfont(1);
      o->labelsize(11);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->callback((Fl_Callback*)cb_P);
    } // Fl_Button* o
    SUBparameters->end();
  } // Fl_Double_Window* SUBparameters
  return SUBparameters;
}

void SUBnoteUI::refresh() {
  for (int i=0;i<MAX_SUB_HARMONICS;i++) h[i]->refresh();
vol->value(pars->PVolume);
vsns->value(pars->PAmpVelocityScaleFunction);
pan->value(pars->PPanning);


bandwidth->value(pars->Pbandwidth);
bwidthscale->value(pars->Pbwscale-64);
bwee->value(pars->PBandWidthEnvelopeEnabled);
if (pars->PBandWidthEnvelopeEnabled==0) bandwidthenvelopegroup->deactivate();
    else bandwidthenvelopegroup->activate();
bwee->show();
bandwidthsettingsui->redraw();

detunevalueoutput->value(getdetune(pars->PDetuneType,0,pars->PDetune));
freqee->value(pars->PFreqEnvelopeEnabled);
if (pars->PFreqEnvelopeEnabled==0) freqenvelopegroup->deactivate();
    else freqenvelopegroup->activate();
freqee->show();
freqsettingsui->redraw();

detune->value(pars->PDetune-8192);
hz440->value(pars->Pfixedfreq);

fixedfreqetdial->value(pars->PfixedfreqET);

int k=pars->PCoarseDetune/1024;if (k>=8) k-=16;
octave->value(k);

detunetype->value(pars->PDetuneType-1);

k=pars->PCoarseDetune%1024;if (k>=512) k-=1024;
coarsedet->value(k);

filtere->value(pars->PGlobalFilterEnabled);
if (pars->PGlobalFilterEnabled==0) globalfiltergroup->deactivate();
    else globalfiltergroup->activate();
filtere->show();
globalfiltergroup->redraw();

stereo->value(pars->Pstereo);
filterstages->value(pars->Pnumstages);
magtype->value(pars->Phmagtype);
start->value(pars->Pstart);

ampenv->refresh();
bandwidthenvelopegroup->refresh();
freqenvelopegroup->refresh();
filterui->refresh();
filterenv->refresh();
}

void SUBnoteUI::refresh_g() {  
vol->value(pars->PVolume);
vsns->value(pars->PAmpVelocityScaleFunction);
pan->value(pars->PPanning);


bandwidth->value(pars->Pbandwidth);
bwidthscale->value(pars->Pbwscale-64);
}

SUBnoteUI::SUBnoteUI(SUBnoteParameters *parameters, Master* master_) {
	master = master_;
	LearningMidi =( master->LearningMidi);
	midiRL = master->theVST->midiRL;
  pars=parameters;
make_window();
}

SUBnoteUI::~SUBnoteUI() {
  //for (int i=0;i<MAX_SUB_HARMONICS;i++) delete (h[i]);
SUBparameters->hide();
delete(SUBparameters);
}
