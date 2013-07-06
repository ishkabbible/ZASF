// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "BankUI.h"

//****
#include "../Output/VSTaudiooutput.h"
//**VDX include

//Copyright (c) 2002-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later

void BankProcess_::process() {
}

BankSlot::BankSlot(int x,int y, int w, int h, const char *label):Fl_Button(x,y,w,h,label) {
  what=NULL;
whatslot=NULL;
nslot=0;
nselected=NULL;
}

int BankSlot::handle(int event) {
  if (what==NULL) return(0);
if (Fl::event_inside(this)){
  *what=0;*whatslot=nslot;
  if ((event==FL_RELEASE)&&(Fl::event_button()==1))*what=1;
  if ((event==FL_RELEASE)&&(Fl::event_button()==3))*what=2;
  if (event==FL_PUSH) highlight=1;
}else highlight=0;

int tmp=Fl_Button::handle(event);
if ((*what!=0) && Fl::event_inside(this)) (bp->*fnc)();
return(tmp);
}

void BankSlot::init(int nslot_, int *what_, int *whatslot_,void (BankProcess_:: *fnc_)(void),BankProcess_ *bp_,Bank *bank_,int *nselected_) {
  nslot=nslot_;
what=what_;
whatslot=whatslot_;
fnc=fnc_;
bp=bp_;
bank=bank_;
nselected=nselected_;
box(FL_THIN_UP_BOX);
labelfont(0);
labelsize(13);
align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP);

highlight=0;
refresh();
}

void BankSlot::refresh() {
  if (bank->emptyslot(nslot)) {
	color(46);
} else {
	if (bank->isPADsynth_used(nslot)) color(26);
		else color(51);
};

if (*nselected==nslot) color(6);


label(bank->getnamenumbered(nslot));
}

void BankUI::cb_Close_i(Fl_Button*, void*) {
  bankuiwindow->hide();
}
void BankUI::cb_Close(Fl_Button* o, void* v) {
  ((BankUI*)(o->parent()->user_data()))->cb_Close_i(o,v);
}

void BankUI::cb_writebutton_i(Fl_Light_Button* o, void*) {
  if (o->value()>0.5) mode=2;
removeselection();
}
void BankUI::cb_writebutton(Fl_Light_Button* o, void* v) {
  ((BankUI*)(o->parent()->parent()->user_data()))->cb_writebutton_i(o,v);
}

void BankUI::cb_readbutton_i(Fl_Light_Button* o, void*) {
  if (o->value()>0.5) mode=1;
removeselection();
}
void BankUI::cb_readbutton(Fl_Light_Button* o, void* v) {
  ((BankUI*)(o->parent()->parent()->user_data()))->cb_readbutton_i(o,v);
}

void BankUI::cb_clearbutton_i(Fl_Light_Button* o, void*) {
  if (o->value()>0.5) mode=3;
removeselection();
}
void BankUI::cb_clearbutton(Fl_Light_Button* o, void* v) {
  ((BankUI*)(o->parent()->parent()->user_data()))->cb_clearbutton_i(o,v);
}

void BankUI::cb_swapbutton_i(Fl_Light_Button* o, void*) {
  if (o->value()>0.5) mode=4;
removeselection();
}
void BankUI::cb_swapbutton(Fl_Light_Button* o, void* v) {
  ((BankUI*)(o->parent()->parent()->user_data()))->cb_swapbutton_i(o,v);
}

void BankUI::cb_New_i(Fl_Button*, void*) {
  const char *dirname;

dirname=fl_input("New empty Bank:");
if (dirname==NULL) return;


int result=bank->newbank(dirname);

if (result!=0) fl_alert("Error: Could not make a new bank (directory)..");

refreshmainwindow();
}
void BankUI::cb_New(Fl_Button* o, void* v) {
  ((BankUI*)(o->parent()->user_data()))->cb_New_i(o,v);
}

void BankUI::cb_auto_i(Fl_Check_Button* o, void*) {
  config->cfg.BankUIAutoClose=(int) o->value();
}
void BankUI::cb_auto(Fl_Check_Button* o, void* v) {
  ((BankUI*)(o->parent()->user_data()))->cb_auto_i(o,v);
}

void BankUI::cb_banklist_i(Fl_Choice* o, void*) {
  int n=o->value();
char *dirname=bank->banks[n].dir;
if (dirname==NULL) return;

if (bank->loadbank(dirname)==2)
       fl_alert("Error: Could not load the bank from the directory\n%s.",dirname);
for (int i=0;i<BANK_SIZE;i++) bs[i]->refresh();
refreshmainwindow();
}
void BankUI::cb_banklist(Fl_Choice* o, void* v) {
  ((BankUI*)(o->parent()->user_data()))->cb_banklist_i(o,v);
}

void BankUI::cb_Refresh_i(Fl_Button*, void*) {
  rescan_for_banks();
banklist->value(0);
}
void BankUI::cb_Refresh(Fl_Button* o, void* v) {
  ((BankUI*)(o->parent()->user_data()))->cb_Refresh_i(o,v);
}

void BankUI::cb_Show_i(Fl_Check_Button* o, void*) {
  config->cfg.CheckPADsynth=(int) o->value();
refreshmainwindow();
}
void BankUI::cb_Show(Fl_Check_Button* o, void* v) {
  ((BankUI*)(o->parent()->user_data()))->cb_Show_i(o,v);
}

Fl_Double_Window* BankUI::make_window() {
  { Fl_Double_Window* o = bankuiwindow = new Fl_Double_Window(785, 575, "Bank");
    bankuiwindow->user_data((void*)(this));
    { Fl_Button* o = new Fl_Button(705, 546, 70, 24, "Close");
      o->box(FL_THIN_UP_BOX);
      o->callback((Fl_Callback*)cb_Close);
    } // Fl_Button* o
    { Fl_Group* o = new Fl_Group(5, 34, 772, 491);
      o->box(FL_ENGRAVED_FRAME);
      { Fl_Pack* o = new Fl_Pack(10, 39, 150, 481);
        o->box(FL_BORDER_BOX);
        o->box(FL_NO_BOX);
        for (int i=0;i<32;i++){bs[i]=new BankSlot (0,0,o->w(),15," ");bs[i]->init(i,&what,&slot,&BankProcess_::process,(BankProcess_ *)this,bank,&nselected);};
        o->end();
      } // Fl_Pack* o
      { Fl_Pack* o = new Fl_Pack(163, 39, 150, 481);
        o->box(FL_BORDER_BOX);
        o->box(FL_NO_BOX);
        for (int i=32;i<64;i++){bs[i]=new BankSlot (0,0,o->w(),15," ");bs[i]->init(i,&what,&slot,&BankProcess_::process,(BankProcess_ *)this,bank,&nselected);};
        o->end();
      } // Fl_Pack* o
      { Fl_Pack* o = new Fl_Pack(316, 39, 150, 481);
        o->box(FL_BORDER_BOX);
        o->box(FL_NO_BOX);
        for (int i=64;i<96;i++){bs[i]=new BankSlot (0,0,o->w(),15," ");bs[i]->init(i,&what,&slot,&BankProcess_::process,(BankProcess_ *)this,bank,&nselected);};
        o->end();
      } // Fl_Pack* o
      { Fl_Pack* o = new Fl_Pack(469, 39, 150, 481);
        o->box(FL_BORDER_BOX);
        o->box(FL_NO_BOX);
        for (int i=96;i<128;i++){bs[i]=new BankSlot (0,0,o->w(),15," ");bs[i]->init(i,&what,&slot,&BankProcess_::process,(BankProcess_ *)this,bank,&nselected);};
        o->end();
      } // Fl_Pack* o
      { Fl_Pack* o = new Fl_Pack(622, 39, 150, 481);
        o->box(FL_BORDER_BOX);
        o->box(FL_NO_BOX);
        for (int i=128;i<160;i++){bs[i]=new BankSlot (0,0,o->w(),15," ");bs[i]->init(i,&what,&slot,&BankProcess_::process,(BankProcess_ *)this,bank,&nselected);};
        o->end();
      } // Fl_Pack* o
      o->end();
    } // Fl_Group* o
    { modeselect = new Fl_Group(5, 528, 425, 42);
      modeselect->box(FL_ENGRAVED_BOX);
      { Fl_Light_Button* o = writebutton = new Fl_Light_Button(116, 534, 99, 30, "WRITE");
        writebutton->type(102);
        writebutton->box(FL_PLASTIC_UP_BOX);
        writebutton->down_box(FL_THIN_DOWN_BOX);
        writebutton->selection_color((Fl_Color)1);
        writebutton->labeltype(FL_ENGRAVED_LABEL);
        writebutton->labelfont(1);
        writebutton->labelsize(13);
        writebutton->callback((Fl_Callback*)cb_writebutton);
        if (bank->locked()) o->deactivate();
      } // Fl_Light_Button* writebutton
      { Fl_Light_Button* o = readbutton = new Fl_Light_Button(11, 534, 99, 30, "READ");
        readbutton->type(102);
        readbutton->box(FL_PLASTIC_UP_BOX);
        readbutton->down_box(FL_THIN_DOWN_BOX);
        readbutton->selection_color((Fl_Color)101);
        readbutton->labeltype(FL_ENGRAVED_LABEL);
        readbutton->labelfont(1);
        readbutton->labelsize(13);
        readbutton->callback((Fl_Callback*)cb_readbutton);
        o->value(1);
      } // Fl_Light_Button* readbutton
      { Fl_Light_Button* o = clearbutton = new Fl_Light_Button(221, 534, 99, 30, "CLEAR");
        clearbutton->type(102);
        clearbutton->box(FL_PLASTIC_UP_BOX);
        clearbutton->down_box(FL_THIN_DOWN_BOX);
        clearbutton->selection_color(FL_FOREGROUND_COLOR);
        clearbutton->labeltype(FL_ENGRAVED_LABEL);
        clearbutton->labelfont(1);
        clearbutton->labelsize(13);
        clearbutton->callback((Fl_Callback*)cb_clearbutton);
        if (bank->locked()) o->deactivate();
      } // Fl_Light_Button* clearbutton
      { Fl_Light_Button* o = swapbutton = new Fl_Light_Button(325, 534, 99, 30, "SWAP");
        swapbutton->type(102);
        swapbutton->box(FL_PLASTIC_UP_BOX);
        swapbutton->down_box(FL_THIN_DOWN_BOX);
        swapbutton->selection_color((Fl_Color)227);
        swapbutton->labeltype(FL_ENGRAVED_LABEL);
        swapbutton->labelfont(1);
        swapbutton->labelsize(13);
        swapbutton->callback((Fl_Callback*)cb_swapbutton);
        if (bank->locked()) o->deactivate();
      } // Fl_Light_Button* swapbutton
      modeselect->end();
    } // Fl_Group* modeselect
    { Fl_Button* o = new Fl_Button(685, 5, 93, 25, "New Bank...");
      o->box(FL_PLASTIC_UP_BOX);
      o->labelfont(1);
      o->labelsize(11);
      o->callback((Fl_Callback*)cb_New);
      o->align(FL_ALIGN_WRAP);
    } // Fl_Button* o
    { Fl_Check_Button* o = new Fl_Check_Button(705, 529, 60, 15, "auto close");
      o->tooltip("automatically close the bank window if the instrument is loaded");
      o->down_box(FL_DOWN_BOX);
      o->labelsize(10);
      o->callback((Fl_Callback*)cb_auto);
      o->value(config->cfg.BankUIAutoClose);
    } // Fl_Check_Button* o
    { banklist = new Fl_Choice(5, 8, 220, 20);
      banklist->down_box(FL_BORDER_BOX);
      banklist->labelfont(1);
      banklist->textfont(1);
      banklist->textsize(11);
      banklist->callback((Fl_Callback*)cb_banklist);
      banklist->align(FL_ALIGN_CENTER);
    } // Fl_Choice* banklist
    { Fl_Button* o = new Fl_Button(230, 8, 105, 20, "Refresh bank list");
      o->tooltip("Refresh the bank list (rescan)");
      o->box(FL_THIN_UP_BOX);
      o->color(FL_LIGHT1);
      o->labelsize(11);
      o->callback((Fl_Callback*)cb_Refresh);
    } // Fl_Button* o
    { Fl_Check_Button* o = new Fl_Check_Button(435, 530, 150, 15, "Show PADsynth status");
      o->down_box(FL_DOWN_BOX);
      o->labelsize(11);
      o->callback((Fl_Callback*)cb_Show);
      o->value(config->cfg.CheckPADsynth);
    } // Fl_Check_Button* o
    o->label(bank->bankfiletitle);
    if (bank->bankfiletitle==NULL) o->label ("Choose a bank from the bank list on the left (or go to settings if to configure the bank location) or choose 'New Bank...' to make a new bank.");
    bankuiwindow->end();
  } // Fl_Double_Window* bankuiwindow
  return bankuiwindow;
}

BankUI::BankUI(Master *master_,int *npart_) {
  fnc=&BankProcess_::process;
master=master_;
npart=npart_;
bank=&master_->bank;
what=0;
nselected=-1;
make_window();
mode=1;
}

BankUI::~BankUI() {
  bankuiwindow->hide();
delete(bankuiwindow);
}

void BankUI::show() {
  bankuiwindow->show();
simplesetmode(config->cfg.UserInterfaceMode==2);
}

void BankUI::hide() {
  bankuiwindow->hide();
}

void BankUI::init(Fl_Valuator *cbwig_) {
  cbwig=cbwig_;
rescan_for_banks();
}

void BankUI::process() {
  int slot=this->slot;

if ((what==2)&&(bank->emptyslot(slot)==0)&&(mode!=4)) {//Rename slot
    const char *tmp=fl_input("Slot (instrument) name:",(const char *)bank->getname(slot));
    if (tmp!=NULL) bank->setname(slot,tmp,-1);
    bs[slot]->refresh();
};

if ((what==1)&&(mode==1)&&(!bank->emptyslot(slot))){//Reads from slot

	//VDX says: User clicked on a bank slot!!!
    //calll updateDisplay() in VSTSDK
	//(*master->cb_updateDisplay)();
	//master->theVST->updateDisplay();
	
	
	//*((int*)0) = 98;

	
	//***
	master->shutup = 1;
	Sleep(400);
	
	 
	 
	 
	 //pthread_mutex_lock(&master->mutex);
     bank->loadfromslot(slot,master->part[*npart]);
	
   
	 //Sleep(100);
     master->part[*npart]->applyparameters();
	 //pthread_mutex_unlock(&master->mutex);
	 master->ShutUp();
    snprintf((char *)master->part[*npart]->Pname,PART_MAX_NAME_LEN,"%s",bank->getname(slot));
     cbwig->do_callback();
     
     if (config->cfg.BankUIAutoClose!=0)
         bankuiwindow->hide();

};

if ((what==1)&&(mode==2)){//save(write) to slot
     if (!bank->emptyslot(slot)){
       if (!fl_choice("Overwrite the slot no. %d ?","No","Yes",NULL,slot+1)) goto nooverwriteslot;
      };
       //pthread_mutex_lock(&master->mutex);
        bank->savetoslot(slot,master->part[*npart]);
       //pthread_mutex_unlock(&master->mutex);

       bs[slot]->refresh();
       mode=1;readbutton->value(1);writebutton->value(0);
       nooverwriteslot:;
};



if ((what==1)&&(mode==3)&&(!bank->emptyslot(slot))){//Clears the slot
      if (fl_choice("Clear the slot no. %d ?","No","Yes",NULL,slot+1)){
          bank->clearslot(slot);
          bs[slot]->refresh();
      };
};

if (mode==4){//swap
    bool done=false;
    if ((what==1)&&(nselected>=0)){
         bank->swapslot(nselected,slot);
         int ns=nselected;
         nselected=-1;
         bs[slot]->refresh();
         bs[ns]->refresh();
         done=true;
    };
    if (((nselected<0)||(what==2))&&(!done)){
        int ns=nselected;
        nselected=slot;
        if (ns>0) bs[ns]->refresh();
        bs[slot]->refresh();
    };
};
if (mode!=4) refreshmainwindow();
 master->theVST->updateDisplay();
}

void BankUI::refreshmainwindow() {
  bankuiwindow->label(bank->bankfiletitle);
mode=1;readbutton->value(1);writebutton->value(0);clearbutton->value(0);swapbutton->value(0);
nselected=-1;
if (bank->locked()){
    writebutton->deactivate();
    clearbutton->deactivate();
    swapbutton->deactivate();
} else {
    writebutton->activate();
    clearbutton->activate();
    swapbutton->activate();
};
for (int i=0;i<BANK_SIZE;i++) bs[i]->refresh();
}

void BankUI::removeselection() {
  if (nselected>=0) {
   int ns=nselected;
   nselected=-1;
   bs[ns]->refresh();
};
}

void BankUI::rescan_for_banks() {
  banklist->clear();
banklist->add(" ");
bank->rescanforbanks();

for (int i=1;i<MAX_NUM_BANKS;i++) {
    if (bank->banks[i].name!=NULL) banklist->add(bank->banks[i].name);
};
}

void BankUI::simplesetmode(bool beginnerui) {
  readbutton->value(1);
mode=1;
removeselection();
if (beginnerui) modeselect->hide();
	else modeselect->show();
}
