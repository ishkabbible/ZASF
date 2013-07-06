// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "PresetsUI.h"

void PresetsUI_::refresh() {
}

PresetsUI_::~PresetsUI_() {
}

void PresetsUI::cb_copybrowse_i(Fl_Browser* o, void*) {
  int val=o->value();
if (val!=0){
   presetname->cut(0,presetname->maximum_size());
   presetname->insert(o->text(val));
};
}
void PresetsUI::cb_copybrowse(Fl_Browser* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_copybrowse_i(o,v);
}

void PresetsUI::cb_copypbutton_i(Fl_Button*, void*) {
  const char *tmp=presetname->value();
if (tmp!=NULL) {
        if (strlen(tmp)>0){
           p->copy(tmp);
	   copywin->hide();
        };
};
}
void PresetsUI::cb_copypbutton(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_copypbutton_i(o,v);
}

void PresetsUI::cb_copybutton_i(Fl_Button*, void*) {
  p->copy(NULL);
copywin->hide();
}
void PresetsUI::cb_copybutton(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_copybutton_i(o,v);
}

void PresetsUI::cb_Cancel_i(Fl_Button*, void*) {
  copywin->hide();
}
void PresetsUI::cb_Cancel(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_Cancel_i(o,v);
}

void PresetsUI::cb_presetname_i(Fl_Input* o, void*) {
  const char *tmp=o->value();
if (tmp==NULL) tmp="";
if (strlen(tmp)>0) {
	copybutton->deactivate();
	copypbutton->activate();
} else {
	copybutton->activate();
	copypbutton->deactivate();
};
}
void PresetsUI::cb_presetname(Fl_Input* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_presetname_i(o,v);
}

void PresetsUI::cb_pastebrowse_i(Fl_Browser* o, void*) {
  if (o->value()==0) {
	pastepbutton->deactivate();
	deletepbutton->deactivate();
}else{
	pastepbutton->activate();
	deletepbutton->activate();
};
}
void PresetsUI::cb_pastebrowse(Fl_Browser* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_pastebrowse_i(o,v);
}

void PresetsUI::cb_pastepbutton_i(Fl_Button*, void*) {
  int n=pastebrowse->value();
if (n!=0) p->paste(n);
pastewin->hide();
pui->refresh();
}
void PresetsUI::cb_pastepbutton(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_pastepbutton_i(o,v);
}

void PresetsUI::cb_pastebutton_i(Fl_Button*, void*) {
  p->paste(0);
pastewin->hide();
pui->refresh();
}
void PresetsUI::cb_pastebutton(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_pastebutton_i(o,v);
}

void PresetsUI::cb_Cancel1_i(Fl_Button*, void*) {
  pastewin->hide();
}
void PresetsUI::cb_Cancel1(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_Cancel1_i(o,v);
}

void PresetsUI::cb_deletepbutton_i(Fl_Button*, void*) {
  int n=pastebrowse->value();
if (n!=0) p->deletepreset(n);
rescan();
}
void PresetsUI::cb_deletepbutton(Fl_Button* o, void* v) {
  ((PresetsUI*)(o->parent()->user_data()))->cb_deletepbutton_i(o,v);
}

PresetsUI::PresetsUI() {
  p=NULL;
make_window();
}

PresetsUI::~PresetsUI() {
  copywin->hide();delete(copywin);
pastewin->hide();delete(pastewin);
}

Fl_Double_Window* PresetsUI::make_window() {
  { copywin = new Fl_Double_Window(265, 430, "Copy to Clipboard/Preset");
    copywin->box(FL_PLASTIC_THIN_UP_BOX);
    copywin->color((Fl_Color)238);
    copywin->user_data((void*)(this));
    { copybrowse = new Fl_Browser(10, 25, 245, 320);
      copybrowse->type(1);
      copybrowse->callback((Fl_Callback*)cb_copybrowse);
    } // Fl_Browser* copybrowse
    { copypbutton = new Fl_Button(145, 355, 110, 20, "Copy to Preset");
      copypbutton->box(FL_THIN_UP_BOX);
      copypbutton->callback((Fl_Callback*)cb_copypbutton);
    } // Fl_Button* copypbutton
    { copybutton = new Fl_Button(25, 385, 90, 35, "Copy to Clipboard");
      copybutton->box(FL_THIN_UP_BOX);
      copybutton->callback((Fl_Callback*)cb_copybutton);
      copybutton->align(192);
    } // Fl_Button* copybutton
    { Fl_Button* o = new Fl_Button(160, 385, 80, 35, "Cancel");
      o->box(FL_THIN_UP_BOX);
      o->callback((Fl_Callback*)cb_Cancel);
      o->align(192);
    } // Fl_Button* o
    { Fl_Box* o = new Fl_Box(10, 5, 40, 15, "Type:");
      o->labelsize(11);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    } // Fl_Box* o
    { copytypetext = new Fl_Box(50, 5, 205, 15);
      copytypetext->box(FL_FLAT_BOX);
      copytypetext->color((Fl_Color)238);
      copytypetext->labelfont(1);
      copytypetext->labelsize(11);
      copytypetext->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    } // Fl_Box* copytypetext
    { presetname = new Fl_Input(10, 355, 130, 20);
      presetname->callback((Fl_Callback*)cb_presetname);
      presetname->when(FL_WHEN_CHANGED);
    } // Fl_Input* presetname
    copywin->set_modal();
    copywin->end();
  } // Fl_Double_Window* copywin
  { pastewin = new Fl_Double_Window(265, 430, "Paste from Clipboard/Preset");
    pastewin->box(FL_PLASTIC_THIN_UP_BOX);
    pastewin->color((Fl_Color)238);
    pastewin->user_data((void*)(this));
    { pastebrowse = new Fl_Browser(10, 25, 245, 320);
      pastebrowse->type(2);
      pastebrowse->callback((Fl_Callback*)cb_pastebrowse);
    } // Fl_Browser* pastebrowse
    { pastepbutton = new Fl_Button(10, 355, 160, 20, "Paste from Preset");
      pastepbutton->box(FL_THIN_UP_BOX);
      pastepbutton->callback((Fl_Callback*)cb_pastepbutton);
    } // Fl_Button* pastepbutton
    { pastebutton = new Fl_Button(25, 385, 90, 35, "Paste from Clipboard");
      pastebutton->box(FL_THIN_UP_BOX);
      pastebutton->callback((Fl_Callback*)cb_pastebutton);
      pastebutton->align(192);
    } // Fl_Button* pastebutton
    { Fl_Button* o = new Fl_Button(160, 385, 80, 35, "Cancel");
      o->box(FL_THIN_UP_BOX);
      o->callback((Fl_Callback*)cb_Cancel1);
      o->align(192);
    } // Fl_Button* o
    { pastetypetext = new Fl_Box(55, 5, 200, 15);
      pastetypetext->box(FL_FLAT_BOX);
      pastetypetext->color((Fl_Color)238);
      pastetypetext->labelfont(1);
      pastetypetext->labelsize(11);
      pastetypetext->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    } // Fl_Box* pastetypetext
    { Fl_Box* o = new Fl_Box(15, 5, 40, 15, "Type:");
      o->labelsize(11);
      o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    } // Fl_Box* o
    { deletepbutton = new Fl_Button(180, 355, 75, 20, "Delete");
      deletepbutton->box(FL_THIN_UP_BOX);
      deletepbutton->callback((Fl_Callback*)cb_deletepbutton);
    } // Fl_Button* deletepbutton
    pastewin->set_modal();
    pastewin->end();
  } // Fl_Double_Window* pastewin
  return pastewin;
}

void PresetsUI::copy(Presets *p) {
  copybutton->activate();
copypbutton->deactivate();


this->p=p;
this->pui=NULL;
bool but=(Fl::event_button()!=FL_LEFT_MOUSE);
presetname->cut(0,presetname->maximum_size());

if (but) p->copy(NULL);
   else {
        rescan();
	copytypetext->label(&p->type[1]);
	copywin->show();
   };
}

void PresetsUI::paste(Presets *p,PresetsUI_ *pui) {
  this->p=p;
this->pui=pui;
bool but=(Fl::event_button()!=FL_LEFT_MOUSE);
pastepbutton->deactivate();
deletepbutton->deactivate();

if (but) {
	p->paste(0);
	pui->refresh();
} else {
        rescan();
	pastetypetext->label(&p->type[1]);
	if (p->checkclipboardtype()) pastebutton->activate();
		else pastebutton->deactivate();
	pastewin->show();
   };
}

void PresetsUI::copy(Presets *p,int n) {
  p->setelement(n);
copy(p);
}

void PresetsUI::paste(Presets *p,PresetsUI_ *pui,int n) {
  p->setelement(n);
paste(p,pui);
}

void PresetsUI::rescan() {
  copybrowse->clear();
pastebrowse->clear();
p->rescanforpresets();

for (int i=0;i<MAX_PRESETS;i++){
   char *name=presetsstore.presets[i].name;
   if (name==NULL) break;
   copybrowse->add(name);
   pastebrowse->add(name);
};
}
PresetsUI *presetsui; 
