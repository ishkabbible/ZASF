// generated by Fast Light User Interface Designer (fluid) version 1.0108

#ifndef OscilGenUI_h
#define OscilGenUI_h
#include <FL/Fl.H>
#include "../Synth/OscilGen.h"
#include "../Misc/Util.h"
#include "../Misc/Master.h"
#include "ResonanceUI.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Slider.H>
#include "WidgetPDial.h"
#include "EnvelopeUI.h"
#include "LFOUI.h"
#include "FilterUI.h"
#include "PresetsUI.h"

class OscilSpectrum : public Fl_Box {
public:
	int* LearningMidi;
  MidiRL* midiRL;
  OscilSpectrum(int x,int y, int w, int h, const char *label=0);
  void init(OscilGen *oscil_,int oscbase_,Master *master_);
  void draw();
private:
  OscilGen *oscil; 
  int oscbase; 
  Master *master; 
};

class PSlider : public Fl_Slider {
public:
  PSlider(int x,int y, int w, int h, const char *label=0);
  int handle(int event);
};

class Oscilloscope : public Fl_Box {
public:
	int* LearningMidi;
  MidiRL* midiRL;
  Oscilloscope(int x,int y, int w, int h, const char *label=0);
  void init(OscilGen *oscil_,Master *master_);
  void init(OscilGen *oscil_,int oscbase_,Master *master_);
  void init(OscilGen *oscil_,int oscbase_,int phase_,Master *master_);
  void draw();
private:
  OscilGen *oscil; 
  int oscbase; 
public:
  int phase; 
private:
  Master *master; 
};
#include <FL/Fl_Box.H>

class Oscilharmonic : public Fl_Group {
  Fl_Group* make_window();
  Fl_Group *harmonic;
public:
	int* LearningMidi;
  MidiRL* midiRL;
  PSlider *mag;
private:
  void cb_mag_i(PSlider*, void*);
  static void cb_mag(PSlider*, void*);
public:
  PSlider *phase;
private:
  void cb_phase_i(PSlider*, void*);
  static void cb_phase(PSlider*, void*);
public:
  Oscilharmonic(int x,int y, int w, int h, const char *label=0);
  void init(OscilGen *oscil_,int n_,Fl_Group *display_,Fl_Widget *oldosc_,Fl_Widget *cbwidget_,Fl_Widget *applybutton_, Master *master_);
  void refresh();
  ~Oscilharmonic();
private:
  OscilGen *oscil; 
  Fl_Group *display; 
  int n; 
  Fl_Widget *oldosc,*cbwidget,*applybutton; 
  Master *master; 
};
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>

class OscilEditor : public PresetsUI_ {
public:
	int* LearningMidi;
  MidiRL* midiRL;
  Fl_Double_Window* make_window();
  Fl_Double_Window *osceditUI;
  Fl_Button *applybutton;
private:
  void cb_applybutton_i(Fl_Button*, void*);
  static void cb_applybutton(Fl_Button*, void*);
public:
  Fl_Group *oscildisplaygroup;
  Fl_Value_Slider *rndslider;
private:
  void cb_rndslider_i(Fl_Value_Slider*, void*);
  static void cb_rndslider(Fl_Value_Slider*, void*);
public:
  Fl_Choice *hrndtype;
private:
  void cb_hrndtype_i(Fl_Choice*, void*);
  static void cb_hrndtype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_hrndtype[];
public:
  WidgetPDial *hrnddial;
private:
  void cb_hrnddial_i(WidgetPDial*, void*);
  static void cb_hrnddial(WidgetPDial*, void*);
public:
  Fl_Group *basefuncdisplaygroup;
  WidgetPDial *bfslider;
private:
  void cb_bfslider_i(WidgetPDial*, void*);
  static void cb_bfslider(WidgetPDial*, void*);
public:
  Fl_Choice *bftype;
private:
  void cb_bftype_i(Fl_Choice*, void*);
  static void cb_bftype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_bftype[];
public:
  Fl_Value_Output *bfparval;
  Fl_Group *basefuncmodulation;
  Fl_Choice *bfmodtype;
private:
  void cb_bfmodtype_i(Fl_Choice*, void*);
  static void cb_bfmodtype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_bfmodtype[];
public:
  WidgetPDial *bfmodpar1;
private:
  void cb_bfmodpar1_i(WidgetPDial*, void*);
  static void cb_bfmodpar1(WidgetPDial*, void*);
public:
  WidgetPDial *bfmodpar2;
private:
  void cb_bfmodpar2_i(WidgetPDial*, void*);
  static void cb_bfmodpar2(WidgetPDial*, void*);
public:
  WidgetPDial *bfmodpar3;
private:
  void cb_bfmodpar3_i(WidgetPDial*, void*);
  static void cb_bfmodpar3(WidgetPDial*, void*);
public:
  Fl_Choice *magtype;
private:
  void cb_magtype_i(Fl_Choice*, void*);
  static void cb_magtype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_magtype[];
  void cb_Use_i(Fl_Button*, void*);
  static void cb_Use(Fl_Button*, void*);
  void cb_Close_i(Fl_Button*, void*);
  static void cb_Close(Fl_Button*, void*);
  void cb_Clear_i(Fl_Button*, void*);
  static void cb_Clear(Fl_Button*, void*);
public:
  Fl_Choice *wshbutton;
private:
  void cb_wshbutton_i(Fl_Choice*, void*);
  static void cb_wshbutton(Fl_Choice*, void*);
  static Fl_Menu_Item menu_wshbutton[];
public:
  WidgetPDial *wshpar;
private:
  void cb_wshpar_i(WidgetPDial*, void*);
  static void cb_wshpar(WidgetPDial*, void*);
public:
  Fl_Value_Output *wsparval;
  Fl_Light_Button *autoclearbutton;
  Fl_Choice *fltbutton;
private:
  void cb_fltbutton_i(Fl_Choice*, void*);
  static void cb_fltbutton(Fl_Choice*, void*);
  static Fl_Menu_Item menu_fltbutton[];
public:
  WidgetPDial *filtervalue1;
private:
  void cb_filtervalue1_i(WidgetPDial*, void*);
  static void cb_filtervalue1(WidgetPDial*, void*);
public:
  Fl_Check_Button *filterpref;
private:
  void cb_filterpref_i(Fl_Check_Button*, void*);
  static void cb_filterpref(Fl_Check_Button*, void*);
public:
  WidgetPDial *filtervalue2;
private:
  void cb_filtervalue2_i(WidgetPDial*, void*);
  static void cb_filtervalue2(WidgetPDial*, void*);
public:
  Fl_Choice *sabutton;
private:
  void cb_sabutton_i(Fl_Choice*, void*);
  static void cb_sabutton(Fl_Choice*, void*);
  static Fl_Menu_Item menu_sabutton[];
public:
  WidgetPDial *sadjpar;
private:
  void cb_sadjpar_i(WidgetPDial*, void*);
  static void cb_sadjpar(WidgetPDial*, void*);
public:
  Fl_Counter *harmonicshiftcounter;
private:
  void cb_harmonicshiftcounter_i(Fl_Counter*, void*);
  static void cb_harmonicshiftcounter(Fl_Counter*, void*);
public:
  Fl_Check_Button *harmonicshiftpre;
private:
  void cb_harmonicshiftpre_i(Fl_Check_Button*, void*);
  static void cb_harmonicshiftpre(Fl_Check_Button*, void*);
  void cb_R_i(Fl_Button*, void*);
  static void cb_R(Fl_Button*, void*);
public:
  Fl_Choice *adhrtype;
private:
  void cb_adhrtype_i(Fl_Choice*, void*);
  static void cb_adhrtype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_adhrtype[];
public:
  WidgetPDial *adhrpow;
private:
  void cb_adhrpow_i(WidgetPDial*, void*);
  static void cb_adhrpow(WidgetPDial*, void*);
public:
  WidgetPDial *adhrbf;
private:
  void cb_adhrbf_i(WidgetPDial*, void*);
  static void cb_adhrbf(WidgetPDial*, void*);
public:
  Fl_Slider *adhrpar;
private:
  void cb_adhrpar_i(Fl_Slider*, void*);
  static void cb_adhrpar(Fl_Slider*, void*);
public:
  Fl_Choice *modtype;
private:
  void cb_modtype_i(Fl_Choice*, void*);
  static void cb_modtype(Fl_Choice*, void*);
  static Fl_Menu_Item menu_modtype[];
public:
  WidgetPDial *modpar1;
private:
  void cb_modpar1_i(WidgetPDial*, void*);
  static void cb_modpar1(WidgetPDial*, void*);
public:
  WidgetPDial *modpar2;
private:
  void cb_modpar2_i(WidgetPDial*, void*);
  static void cb_modpar2(WidgetPDial*, void*);
public:
  WidgetPDial *modpar3;
private:
  void cb_modpar3_i(WidgetPDial*, void*);
  static void cb_modpar3(WidgetPDial*, void*);
  void cb_Sine_i(Fl_Button*, void*);
  static void cb_Sine(Fl_Button*, void*);
  void cb_C_i(Fl_Button*, void*);
  static void cb_C(Fl_Button*, void*);
  void cb_P_i(Fl_Button*, void*);
  static void cb_P(Fl_Button*, void*);
public:
  Fl_Scroll *_this_has_to_be_the_last;
  Fl_Pack *harmonics;
  OscilEditor(OscilGen *oscil_,Fl_Widget *oldosc_,Fl_Widget *cbwidget_,Fl_Widget *cbapplywidget_,Master *master_);
  ~OscilEditor();
  void refresh();
  void redrawoscil();
private:
  OscilGen *oscil; 
  Fl_Widget *oldosc,*cbwidget,*cbapplywidget; 
  Oscilharmonic *h[MAX_AD_HARMONICS]; 
  Master *master; 
};
#endif
