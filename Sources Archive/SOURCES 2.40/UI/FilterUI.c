// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "FilterUI.h"
//Copyright (c) 2002-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later

FormantFilterGraph::FormantFilterGraph(int x,int y, int w, int h, const char *label):Fl_Box(x,y,w,h,label) {
  pars=NULL;
nvowel=NULL;
nformant=NULL;
graphpoints=NULL;
}

void FormantFilterGraph::init(FilterParams *pars_,int *nvowel_,int *nformant_) {
  pars=pars_;
nvowel=nvowel_;
nformant=nformant_;
oldx=-1;
graphpoints=new REALTYPE [w()];
}

void FormantFilterGraph::draw_freq_line(REALTYPE freq,int type) {
  REALTYPE freqx=pars->getfreqpos(freq);
switch(type){
  case 0:fl_line_style(FL_SOLID);break;
  case 1:fl_line_style(FL_DOT);break;
  case 2:fl_line_style(FL_DASH);break;
}; 


if ((freqx>0.0)&&(freqx<1.0))
   fl_line(x()+(int) (freqx*w()),y(),
   x()+(int) (freqx*w()),y()+h());
}

void FormantFilterGraph::draw() {
  int maxdB=30;
int ox=x(),oy=y(),lx=w(),ly=h(),i,oiy;
REALTYPE freqx;

fl_color(FL_BLACK);
fl_rectf(ox,oy,lx,ly);


//draw the lines
fl_color(FL_GRAY);

fl_line_style(FL_SOLID);
//fl_line(ox+2,oy+ly/2,ox+lx-2,oy+ly/2);

freqx=pars->getfreqpos(1000.0);
if ((freqx>0.0)&&(freqx<1.0))
   fl_line(ox+(int) (freqx*lx),oy,
    ox+(int) (freqx*lx),oy+ly);

for (i=1;i<10;i++){
   if(i==1){
     draw_freq_line(i*100.0,0);
     draw_freq_line(i*1000.0,0);
   }else 
    if (i==5){
      draw_freq_line(i*100.0,2);
      draw_freq_line(i*1000.0,2);
    }else{
      draw_freq_line(i*100.0,1);
      draw_freq_line(i*1000.0,1);
    };
};

draw_freq_line(10000.0,0);
draw_freq_line(20000.0,1);

fl_line_style(FL_DOT);
int GY=10;if (ly<GY*3) GY=-1;
for (i=1;i<GY;i++){
   int tmp=(int)(ly/(REALTYPE)GY*i);
   fl_line(ox+2,oy+tmp,ox+lx-2,oy+tmp);
};

fl_color(FL_YELLOW);
fl_font(FL_HELVETICA,10);
if (*nformant<pars->Pnumformants){
   draw_freq_line(pars->getformantfreq(pars->Pvowels[*nvowel].formants[*nformant].freq),2);

//show some information (like current formant frequency,amplitude)
   char tmpstr[20];

   snprintf(tmpstr,20,"%.2f kHz",pars->getformantfreq(pars->Pvowels[*nvowel].formants[*nformant].freq)*0.001);
   fl_draw(tmpstr,ox+1,oy+1,40,12,FL_ALIGN_LEFT,NULL,0);

   snprintf(tmpstr,20,"%d dB",(int)( rap2dB(1e-9 + pars->getformantamp(pars->Pvowels[*nvowel].formants[*nformant].amp)) + pars->getgain() ));
   fl_draw(tmpstr,ox+1,oy+15,40,12,FL_ALIGN_LEFT,NULL,0);

};

//draw the data

fl_color(FL_RED);
fl_line_style(FL_SOLID);

pars->formantfilterH(*nvowel,lx,graphpoints);

oiy=(int) ((graphpoints[0]/maxdB+1.0)*ly/2.0);
for (i=1;i<lx;i++){
   int iy=(int) ((graphpoints[i]/maxdB+1.0)*ly/2.0);
   if ((iy>=0)&&(oiy>=0)&&(iy<ly)&&(oiy<lx))
      fl_line(ox+i-1,oy+ly-oiy,ox+i,oy+ly-iy);
   oiy=iy;
};
}

FormantFilterGraph::~FormantFilterGraph() {
  delete [] graphpoints;
}

void FilterUI::cb_analogfiltertypechoice_i(Fl_Choice* o, void*) {
  pars->Ptype=(int)o->value();
pars->changed=true;
}
void FilterUI::cb_analogfiltertypechoice(Fl_Choice* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_analogfiltertypechoice_i(o,v);
}

Fl_Menu_Item FilterUI::menu_analogfiltertypechoice[] = {
 {"LPF1", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"HPF1", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"LPF2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"HPF2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"BPF2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"NF2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"PkF2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"LSh2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"HSh2", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void FilterUI::cb_svfiltertypechoice_i(Fl_Choice* o, void*) {
  pars->Ptype=(int)o->value();
pars->changed=true;
}
void FilterUI::cb_svfiltertypechoice(Fl_Choice* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_svfiltertypechoice_i(o,v);
}

Fl_Menu_Item FilterUI::menu_svfiltertypechoice[] = {
 {"1LPF", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"1HPF", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"1BPF", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"1NF", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void FilterUI::cb_filtertype_i(Fl_Choice* o, void*) {
  switchcategory((int)o->value());
pars->changed=true;
}
void FilterUI::cb_filtertype(Fl_Choice* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_filtertype_i(o,v);
}

Fl_Menu_Item FilterUI::menu_filtertype[] = {
 {"Analog", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"Formant", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"StVarF", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void FilterUI::cb_cfreqdial_i(WidgetPDial* o, void*) {
  pars->Pfreq=(int)o->value();
}
void FilterUI::cb_cfreqdial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_cfreqdial_i(o,v);
}

void FilterUI::cb_qdial_i(WidgetPDial* o, void*) {
  pars->Pq=(int)o->value();
formantfiltergraph->redraw();
}
void FilterUI::cb_qdial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_qdial_i(o,v);
}

void FilterUI::cb_freqtrdial_i(WidgetPDial* o, void*) {
  pars->Pfreqtrack=(int) o->value();
}
void FilterUI::cb_freqtrdial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_freqtrdial_i(o,v);
}

void FilterUI::cb_vsnsadial_i(WidgetPDial* o, void*) {
  if (velsnsamp!=NULL) *velsnsamp=(int)o->value();
}
void FilterUI::cb_vsnsadial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_vsnsadial_i(o,v);
}

void FilterUI::cb_vsnsdial_i(WidgetPDial* o, void*) {
  if (velsns!=NULL) *velsns=(int)o->value();
}
void FilterUI::cb_vsnsdial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_vsnsdial_i(o,v);
}

void FilterUI::cb_gaindial_i(WidgetPDial* o, void*) {
  pars->Pgain=(int)o->value();
formantfiltergraph->redraw();
pars->changed=true;
}
void FilterUI::cb_gaindial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_gaindial_i(o,v);
}

void FilterUI::cb_stcounter_i(Fl_Choice* o, void*) {
  pars->Pstages=(int)o->value();
formantfiltergraph->redraw();
pars->changed=true;
}
void FilterUI::cb_stcounter(Fl_Choice* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_stcounter_i(o,v);
}

void FilterUI::cb_editbutton_i(Fl_Button*, void*) {
  formantparswindow->show();
}
void FilterUI::cb_editbutton(Fl_Button* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_editbutton_i(o,v);
}

void FilterUI::cb_C_i(Fl_Button*, void*) {
  presetsui->copy(pars);
}
void FilterUI::cb_C(Fl_Button* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_C_i(o,v);
}

void FilterUI::cb_P_i(Fl_Button*, void*) {
  presetsui->paste(pars,this);
}
void FilterUI::cb_P(Fl_Button* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_P_i(o,v);
}

void FilterUI::cb_Formant_i(Fl_Counter* o, void*) {
  nformant=(int) o->value();
update_formant_window();
formantfiltergraph->redraw();
}
void FilterUI::cb_Formant(Fl_Counter* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_Formant_i(o,v);
}

void FilterUI::cb_Vowel_i(Fl_Counter* o, void*) {
  nvowel=(int) o->value();
update_formant_window();
formantfiltergraph->redraw();
}
void FilterUI::cb_Vowel(Fl_Counter* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_Vowel_i(o,v);
}

void FilterUI::cb_formant_freq_dial_i(WidgetPDial* o, void*) {
  pars->Pvowels[nvowel].formants[nformant].freq=(int) o->value();
formantfiltergraph->redraw();
pars->changed=true;
}
void FilterUI::cb_formant_freq_dial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->parent()->user_data()))->cb_formant_freq_dial_i(o,v);
}

void FilterUI::cb_formant_q_dial_i(WidgetPDial* o, void*) {
  pars->Pvowels[nvowel].formants[nformant].q=(int) o->value();
formantfiltergraph->redraw();
pars->changed=true;
}
void FilterUI::cb_formant_q_dial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->parent()->user_data()))->cb_formant_q_dial_i(o,v);
}

void FilterUI::cb_formant_amp_dial_i(WidgetPDial* o, void*) {
  pars->Pvowels[nvowel].formants[nformant].amp=(int) o->value();
formantfiltergraph->redraw();
pars->changed=true;
}
void FilterUI::cb_formant_amp_dial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->parent()->user_data()))->cb_formant_amp_dial_i(o,v);
}

void FilterUI::cb_Seq_i(Fl_Counter* o, void*) {
  pars->Psequencesize=(int) o->value();
update_formant_window();
pars->changed=true;
}
void FilterUI::cb_Seq(Fl_Counter* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_Seq_i(o,v);
}

void FilterUI::cb_S_i(Fl_Counter* o, void*) {
  nseqpos=(int) o->value();
update_formant_window();
pars->changed=true;
}
void FilterUI::cb_S(Fl_Counter* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_S_i(o,v);
}

void FilterUI::cb_vowel_counter_i(Fl_Counter* o, void*) {
  pars->Psequence[nseqpos].nvowel=(int) o->value();
pars->changed=true;
}
void FilterUI::cb_vowel_counter(Fl_Counter* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_vowel_counter_i(o,v);
}

void FilterUI::cb_Neg_i(Fl_Check_Button* o, void*) {
  pars->Psequencereversed=(int) o->value();
pars->changed=true;
}
void FilterUI::cb_Neg(Fl_Check_Button* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_Neg_i(o,v);
}

void FilterUI::cb_strchdial_i(WidgetPDial* o, void*) {
  pars->Psequencestretch=(int) o->value();
pars->changed=true;
}
void FilterUI::cb_strchdial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->parent()->user_data()))->cb_strchdial_i(o,v);
}

void FilterUI::cb_Num_i(Fl_Counter* o, void*) {
  pars->Pnumformants=(int) o->value();
update_formant_window();
pars->changed=true;
formantfiltergraph->redraw();
}
void FilterUI::cb_Num(Fl_Counter* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_Num_i(o,v);
}

void FilterUI::cb_frsldial_i(WidgetPDial* o, void*) {
  pars->Pformantslowness=(int) o->value();
pars->changed=true;
}
void FilterUI::cb_frsldial(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_frsldial_i(o,v);
}

void FilterUI::cb_centerfreqvo_i(Fl_Value_Output* o, void*) {
  o->value(pars->getcenterfreq()/1000.0);
}
void FilterUI::cb_centerfreqvo(Fl_Value_Output* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_centerfreqvo_i(o,v);
}

void FilterUI::cb_octavesfreqvo_i(Fl_Value_Output* o, void*) {
  o->value(pars->getoctavesfreq());
}
void FilterUI::cb_octavesfreqvo(Fl_Value_Output* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_octavesfreqvo_i(o,v);
}

void FilterUI::cb_cfknob_i(Fl_Slider* o, void*) {
  pars->Pcenterfreq=(int)o->value();
centerfreqvo->do_callback();
formantfiltergraph->redraw();
pars->changed=true;
}
void FilterUI::cb_cfknob(Fl_Slider* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_cfknob_i(o,v);
}

void FilterUI::cb_octknob_i(Fl_Slider* o, void*) {
  pars->Poctavesfreq=(int)o->value();
octavesfreqvo->do_callback();
formantfiltergraph->redraw();
}
void FilterUI::cb_octknob(Fl_Slider* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_octknob_i(o,v);
}

void FilterUI::cb_wvknob_i(WidgetPDial* o, void*) {
  pars->Pvowelclearness=(int) o->value();
pars->changed=true;
}
void FilterUI::cb_wvknob(WidgetPDial* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_wvknob_i(o,v);
}

void FilterUI::cb_Close_i(Fl_Button*, void*) {
  formantparswindow->hide();
}
void FilterUI::cb_Close(Fl_Button* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_Close_i(o,v);
}

void FilterUI::cb_C1_i(Fl_Button*, void*) {
  presetsui->copy(pars,nvowel);
}
void FilterUI::cb_C1(Fl_Button* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_C1_i(o,v);
}

void FilterUI::cb_P1_i(Fl_Button*, void*) {
  presetsui->paste(pars,this,nvowel);
}
void FilterUI::cb_P1(Fl_Button* o, void* v) {
  ((FilterUI*)(o->parent()->user_data()))->cb_P1_i(o,v);
}

FilterUI::FilterUI(int x,int y, int w, int h, const char *label):Fl_Group(x,y,w,h,label) {
  pars=NULL;
velsnsamp=NULL;
velsns=NULL;
nvowel=0;nformant=0;nseqpos=0;
}

FilterUI::~FilterUI() {
  filterui->hide();
formantparswindow->hide();
hide();
//delete (filterui);
delete (formantparswindow);
}

Fl_Group* FilterUI::make_window() {
  { filterui = new Fl_Group(0, 0, 275, 75);
    filterui->box(FL_FLAT_BOX);
    filterui->color(FL_LIGHT1);
    filterui->selection_color(FL_BACKGROUND_COLOR);
    filterui->labeltype(FL_NO_LABEL);
    filterui->labelfont(1);
    filterui->labelsize(14);
    filterui->labelcolor(FL_FOREGROUND_COLOR);
    filterui->user_data((void*)(this));
    filterui->align(FL_ALIGN_TOP);
    filterui->when(FL_WHEN_RELEASE);
    { filterparamswindow = new Fl_Group(0, 0, 275, 75, "Filter Parameters");
      filterparamswindow->box(FL_PLASTIC_UP_BOX);
      filterparamswindow->color((Fl_Color)183);
      filterparamswindow->labeltype(FL_ENGRAVED_LABEL);
      filterparamswindow->labelsize(10);
      filterparamswindow->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);
      { Fl_Choice* o = analogfiltertypechoice = new Fl_Choice(10, 50, 50, 15, "FilterType");
        analogfiltertypechoice->tooltip("The Filter type");
        analogfiltertypechoice->down_box(FL_BORDER_BOX);
        analogfiltertypechoice->labelsize(10);
        analogfiltertypechoice->textsize(10);
        analogfiltertypechoice->callback((Fl_Callback*)cb_analogfiltertypechoice);
        analogfiltertypechoice->align(FL_ALIGN_TOP_LEFT);
        analogfiltertypechoice->menu(menu_analogfiltertypechoice);
        o->value(pars->Ptype);
      } // Fl_Choice* analogfiltertypechoice
      { Fl_Choice* o = svfiltertypechoice = new Fl_Choice(10, 50, 50, 15, "FilterType");
        svfiltertypechoice->tooltip("The Filter type");
        svfiltertypechoice->down_box(FL_BORDER_BOX);
        svfiltertypechoice->labelsize(10);
        svfiltertypechoice->textsize(10);
        svfiltertypechoice->callback((Fl_Callback*)cb_svfiltertypechoice);
        svfiltertypechoice->align(FL_ALIGN_TOP_LEFT);
        svfiltertypechoice->menu(menu_svfiltertypechoice);
        o->value(pars->Ptype);
      } // Fl_Choice* svfiltertypechoice
      { Fl_Choice* o = filtertype = new Fl_Choice(10, 20, 60, 15, "Category");
        filtertype->tooltip("The Category of the Filter (Analog/Formantic/etc.)");
        filtertype->down_box(FL_BORDER_BOX);
        filtertype->labelsize(10);
        filtertype->textsize(10);
        filtertype->callback((Fl_Callback*)cb_filtertype);
        filtertype->align(FL_ALIGN_TOP_LEFT);
        filtertype->menu(menu_filtertype);
        o->value(pars->Pcategory);
      } // Fl_Choice* filtertype
      { WidgetPDial* o = cfreqdial = new WidgetPDial(75, 25, 30, 30, "C.Freq");
        cfreqdial->tooltip("Center Frequency of the Filter or the base position in the vowel\'s sequence");
        cfreqdial->box(FL_ROUND_UP_BOX);
        cfreqdial->color(FL_BACKGROUND_COLOR);
        cfreqdial->selection_color(FL_INACTIVE_COLOR);
        cfreqdial->labeltype(FL_NORMAL_LABEL);
        cfreqdial->labelfont(0);
        cfreqdial->labelsize(10);
        cfreqdial->labelcolor(FL_FOREGROUND_COLOR);
        cfreqdial->maximum(127);
        cfreqdial->step(1);
        cfreqdial->callback((Fl_Callback*)cb_cfreqdial);
        cfreqdial->align(FL_ALIGN_BOTTOM);
        cfreqdial->when(FL_WHEN_CHANGED);
        o->value(pars->Pfreq);
      } // WidgetPDial* cfreqdial
      { WidgetPDial* o = qdial = new WidgetPDial(110, 25, 30, 30, "Q");
        qdial->tooltip("Filter resonance or bandwidth");
        qdial->box(FL_ROUND_UP_BOX);
        qdial->color(FL_BACKGROUND_COLOR);
        qdial->selection_color(FL_INACTIVE_COLOR);
        qdial->labeltype(FL_NORMAL_LABEL);
        qdial->labelfont(0);
        qdial->labelsize(10);
        qdial->labelcolor(FL_FOREGROUND_COLOR);
        qdial->maximum(127);
        qdial->step(1);
        qdial->callback((Fl_Callback*)cb_qdial);
        qdial->align(FL_ALIGN_BOTTOM);
        qdial->when(FL_WHEN_CHANGED);
        o->value(pars->Pq);
      } // WidgetPDial* qdial
      { WidgetPDial* o = freqtrdial = new WidgetPDial(215, 25, 30, 30, "freq.tr.");
        freqtrdial->tooltip("Filter frequency tracking (left is negative, middle is 0, and right is positi\
ve)");
        freqtrdial->box(FL_ROUND_UP_BOX);
        freqtrdial->color(FL_BACKGROUND_COLOR);
        freqtrdial->selection_color(FL_INACTIVE_COLOR);
        freqtrdial->labeltype(FL_NORMAL_LABEL);
        freqtrdial->labelfont(0);
        freqtrdial->labelsize(10);
        freqtrdial->labelcolor(FL_FOREGROUND_COLOR);
        freqtrdial->maximum(127);
        freqtrdial->step(1);
        freqtrdial->callback((Fl_Callback*)cb_freqtrdial);
        freqtrdial->align(FL_ALIGN_BOTTOM);
        freqtrdial->when(FL_WHEN_CHANGED);
        o->value(pars->Pfreqtrack);
      } // WidgetPDial* freqtrdial
      { vsnsadial = new WidgetPDial(145, 25, 30, 30, "V.SnsA.");
        vsnsadial->tooltip("Velocity sensing amount of the Filter");
        vsnsadial->box(FL_ROUND_UP_BOX);
        vsnsadial->color(FL_BACKGROUND_COLOR);
        vsnsadial->selection_color(FL_INACTIVE_COLOR);
        vsnsadial->labeltype(FL_NORMAL_LABEL);
        vsnsadial->labelfont(0);
        vsnsadial->labelsize(10);
        vsnsadial->labelcolor(FL_FOREGROUND_COLOR);
        vsnsadial->maximum(127);
        vsnsadial->step(1);
        vsnsadial->callback((Fl_Callback*)cb_vsnsadial);
        vsnsadial->align(FL_ALIGN_BOTTOM);
        vsnsadial->when(FL_WHEN_CHANGED);
      } // WidgetPDial* vsnsadial
      { vsnsdial = new WidgetPDial(180, 25, 30, 30, "V.Sns.");
        vsnsdial->tooltip("Velocity Sensing Function of the Filter");
        vsnsdial->box(FL_ROUND_UP_BOX);
        vsnsdial->color(FL_BACKGROUND_COLOR);
        vsnsdial->selection_color(FL_INACTIVE_COLOR);
        vsnsdial->labeltype(FL_NORMAL_LABEL);
        vsnsdial->labelfont(0);
        vsnsdial->labelsize(10);
        vsnsdial->labelcolor(FL_FOREGROUND_COLOR);
        vsnsdial->maximum(127);
        vsnsdial->step(1);
        vsnsdial->callback((Fl_Callback*)cb_vsnsdial);
        vsnsdial->align(FL_ALIGN_BOTTOM);
        vsnsdial->when(FL_WHEN_CHANGED);
      } // WidgetPDial* vsnsdial
      { WidgetPDial* o = gaindial = new WidgetPDial(250, 35, 20, 20, "gain");
        gaindial->tooltip("Filter output gain/damp");
        gaindial->box(FL_ROUND_UP_BOX);
        gaindial->color(FL_BACKGROUND_COLOR);
        gaindial->selection_color(FL_INACTIVE_COLOR);
        gaindial->labeltype(FL_NORMAL_LABEL);
        gaindial->labelfont(0);
        gaindial->labelsize(10);
        gaindial->labelcolor(FL_FOREGROUND_COLOR);
        gaindial->maximum(127);
        gaindial->step(1);
        gaindial->callback((Fl_Callback*)cb_gaindial);
        gaindial->align(FL_ALIGN_BOTTOM);
        gaindial->when(FL_WHEN_CHANGED);
        o->value(pars->Pgain);
      } // WidgetPDial* gaindial
      { Fl_Choice* o = stcounter = new Fl_Choice(235, 5, 35, 15, "St");
        stcounter->tooltip("Filter stages (in order to increase dB/oct. value and the order of the filter\
)");
        stcounter->down_box(FL_BORDER_BOX);
        stcounter->labelsize(10);
        stcounter->textfont(1);
        stcounter->textsize(10);
        stcounter->callback((Fl_Callback*)cb_stcounter);
        for (int i=0;i<MAX_FILTER_STAGES;i++) {char tmp[10];snprintf(tmp,10,"%dx",i+1);o->add(tmp);};
        o->value(pars->Pstages);
      } // Fl_Choice* stcounter
      filterparamswindow->end();
    } // Fl_Group* filterparamswindow
    { editbutton = new Fl_Button(15, 40, 50, 25, "Edit");
      editbutton->box(FL_PLASTIC_UP_BOX);
      editbutton->labelfont(1);
      editbutton->labelsize(11);
      editbutton->callback((Fl_Callback*)cb_editbutton);
    } // Fl_Button* editbutton
    { Fl_Button* o = new Fl_Button(186, 5, 15, 15, "C");
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)179);
      o->labelfont(1);
      o->labelsize(10);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->callback((Fl_Callback*)cb_C);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(203, 5, 15, 15, "P");
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)179);
      o->labelfont(1);
      o->labelsize(10);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->callback((Fl_Callback*)cb_P);
    } // Fl_Button* o
    filterui->end();
  } // Fl_Group* filterui
  return filterui;
}

Fl_Double_Window* FilterUI::make_formant_window() {
  { formantparswindow = new Fl_Double_Window(700, 205, "Formant Filter Parameters");
    formantparswindow->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(485, 47, 105, 113);
      o->box(FL_THIN_UP_BOX);
      { Fl_Counter* o = new Fl_Counter(545, 80, 40, 15, "Formant ");
        o->type(1);
        o->labelfont(1);
        o->labelsize(10);
        o->minimum(0);
        o->maximum(127);
        o->step(1);
        o->textsize(10);
        o->callback((Fl_Callback*)cb_Formant);
        o->align(FL_ALIGN_LEFT);
        o->bounds(0,FF_MAX_FORMANTS-1);
        o->value(nformant);
      } // Fl_Counter* o
      { Fl_Counter* o = new Fl_Counter(545, 55, 40, 20, "Vowel no.");
        o->type(1);
        o->labelfont(1);
        o->labelsize(10);
        o->minimum(0);
        o->maximum(127);
        o->step(1);
        o->textfont(1);
        o->textsize(11);
        o->callback((Fl_Callback*)cb_Vowel);
        o->align(FL_ALIGN_LEFT);
        o->bounds(0,FF_MAX_VOWELS-1);
        o->value(nvowel);
      } // Fl_Counter* o
      { formantparsgroup = new Fl_Group(490, 105, 95, 50);
        formantparsgroup->box(FL_ENGRAVED_FRAME);
        { formant_freq_dial = new WidgetPDial(495, 115, 25, 25, "freq");
          formant_freq_dial->tooltip("Formant frequency");
          formant_freq_dial->box(FL_ROUND_UP_BOX);
          formant_freq_dial->color(FL_BACKGROUND_COLOR);
          formant_freq_dial->selection_color(FL_INACTIVE_COLOR);
          formant_freq_dial->labeltype(FL_NORMAL_LABEL);
          formant_freq_dial->labelfont(0);
          formant_freq_dial->labelsize(10);
          formant_freq_dial->labelcolor(FL_FOREGROUND_COLOR);
          formant_freq_dial->maximum(127);
          formant_freq_dial->step(1);
          formant_freq_dial->callback((Fl_Callback*)cb_formant_freq_dial);
          formant_freq_dial->align(FL_ALIGN_BOTTOM);
          formant_freq_dial->when(FL_WHEN_CHANGED);
        } // WidgetPDial* formant_freq_dial
        { formant_q_dial = new WidgetPDial(525, 115, 24, 25, "Q");
          formant_q_dial->tooltip("Formant\'s Q");
          formant_q_dial->box(FL_ROUND_UP_BOX);
          formant_q_dial->color(FL_BACKGROUND_COLOR);
          formant_q_dial->selection_color(FL_INACTIVE_COLOR);
          formant_q_dial->labeltype(FL_NORMAL_LABEL);
          formant_q_dial->labelfont(0);
          formant_q_dial->labelsize(10);
          formant_q_dial->labelcolor(FL_FOREGROUND_COLOR);
          formant_q_dial->maximum(127);
          formant_q_dial->step(1);
          formant_q_dial->callback((Fl_Callback*)cb_formant_q_dial);
          formant_q_dial->align(FL_ALIGN_BOTTOM);
          formant_q_dial->when(FL_WHEN_CHANGED);
        } // WidgetPDial* formant_q_dial
        { formant_amp_dial = new WidgetPDial(555, 115, 24, 25, "amp");
          formant_amp_dial->tooltip("Formant amplitude");
          formant_amp_dial->box(FL_ROUND_UP_BOX);
          formant_amp_dial->color(FL_BACKGROUND_COLOR);
          formant_amp_dial->selection_color(FL_INACTIVE_COLOR);
          formant_amp_dial->labeltype(FL_NORMAL_LABEL);
          formant_amp_dial->labelfont(0);
          formant_amp_dial->labelsize(10);
          formant_amp_dial->labelcolor(FL_FOREGROUND_COLOR);
          formant_amp_dial->maximum(127);
          formant_amp_dial->step(1);
          formant_amp_dial->callback((Fl_Callback*)cb_formant_amp_dial);
          formant_amp_dial->align(FL_ALIGN_BOTTOM);
          formant_amp_dial->when(FL_WHEN_CHANGED);
        } // WidgetPDial* formant_amp_dial
        formantparsgroup->end();
      } // Fl_Group* formantparsgroup
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(590, 47, 100, 113);
      o->box(FL_THIN_UP_BOX);
      { Fl_Counter* o = new Fl_Counter(595, 62, 55, 20, "Seq.Size");
        o->type(1);
        o->labelfont(1);
        o->labelsize(10);
        o->minimum(0);
        o->maximum(127);
        o->step(1);
        o->textfont(1);
        o->textsize(11);
        o->callback((Fl_Callback*)cb_Seq);
        o->align(FL_ALIGN_TOP_LEFT);
        o->bounds(1,FF_MAX_SEQUENCE-1);
        o->value(pars->Psequencesize);
      } // Fl_Counter* o
      { Fl_Counter* o = new Fl_Counter(595, 97, 40, 15, "S.Pos.");
        o->tooltip("Current position from the sequence");
        o->type(1);
        o->labelfont(1);
        o->labelsize(10);
        o->minimum(0);
        o->maximum(127);
        o->step(1);
        o->textsize(10);
        o->callback((Fl_Callback*)cb_S);
        o->align(FL_ALIGN_TOP_RIGHT);
        o->bounds(0,FF_MAX_SEQUENCE-2);
        o->value(nseqpos);
      } // Fl_Counter* o
      { Fl_Counter* o = vowel_counter = new Fl_Counter(640, 97, 40, 15, "Vowel");
        vowel_counter->type(1);
        vowel_counter->labelsize(10);
        vowel_counter->minimum(0);
        vowel_counter->maximum(127);
        vowel_counter->step(1);
        vowel_counter->textsize(10);
        vowel_counter->callback((Fl_Callback*)cb_vowel_counter);
        vowel_counter->align(FL_ALIGN_TOP);
        o->bounds(0,FF_MAX_VOWELS-1);
      } // Fl_Counter* vowel_counter
      { Fl_Check_Button* o = new Fl_Check_Button(625, 132, 60, 20, "Neg.Input");
        o->tooltip("Negate the input from LFO/envelopes/etc.");
        o->down_box(FL_DOWN_BOX);
        o->labelsize(10);
        o->callback((Fl_Callback*)cb_Neg);
        o->value(pars->Psequencereversed);
      } // Fl_Check_Button* o
      { WidgetPDial* o = strchdial = new WidgetPDial(595, 130, 25, 25, "Strch");
        strchdial->tooltip("Sequence Stretch");
        strchdial->box(FL_ROUND_UP_BOX);
        strchdial->color(FL_BACKGROUND_COLOR);
        strchdial->selection_color(FL_INACTIVE_COLOR);
        strchdial->labeltype(FL_NORMAL_LABEL);
        strchdial->labelfont(0);
        strchdial->labelsize(10);
        strchdial->labelcolor(FL_FOREGROUND_COLOR);
        strchdial->maximum(127);
        strchdial->step(1);
        strchdial->callback((Fl_Callback*)cb_strchdial);
        strchdial->align(FL_ALIGN_TOP);
        strchdial->when(FL_WHEN_CHANGED);
        o->value(pars->Psequencestretch);
      } // WidgetPDial* strchdial
      o->end();
    } // Fl_Group* o
    { Fl_Counter* o = new Fl_Counter(485, 15, 65, 20, "Num.Formants");
      o->type(1);
      o->labelfont(1);
      o->labelsize(10);
      o->minimum(0);
      o->maximum(127);
      o->step(1);
      o->callback((Fl_Callback*)cb_Num);
      o->align(FL_ALIGN_TOP_LEFT);
      o->bounds(1,FF_MAX_FORMANTS);
      o->value(pars->Pnumformants);
    } // Fl_Counter* o
    { WidgetPDial* o = frsldial = new WidgetPDial(565, 15, 25, 25, "Fr.Sl.");
      frsldial->tooltip("Formant\'s Slowness (Morphing)");
      frsldial->box(FL_ROUND_UP_BOX);
      frsldial->color(FL_BACKGROUND_COLOR);
      frsldial->selection_color(FL_INACTIVE_COLOR);
      frsldial->labeltype(FL_NORMAL_LABEL);
      frsldial->labelfont(1);
      frsldial->labelsize(10);
      frsldial->labelcolor(FL_FOREGROUND_COLOR);
      frsldial->maximum(127);
      frsldial->step(1);
      frsldial->callback((Fl_Callback*)cb_frsldial);
      frsldial->align(FL_ALIGN_TOP);
      frsldial->when(FL_WHEN_CHANGED);
      o->value(pars->Pformantslowness);
    } // WidgetPDial* frsldial
    { Fl_Value_Output* o = centerfreqvo = new Fl_Value_Output(515, 164, 33, 18, "C.f.");
      centerfreqvo->tooltip("Center Frequency (kHz)");
      centerfreqvo->minimum(1);
      centerfreqvo->maximum(10);
      centerfreqvo->step(0.01);
      centerfreqvo->value(1);
      centerfreqvo->textfont(1);
      centerfreqvo->callback((Fl_Callback*)cb_centerfreqvo);
      centerfreqvo->when(3);
      o->value(pars->getcenterfreq()/1000.0);
    } // Fl_Value_Output* centerfreqvo
    { Fl_Value_Output* o = octavesfreqvo = new Fl_Value_Output(515, 182, 33, 18, "Oct.");
      octavesfreqvo->tooltip("No. of octaves");
      octavesfreqvo->minimum(1);
      octavesfreqvo->maximum(127);
      octavesfreqvo->step(1);
      octavesfreqvo->value(5);
      octavesfreqvo->textfont(1);
      octavesfreqvo->callback((Fl_Callback*)cb_octavesfreqvo);
      octavesfreqvo->when(3);
      o->value(pars->getoctavesfreq());
    } // Fl_Value_Output* octavesfreqvo
    { Fl_Slider* o = cfknob = new Fl_Slider(551, 167, 84, 15);
      cfknob->type(5);
      cfknob->box(FL_FLAT_BOX);
      cfknob->maximum(127);
      cfknob->callback((Fl_Callback*)cb_cfknob);
      o->value(pars->Pcenterfreq);
    } // Fl_Slider* cfknob
    { Fl_Slider* o = octknob = new Fl_Slider(551, 185, 84, 15);
      octknob->type(5);
      octknob->box(FL_FLAT_BOX);
      octknob->maximum(127);
      octknob->callback((Fl_Callback*)cb_octknob);
      o->value(pars->Poctavesfreq);
    } // Fl_Slider* octknob
    { FormantFilterGraph* o = formantfiltergraph = new FormantFilterGraph(5, 5, 475, 195);
      formantfiltergraph->box(FL_BORDER_BOX);
      formantfiltergraph->color(FL_BACKGROUND_COLOR);
      formantfiltergraph->selection_color(FL_BACKGROUND_COLOR);
      formantfiltergraph->labeltype(FL_NORMAL_LABEL);
      formantfiltergraph->labelfont(0);
      formantfiltergraph->labelsize(14);
      formantfiltergraph->labelcolor(FL_FOREGROUND_COLOR);
      formantfiltergraph->align(FL_ALIGN_CENTER);
      formantfiltergraph->when(FL_WHEN_RELEASE);
      o->init(pars,&nvowel,&nformant);
    } // FormantFilterGraph* formantfiltergraph
    { WidgetPDial* o = wvknob = new WidgetPDial(600, 15, 25, 25, "Vw.Cl.");
      wvknob->tooltip("Vowel \"clearness\" (how the mixed vowels are avoided)");
      wvknob->box(FL_ROUND_UP_BOX);
      wvknob->color(FL_BACKGROUND_COLOR);
      wvknob->selection_color(FL_INACTIVE_COLOR);
      wvknob->labeltype(FL_NORMAL_LABEL);
      wvknob->labelfont(1);
      wvknob->labelsize(10);
      wvknob->labelcolor(FL_FOREGROUND_COLOR);
      wvknob->maximum(127);
      wvknob->step(1);
      wvknob->callback((Fl_Callback*)cb_wvknob);
      wvknob->align(FL_ALIGN_TOP);
      wvknob->when(FL_WHEN_CHANGED);
      o->value(pars->Pvowelclearness);
    } // WidgetPDial* wvknob
    { Fl_Button* o = new Fl_Button(645, 180, 50, 25, "Close");
      o->box(FL_THIN_UP_BOX);
      o->callback((Fl_Callback*)cb_Close);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(635, 25, 25, 15, "C");
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)179);
      o->labelfont(1);
      o->labelsize(11);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->callback((Fl_Callback*)cb_C1);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(665, 25, 25, 15, "P");
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)179);
      o->labelfont(1);
      o->labelsize(11);
      o->labelcolor(FL_BACKGROUND2_COLOR);
      o->callback((Fl_Callback*)cb_P1);
    } // Fl_Button* o
    { new Fl_Box(635, 10, 55, 15, "Vowel");
    } // Fl_Box* o
    formantparswindow->end();
  } // Fl_Double_Window* formantparswindow
  return formantparswindow;
}

void FilterUI::update_formant_window() {
  formant_freq_dial->value(pars->Pvowels[nvowel].formants[nformant].freq);
formant_q_dial->value(pars->Pvowels[nvowel].formants[nformant].q);
formant_amp_dial->value(pars->Pvowels[nvowel].formants[nformant].amp);
if (nformant<pars->Pnumformants) formantparsgroup->activate();
	else formantparsgroup->deactivate();

if (nseqpos<pars->Psequencesize) vowel_counter->activate();
	else vowel_counter->deactivate();


vowel_counter->value(pars->Psequence[nseqpos].nvowel);
}

void FilterUI::refresh() {
  update_formant_window();
formantfiltergraph->redraw();

if (pars->Pcategory==0) svfiltertypechoice->value(pars->Ptype);
if (pars->Pcategory==2) analogfiltertypechoice->value(pars->Ptype);

filtertype->value(pars->Pcategory);

cfreqdial->value(pars->Pfreq);
qdial->value(pars->Pq);

freqtrdial->value(pars->Pfreqtrack);
gaindial->value(pars->Pgain);

stcounter->value(pars->Pstages);

int categ=pars->Pcategory;
if ((categ==0)||(categ==2)) {
	if (categ==0) {
           analogfiltertypechoice->show();
           svfiltertypechoice->hide();
        } else {
           svfiltertypechoice->show();
           analogfiltertypechoice->hide();
        };
	editbutton->hide();
        formantparswindow->hide();
        cfreqdial->label("C.freq");
} else {
	analogfiltertypechoice->hide();
	svfiltertypechoice->hide();
	editbutton->show();
        cfreqdial->label("BS.pos");
};

filterparamswindow->redraw();
}

void FilterUI::init(FilterParams *filterpars_,unsigned char *velsnsamp_,unsigned char *velsns_) {
  pars=filterpars_;
velsnsamp=velsnsamp_;
velsns=velsns_;

make_window();
end();
make_formant_window();


filterui->resize(this->x(),this->y(),this->w(),this->h());


if (velsnsamp==NULL){
	vsnsadial->deactivate();
	vsnsadial->value(127);
   } else vsnsadial->value(*velsnsamp);

if (velsns==NULL){
	vsnsdial->deactivate();
	vsnsdial->value(127);
   } else vsnsdial->value(*velsns);

switchcategory(pars->Pcategory);


formantparswindow->label(this->label());

update_formant_window();
}

void FilterUI::switchcategory(int newcat) {
  if (pars->Pcategory!=newcat){
    pars->Pgain=64;
    gaindial->value(64);
    analogfiltertypechoice->value(0);
    analogfiltertypechoice->do_callback();
    svfiltertypechoice->value(0);
    svfiltertypechoice->do_callback();
};
pars->Pcategory=newcat;

refresh();
}

void FilterUI::use_for_dynamic_filter() {
  freqtrdial->deactivate();
gaindial->when(0);

cfknob->when(FL_WHEN_RELEASE);
octknob->when(FL_WHEN_RELEASE);

frsldial->when(0);
wvknob->when(0);
formant_freq_dial->when(0);
formant_q_dial->when(0);
formant_amp_dial->when(0);
strchdial->when(0);
}
