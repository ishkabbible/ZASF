// generated by Fast Light User Interface Designer (fluid) version 1.0108

#ifndef LFOUI_h
#define LFOUI_h
#include <FL/Fl.H>
#include "WidgetPDial.h"
#include <stdio.h>
#include <stdlib.h>
#include "../globals.h"
#include <FL/Fl_Group.H>
#include "../Params/LFOParams.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include "PresetsUI.h"
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include "../Misc/Master.h"

class MidiRL;

class LFOUI : public Fl_Group, PresetsUI_ {
public:
	int* LearningMidi;
  MidiRL* midiRL;
  LFOUI(int x,int y, int w, int h, const char *label=0);
  ~LFOUI();
  Fl_Group* make_window();
  Fl_Group *lfoui;
  Fl_Group *lfoparamswindow;
  WidgetPDial *freq;
private:
  void cb_freq_i(WidgetPDial*, void*);
  static void cb_freq(WidgetPDial*, void*);
public:
  WidgetPDial *intensity;
private:
  void cb_intensity_i(WidgetPDial*, void*);
  static void cb_intensity(WidgetPDial*, void*);
public:
  WidgetPDial *delay;
private:
  void cb_delay_i(WidgetPDial*, void*);
  static void cb_delay(WidgetPDial*, void*);
public:
  WidgetPDial *startphase;
private:
  void cb_startphase_i(WidgetPDial*, void*);
  static void cb_startphase(WidgetPDial*, void*);
public:
  WidgetPDial *randomness;
private:
  void cb_randomness_i(WidgetPDial*, void*);
  static void cb_randomness(WidgetPDial*, void*);
public:
  Fl_Choice *LFOtype;
private:
  void cb_LFOtype_i(Fl_Choice*, void*);
  static void cb_LFOtype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_LFOtype[];
public:
  Fl_Check_Button *continous;
private:
  void cb_continous_i(Fl_Check_Button*, void*);
  static void cb_continous(Fl_Check_Button*, void*);
public:
  WidgetPDial *freqrand;
private:
  void cb_freqrand_i(WidgetPDial*, void*);
  static void cb_freqrand(WidgetPDial*, void*);
public:
  WidgetPDial *stretch;
private:
  void cb_stretch_i(WidgetPDial*, void*);
  static void cb_stretch(WidgetPDial*, void*);
  void cb_C_i(Fl_Button*, void*);
  static void cb_C(Fl_Button*, void*);
  void cb_P_i(Fl_Button*, void*);
  static void cb_P(Fl_Button*, void*);
public:
  void refresh();
  void init(LFOParams *lfopars_, Master* master_);
  Master* master;
private:
  LFOParams *pars; 
};
#endif
