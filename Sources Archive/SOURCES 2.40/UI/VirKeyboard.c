// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "VirKeyboard.h"
//Copyright (c) 2002-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later
static const int keyspos[12]={0,-1,1,-2,2,3,-4,4,-5,5,-6,6}; 
static const int keysoct1qwerty[]={'q','2','w','3','e','r','5','t','6','y','7','u','i','9','o','0','p','[','=',']','\\',FL_Enter,0}; 
static const int keysoct2qwerty[]={'z','s','x','d','c','v','g','b','h','n','j','m',',','l','.',';','/',0}; 
static const int keysoct1dw[]={'\'','2',',','3','.','p','5','y','6','f','7','g','c','9','r','0','l','/',']','=','\\',FL_Enter,0}; 
static const int keysoct2dw[]={';','o','q','e','j','k','i','x','d','b','h','m','w','n','v','s','z',0}; 
static const int keysoct1qwertz[]={'q','2','w','3','e','r','5','t','6','z','7','u','i','9','o','0','p',252,'\'','+','\\',FL_Enter,0}; 
static const int keysoct2qwertz[]={'y','s','x','d','c','v','g','b','h','n','j','m',',','l','.',246,'-',0}; 

VirKeys::VirKeys(int x,int y, int w, int h, const char *label):Fl_Box(x,y,w,h,label) {
  master=NULL;
}

void VirKeys::init(Master *master_) {
  master=master_;
for (int i=0;i<N_OCT*12+1;i++) pressed[i]=0;
midich=0;
midivel=100;
midioct=2;

keyoct1=3;
keyoct2=2;
rndvelocity=0;
}

void VirKeys::draw() {
  int ox=x(),oy=y(),lx=w(),ly=h()-1,i;

if (damage()!=1){
 fl_color(250,240,230);
 fl_rectf(ox,oy,lx,ly);

 fl_color(FL_BLACK);
 fl_line(ox,oy,ox+lx,oy);
 fl_line(ox,oy+ly,ox+lx,oy+ly);
 for (i=0;i<N_OCT*7+1;i++){
   fl_line(ox+i*SIZE_WHITE,oy,ox+i*SIZE_WHITE,oy+ly);
   int ik=i%7;
   if ((ik==1)||(ik==2)||(ik==4)||(ik==5)||(ik==6)) 
     fl_rectf(ox+i*SIZE_WHITE-SIZE_BLACK/2,oy,
              SIZE_BLACK+1,ly*3/5);
 };
};


for (i=0;i<N_OCT*12;i++){
  // if (pressed[i]==0) continue;

   int noct=i/12;
   int kv=keyspos[i%12];

   if (kv>=0){//white keys
     if (pressed[i]==0) fl_color(250,240,230);
        else fl_color(FL_BLUE);
     fl_rectf(ox+(kv+7*noct)*SIZE_WHITE+3,oy+ly*3/5+2,
       SIZE_WHITE-4,ly*2/5-3);
   } else {//black keys
     kv=keyspos[(i+1)%12];
     if (pressed[i]==0) fl_color(FL_BLACK);
        else fl_color(FL_BLUE);
     fl_rectf(ox+(kv+7*noct)*SIZE_WHITE-SIZE_BLACK/2+2,oy+2,
              SIZE_BLACK-3,ly*3/5-5);
   };
};
}

int VirKeys::handle(int event) {
  int i;
int ly=h();
int x_=Fl::event_x()-x();
int y_=Fl::event_y()-y();
if ( (x_<0)&&(x_>w()) && (y_<0)&&(y_>h())){
  return(0);
};


if ((event==FL_PUSH)||(event==FL_DRAG)||(event==FL_RELEASE)){
   int kpos=-1;
  
   if (y_>ly*3/5){//white keys
       int pos=x_/SIZE_WHITE;
       if (pos<0) return(1);
       for (i=0;i<12;i++) {
          if (pos%7==keyspos[i]) {
             kpos=pos/7*12+i;
             break;
          };
       };
   } else {//black keys
       int pos=(x_+SIZE_WHITE/2)/SIZE_WHITE;
       if (pos<0) return(1);
       for (i=1;i<12;i++) {
          if (pos%7==-keyspos[i]) {
             kpos=pos/7*12+i;
             break;
          };
       };
   };

   if (((event==FL_PUSH)||(event==FL_DRAG))&&
       (Fl::event_shift()==0)) {
        presskey(kpos,1,1);
   };

   if ((event==FL_PUSH)&&(Fl::event_shift()!=0)) {
       if (pressed[kpos]==0) presskey(kpos,0,1);
          else relasekey(kpos,1);
   };
   if ((event==FL_RELEASE)&&(Fl::event_shift()==0))
        relaseallkeys(1);
   take_focus();
};


const int *keysoct1=keysoct1qwerty;
const int *keysoct2=keysoct2qwerty;

if (config.cfg.VirKeybLayout==2) {
	keysoct1=keysoct1dw;
	keysoct2=keysoct2dw;
}else if (config.cfg.VirKeybLayout==3) {
	keysoct1=keysoct1qwertz;
	keysoct2=keysoct2qwertz;
};

if ((event==FL_KEYDOWN)||(event==FL_KEYUP)){
   int key=Fl::event_key();
   int kpos=-1;
   for (i=0;keysoct1[i]!=0;i++) if (key==keysoct1[i]) kpos=i+12*keyoct1;
   for (i=0;keysoct2[i]!=0;i++) if (key==keysoct2[i]) kpos=i+12*keyoct2;




   if (kpos==-1) return(0);
   if ((event==FL_KEYUP) && (Fl::event_key(key)==0) && (Fl::get_key(key)!=0)) return(0);
   if (event==FL_KEYDOWN) presskey(kpos,0,2);
      else relasekey(kpos,2);
};

return(1);
}

void VirKeys::presskey(int nk,int exclusive,int type) {
  //Exclusive means that multiple keys can be pressed at once
//when the user uses the shift key
if (nk>=N_OCT*12) return;
if ((nk<0)&&(exclusive==0)) {
  relaseallkeys(type);
  return;
};
if (nk<0) return;
if (pressed[nk]!=0) return;//the key is already pressed

if (exclusive!=0) relaseallkeys(type);
pressed[nk]=type;

damage(1);
float vel=midivel;
if (rndvelocity!=0){
  vel=midivel*(127.0-rndvelocity)/127.0+RND*rndvelocity;
};

//pthread_mutex_lock(&master->mutex);
 master->NoteOn(midich,nk+midioct*12,(int)vel);
//pthread_mutex_unlock(&master->mutex);
}

void VirKeys::relasekey(int nk,int type) {
  if ((nk<0)||(nk>=N_OCT*12)) return;
if (pressed[nk]==0) return;//the key is not pressed
if ((type!=0)&&(pressed[nk]!=type)) return;

pressed[nk]=0;


damage(1);

//pthread_mutex_lock(&master->mutex);
 master->NoteOff(midich,nk+12*midioct);
//pthread_mutex_unlock(&master->mutex);
}

void VirKeys::relaseallkeys(int type) {
  for (int i=0;i<N_OCT*12;i++) relasekey(i,type);
}

void VirKeyboard::cb_virkeyboardwindow_i(Fl_Double_Window*, void*) {
  relaseallkeys();
virkeyboardwindow->hide();
}
void VirKeyboard::cb_virkeyboardwindow(Fl_Double_Window* o, void* v) {
  ((VirKeyboard*)(o->user_data()))->cb_virkeyboardwindow_i(o,v);
}

void VirKeyboard::cb_qwer_i(Fl_Counter* o, void*) {
  relaseallkeys();
virkeys->keyoct1=(int) o->value();
virkeys->take_focus();
}
void VirKeyboard::cb_qwer(Fl_Counter* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_qwer_i(o,v);
}

void VirKeyboard::cb_zxcv_i(Fl_Counter* o, void*) {
  relaseallkeys();
virkeys->keyoct2=(int) o->value();
virkeys->take_focus();
}
void VirKeyboard::cb_zxcv(Fl_Counter* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_zxcv_i(o,v);
}

void VirKeyboard::cb_Vel_i(Fl_Value_Slider* o, void*) {
  virkeys->midivel=(int) o->value();
virkeys->take_focus();
}
void VirKeyboard::cb_Vel(Fl_Value_Slider* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_Vel_i(o,v);
}

void VirKeyboard::cb_Oct_i(Fl_Counter* o, void*) {
  relaseallkeys();
virkeys->midioct=(int) o->value();
virkeys->take_focus();
}
void VirKeyboard::cb_Oct(Fl_Counter* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_Oct_i(o,v);
}

void VirKeyboard::cb_Close_i(Fl_Button*, void*) {
  relaseallkeys();
virkeyboardwindow->hide();
}
void VirKeyboard::cb_Close(Fl_Button* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_Close_i(o,v);
}

void VirKeyboard::cb_Cval_i(Fl_Value_Slider* o, void*) {
  int ctl=midictl;

//pthread_mutex_lock(&master->mutex);
 master->SetController(virkeys->midich,ctl,(int) o->value());
//pthread_mutex_unlock(&master->mutex);
virkeys->take_focus();
}
void VirKeyboard::cb_Cval(Fl_Value_Slider* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_Cval_i(o,v);
}

void VirKeyboard::cb_Controller_i(Fl_Choice* o, void*) {
  switch((int) o->value()+1){
   case 1: midictl=C_modwheel; break;
   case 2: midictl=C_volume; break;
   case 3: midictl=C_panning; break;
   case 4: midictl=C_expression; break;
   case 5: midictl=C_sustain; break;
   case 6: midictl=C_portamento; break;
   case 7: midictl=C_filterq; break;
   case 8: midictl=C_filtercutoff; break;
   case 9: midictl=C_bandwidth; break;
   case 10: midictl=C_fmamp; break;
   case 11: midictl=C_resonance_center; break;
   case 12: midictl=C_resonance_bandwidth; break;
   default: midictl=C_NULL; break;

};



virkeys->take_focus();
}
void VirKeyboard::cb_Controller(Fl_Choice* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_Controller_i(o,v);
}

Fl_Menu_Item VirKeyboard::menu_Controller[] = {
 {"01: Mod.Wheel", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"07: Volume", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"10: Panning", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"11: Expression", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"64: Sustain", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"65: Portamento", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"71: Filter Q", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"74: Filter Freq.", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"75: Bandwidth", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"76: FM Gain", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"77: Res. c. freq", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {"78: Res. bw.", 0,  0, 0, 0, FL_NORMAL_LABEL, 1, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void VirKeyboard::cb_pitchwheelroller_i(Fl_Roller* o, void*) {
  //pthread_mutex_lock(&master->mutex);
 master->SetController(virkeys->midich,C_pitchwheel,-(int) o->value());
//pthread_mutex_unlock(&master->mutex);
virkeys->take_focus();
}
void VirKeyboard::cb_pitchwheelroller(Fl_Roller* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_pitchwheelroller_i(o,v);
}

void VirKeyboard::cb_R_i(Fl_Button*, void*) {
  pitchwheelroller->value(0);
pitchwheelroller->do_callback();
}
void VirKeyboard::cb_R(Fl_Button* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_R_i(o,v);
}

void VirKeyboard::cb_Vrnd_i(WidgetPDial* o, void*) {
  virkeys->rndvelocity=(int) o->value();
}
void VirKeyboard::cb_Vrnd(WidgetPDial* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_Vrnd_i(o,v);
}

void VirKeyboard::cb_partrcv_i(Fl_Choice* o, void*) {
  relaseallkeys();
virkeys->midich=(int) o->value();
virkeys->take_focus();
}
void VirKeyboard::cb_partrcv(Fl_Choice* o, void* v) {
  ((VirKeyboard*)(o->parent()->user_data()))->cb_partrcv_i(o,v);
}

Fl_Double_Window* VirKeyboard::make_window() {
  { virkeyboardwindow = new Fl_Double_Window(650, 130, "Virtual Keyboard - ZynAddSubFX");
    virkeyboardwindow->callback((Fl_Callback*)cb_virkeyboardwindow, (void*)(this));
    { VirKeys* o = virkeys = new VirKeys(10, 10, 590, 80, "Keyboard");
      virkeys->box(FL_FLAT_BOX);
      virkeys->color((Fl_Color)17);
      virkeys->selection_color(FL_BACKGROUND_COLOR);
      virkeys->labeltype(FL_NORMAL_LABEL);
      virkeys->labelfont(0);
      virkeys->labelsize(14);
      virkeys->labelcolor(FL_FOREGROUND_COLOR);
      virkeys->align(FL_ALIGN_CENTER);
      virkeys->when(FL_WHEN_RELEASE);
      o->init(master);
    } // VirKeys* virkeys
    { Fl_Counter* o = new Fl_Counter(380, 95, 45, 15, "\"qwer..\" Oct");
      o->tooltip("keys \"q2w3er5t6y...\" octave");
      o->type(1);
      o->labelsize(10);
      o->minimum(0);
      o->maximum(5);
      o->step(1);
      o->textfont(1);
      o->textsize(10);
      o->callback((Fl_Callback*)cb_qwer);
      o->align(FL_ALIGN_LEFT);
      o->when(FL_WHEN_RELEASE_ALWAYS);
      o->value(virkeys->keyoct1);
    } // Fl_Counter* o
    { Fl_Counter* o = new Fl_Counter(380, 110, 45, 15, "\"zxcv..\" Oct");
      o->tooltip("keys \"zsxdcvgbh...\" octave");
      o->type(1);
      o->labelsize(10);
      o->minimum(0);
      o->maximum(5);
      o->step(1);
      o->textfont(1);
      o->textsize(10);
      o->callback((Fl_Callback*)cb_zxcv);
      o->align(FL_ALIGN_LEFT);
      o->when(FL_WHEN_RELEASE_ALWAYS);
      o->value(virkeys->keyoct2);
    } // Fl_Counter* o
    { Fl_Value_Slider* o = new Fl_Value_Slider(95, 105, 100, 15, "Vel");
      o->tooltip("Velocity");
      o->type(5);
      o->box(FL_FLAT_BOX);
      o->labelsize(10);
      o->minimum(1);
      o->maximum(127);
      o->step(1);
      o->callback((Fl_Callback*)cb_Vel);
      o->align(FL_ALIGN_TOP_LEFT);
      o->value(virkeys->midivel);
    } // Fl_Value_Slider* o
    { Fl_Counter* o = new Fl_Counter(255, 100, 55, 20, "Oct.");
      o->tooltip("Midi Octave");
      o->type(1);
      o->labelsize(11);
      o->minimum(0);
      o->maximum(5);
      o->step(1);
      o->textfont(1);
      o->textsize(11);
      o->callback((Fl_Callback*)cb_Oct);
      o->align(FL_ALIGN_LEFT);
      o->when(FL_WHEN_RELEASE_ALWAYS);
      o->value(virkeys->midioct);
    } // Fl_Counter* o
    { Fl_Button* o = new Fl_Button(545, 105, 55, 20, "Close");
      o->box(FL_THIN_UP_BOX);
      o->callback((Fl_Callback*)cb_Close);
    } // Fl_Button* o
    { Fl_Value_Slider* o = new Fl_Value_Slider(605, 10, 15, 115, "Cval");
      o->tooltip("Controller value");
      o->type(2);
      o->box(FL_ENGRAVED_BOX);
      o->selection_color((Fl_Color)229);
      o->labelsize(8);
      o->minimum(127);
      o->maximum(0);
      o->step(1);
      o->value(64);
      o->textsize(7);
      o->callback((Fl_Callback*)cb_Cval);
      o->align(FL_ALIGN_TOP_LEFT);
    } // Fl_Value_Slider* o
    { Fl_Choice* o = new Fl_Choice(435, 105, 100, 15, "Controller");
      o->down_box(FL_BORDER_BOX);
      o->labelsize(10);
      o->textfont(1);
      o->textsize(10);
      o->callback((Fl_Callback*)cb_Controller);
      o->align(FL_ALIGN_TOP_LEFT);
      o->when(FL_WHEN_RELEASE_ALWAYS);
      o->menu(menu_Controller);
      midictl=C_filtercutoff;o->value(7);
    } // Fl_Choice* o
    { pitchwheelroller = new Fl_Roller(625, 10, 20, 95, "Pwh");
      pitchwheelroller->tooltip("Pitch Wheel");
      pitchwheelroller->box(FL_PLASTIC_UP_BOX);
      pitchwheelroller->labelsize(8);
      pitchwheelroller->minimum(-8192);
      pitchwheelroller->maximum(8192);
      pitchwheelroller->step(64);
      pitchwheelroller->callback((Fl_Callback*)cb_pitchwheelroller);
      pitchwheelroller->align(FL_ALIGN_TOP);
      pitchwheelroller->when(3);
    } // Fl_Roller* pitchwheelroller
    { Fl_Button* o = new Fl_Button(625, 110, 20, 15, "R");
      o->tooltip("Reset Pitch Bend");
      o->box(FL_THIN_UP_BOX);
      o->labelfont(1);
      o->callback((Fl_Callback*)cb_R);
    } // Fl_Button* o
    { WidgetPDial* o = new WidgetPDial(205, 105, 20, 20, "Vrnd");
      o->tooltip("Velocity Randomness");
      o->box(FL_ROUND_UP_BOX);
      o->color(FL_BACKGROUND_COLOR);
      o->selection_color(FL_INACTIVE_COLOR);
      o->labeltype(FL_NORMAL_LABEL);
      o->labelfont(0);
      o->labelsize(10);
      o->labelcolor(FL_FOREGROUND_COLOR);
      o->maximum(127);
      o->step(1);
      o->callback((Fl_Callback*)cb_Vrnd);
      o->align(129);
      o->when(FL_WHEN_CHANGED);
      o->value(virkeys->rndvelocity);
    } // WidgetPDial* o
    { Fl_Choice* o = partrcv = new Fl_Choice(20, 105, 65, 20, "MIDI Ch.");
      partrcv->tooltip("Send to Midi Channel");
      partrcv->down_box(FL_BORDER_BOX);
      partrcv->labelsize(10);
      partrcv->textfont(1);
      partrcv->textsize(10);
      partrcv->callback((Fl_Callback*)cb_partrcv);
      partrcv->align(FL_ALIGN_TOP_LEFT);
      char nrstr[10]; for(int i=0;i<NUM_MIDI_CHANNELS;i++){sprintf(nrstr,"Chn%d",i+1);if (i!=9) o->add(nrstr); else o->add("Drum10");};
      o->value(virkeys->midich);
    } // Fl_Choice* partrcv
    virkeyboardwindow->end();
  } // Fl_Double_Window* virkeyboardwindow
  return virkeyboardwindow;
}

VirKeyboard::VirKeyboard(Master *master_) {
  master=master_;
midictl=75;
make_window();
}

void VirKeyboard::show() {
  virkeyboardwindow->show();
}

void VirKeyboard::hide() {
  virkeyboardwindow->hide();
}

void VirKeyboard::relaseallkeys() {
  virkeys->relaseallkeys(0);
}
