// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "SeqUI.h"
//Copyright (c) 2002-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later

void SeqUI::cb_playbutton_i(Fl_Button* o, void*) {
  o->deactivate();
stopbutton_play->activate();

master->seq.startplay();
}
void SeqUI::cb_playbutton(Fl_Button* o, void* v) {
  ((SeqUI*)(o->parent()->parent()->user_data()))->cb_playbutton_i(o,v);
}

void SeqUI::cb_stopbutton_play_i(Fl_Button* o, void*) {
  o->deactivate();
playbutton->activate();

master->seq.stopplay();
}
void SeqUI::cb_stopbutton_play(Fl_Button* o, void* v) {
  ((SeqUI*)(o->parent()->parent()->user_data()))->cb_stopbutton_play_i(o,v);
}

void SeqUI::cb_Open_i(Fl_Button*, void*) {
  master->seq.importmidifile("test.mid");
}
void SeqUI::cb_Open(Fl_Button* o, void* v) {
  ((SeqUI*)(o->parent()->user_data()))->cb_Open_i(o,v);
}

void SeqUI::cb_Play_i(Fl_Value_Slider* o, void*) {
  master->seq.setplayspeed((int) o->value());
}
void SeqUI::cb_Play(Fl_Value_Slider* o, void* v) {
  ((SeqUI*)(o->parent()->user_data()))->cb_Play_i(o,v);
}

Fl_Double_Window* SeqUI::make_window() {
  { seqwin = new Fl_Double_Window(280, 265, "Sequencer - ZynAddSubFX");
    seqwin->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(120, 20, 100, 65, "Player");
      o->box(FL_ENGRAVED_BOX);
      o->labelfont(1);
      { playbutton = new Fl_Button(130, 30, 30, 30, "Play");
        playbutton->tooltip("Start Playing");
        playbutton->box(FL_DIAMOND_UP_BOX);
        playbutton->color((Fl_Color)79);
        playbutton->labelfont(1);
        playbutton->labelsize(13);
        playbutton->callback((Fl_Callback*)cb_playbutton);
        playbutton->align(FL_ALIGN_BOTTOM);
      } // Fl_Button* playbutton
      { stopbutton_play = new Fl_Button(175, 29, 30, 31, "Stop");
        stopbutton_play->tooltip("Stop Playing");
        stopbutton_play->box(FL_THIN_UP_BOX);
        stopbutton_play->color((Fl_Color)4);
        stopbutton_play->labelfont(1);
        stopbutton_play->labelsize(13);
        stopbutton_play->callback((Fl_Callback*)cb_stopbutton_play);
        stopbutton_play->align(FL_ALIGN_BOTTOM);
        stopbutton_play->deactivate();
      } // Fl_Button* stopbutton_play
      o->end();
    } // Fl_Group* o
    { Fl_Button* o = new Fl_Button(20, 25, 75, 55, "Open test.mid");
      o->callback((Fl_Callback*)cb_Open);
      o->align(FL_ALIGN_WRAP);
    } // Fl_Button* o
    { Fl_Value_Slider* o = new Fl_Value_Slider(15, 105, 190, 20, "Play speed");
      o->type(5);
      o->minimum(-128);
      o->maximum(128);
      o->step(1);
      o->callback((Fl_Callback*)cb_Play);
      o->value(master->seq.playspeed);
    } // Fl_Value_Slider* o
    { Fl_Box* o = new Fl_Box(25, 155, 225, 90, "This is not finished");
      o->labelfont(1);
      o->labelsize(22);
      o->align(FL_ALIGN_WRAP);
    } // Fl_Box* o
    seqwin->end();
  } // Fl_Double_Window* seqwin
  return seqwin;
}

SeqUI::SeqUI(Master *master_) {
  master=master_;

make_window();
}

void SeqUI::show() {
  seqwin->show();
}
