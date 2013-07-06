// generated by Fast Light User Interface Designer (fluid) version 1.0108

#ifndef VirKeyboard_h
#define VirKeyboard_h
#include <FL/Fl.H>
#include <stdlib.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include "../globals.h"
#include "../Misc/Master.h"
#include "../Input/MidiIn.h"
#include "WidgetPDial.h"

class VirKeys : public Fl_Box {
  static const int N_OCT=6; 
  static const int SIZE_WHITE=14; 
  static const int SIZE_BLACK=8; 
public:
  VirKeys(int x,int y, int w, int h, const char *label=0);
  void init(Master *master_);
  void draw();
  int handle(int event);
  void presskey(int nk,int exclusive,int type);
  void relasekey(int nk,int type);
  void relaseallkeys(int type);
private:
  Master *master; 
  int pressed[N_OCT*12+1]; 
public:
  unsigned char midich; 
  unsigned char midivel; 
  char midioct,keyoct1,keyoct2; 
  unsigned char rndvelocity; 
};
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Roller.H>

class VirKeyboard {
public:
  Fl_Double_Window* make_window();
  Fl_Double_Window *virkeyboardwindow;
private:
  void cb_virkeyboardwindow_i(Fl_Double_Window*, void*);
  static void cb_virkeyboardwindow(Fl_Double_Window*, void*);
public:
  VirKeys *virkeys;
private:
  void cb_qwer_i(Fl_Counter*, void*);
  static void cb_qwer(Fl_Counter*, void*);
  void cb_zxcv_i(Fl_Counter*, void*);
  static void cb_zxcv(Fl_Counter*, void*);
  void cb_Vel_i(Fl_Value_Slider*, void*);
  static void cb_Vel(Fl_Value_Slider*, void*);
  void cb_Oct_i(Fl_Counter*, void*);
  static void cb_Oct(Fl_Counter*, void*);
  void cb_Close_i(Fl_Button*, void*);
  static void cb_Close(Fl_Button*, void*);
  void cb_Cval_i(Fl_Value_Slider*, void*);
  static void cb_Cval(Fl_Value_Slider*, void*);
  void cb_Controller_i(Fl_Choice*, void*);
  static void cb_Controller(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Controller[];
public:
  Fl_Roller *pitchwheelroller;
private:
  void cb_pitchwheelroller_i(Fl_Roller*, void*);
  static void cb_pitchwheelroller(Fl_Roller*, void*);
  void cb_R_i(Fl_Button*, void*);
  static void cb_R(Fl_Button*, void*);
  void cb_Vrnd_i(WidgetPDial*, void*);
  static void cb_Vrnd(WidgetPDial*, void*);
public:
  Fl_Choice *partrcv;
private:
  void cb_partrcv_i(Fl_Choice*, void*);
  static void cb_partrcv(Fl_Choice*, void*);
public:
  VirKeyboard(Master *master_);
  void show();
  void hide();
  void relaseallkeys();
private:
  Master *master; 
  int midictl; 
};
#endif
