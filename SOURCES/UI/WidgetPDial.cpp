// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "WidgetPDial.h"
#include <FL/Fl.H>
#include <FL/fl_ask.H>
//Copyright (c) 2003-2005 Nasca Octavian Paul
//License: GNU GPL version 2 or later


#include "../Output/VSTaudiooutput.h"


TipWin::TipWin():Fl_Menu_Window(1,1) {
  strcpy(tip, "X.XX");
set_override();
end();
}

void TipWin::draw() {
  draw_box(FL_BORDER_BOX, 0, 0, w(), h(), Fl_Color(175));
        fl_color(FL_BLACK);
        fl_font(labelfont(), labelsize());
        if(textmode)
             fl_draw(text, 3, 3, w()-6, h()-6, Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_WRAP));
        else
             fl_draw(tip, 3, 3, w()-6, h()-6, Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_WRAP));
}

void TipWin::value(float f) {
  sprintf(tip, "%.2f", f);
textmode=false;
        // Recalc size of window
        fl_font(labelfont(), labelsize());
        int W = w(), H = h();
        fl_measure(tip, W, H, 0);
        W += 8;
        size(W, H);
        redraw();
}

void TipWin::setText(const char * c) {
  strcpy(text,c);
textmode=true;
        // Recalc size of window
        fl_font(labelfont(), labelsize());
        int W = w(), H = h();
        fl_measure(text, W, H, 0);
        W += 8;
        size(W, H);
        redraw();
}

void TipWin::setTextmode() {
  textmode=true;
        // Recalc size of window
        fl_font(labelfont(), labelsize());
        int W = w(), H = h();
        fl_measure(text, W, H, 0);
        W += 8;
        size(W, H);
        redraw();
}

WidgetPDial::WidgetPDial(Master* master_, int x,int y, int w, int h, const char *label):Fl_Dial(x,y,w,h,label) {
  callback(value_cb, (void*)this);
Fl_Group *save = Fl_Group::current();
tipwin = new TipWin();
tipwin->hide();
Fl_Group::current(save);
oldvalue=0.0;
pos=false;
textset=false;
master = master_;
}

WidgetPDial::~WidgetPDial() {
  delete tipwin;
}

int WidgetPDial::handle(int event) {
  double dragsize,v,min=minimum(),max=maximum();
int my;
int set_my_val;
extern int ontop;

set_my_val = 0;
switch (event){
//case  FL_KEYUP:
//	if (Fl::event_key() ==13) set_my_val = 1;
//	else set_my_val = 0;
//	break;

case FL_PUSH:
	 //send mouse_down event to synth
	 //!!!!!!!!!!!!
	 //master->theVST->
     oldvalue=value();
	 sprintf(mytxt, "%.2f",oldvalue);
	 set_my_val = Fl::event_state(FL_CTRL);
	 if (set_my_val) {
		 const char *tmp=fl_input("Knob Value:",mytxt);
		 if (tmp!=NULL) {
			
			v = atof(tmp);
			oldvalue = v;
			if (v<min) v=min;
			else if (v>max) v=max;

     
			value(v);
			//value_damage();
			draw();
			if (this->when()!=0) do_callback();
			
		 }
		 return(1); 
		 break;

	
	 }
	  oldvalue=value();;

case FL_DRAG:
     if(!pos){
         tipwin->position(Fl::event_x_root(), Fl::event_y_root()+20);
         pos=true;
     }
     if (!ontop) {
		tipwin->value(value());
		tipwin->show();
	 }; //ontop
     my=-(Fl::event_y()-y()-h()/2);

     dragsize=200.0;
     if (Fl::event_state(FL_BUTTON1)==0) dragsize*=10;
     v=oldvalue+my/dragsize*(max-min);
     if (v<min) v=min;
        else if (v>max) v=max;

     //printf("%d   %g    %g\n",my,v,oldvalue);     
     value(v);
     value_damage();
     if (this->when()!=0) do_callback();
     return(1); 
     break;
case FL_ENTER:
     if(textset){
         if(!pos){
             tipwin->position(Fl::event_x_root(), Fl::event_y_root()+20);
             pos=true;
         }
         tipwin->setTextmode();
		 if (!ontop) tipwin->show();
         return(1);} 
     break;
case FL_HIDE:
case FL_LEAVE:
     tipwin->hide();
     pos=false;
     break;
case FL_RELEASE:
	//signal mouse up
	if (master->b_writingAutomation!=-1) {		
		master->theVST->endEdit(master->b_writingAutomation);
		master->b_writingAutomation = -1;
	};

     tipwin->hide();
     pos=false;
     if (this->when()==0) do_callback();
	return(1);
     break;
};
return(0);
}

void WidgetPDial::drawgradient(int cx,int cy,int sx,double m1,double m2) {
  for (int i=(int)(m1*sx);i<(int)(m2*sx);i++){
   double tmp=1.0-pow(i*1.0/sx,2.0);
   pdialcolor(140+(int) (tmp*90),140+(int)(tmp*90),140+(int) (tmp*100));
   fl_arc(cx+sx/2-i/2,cy+sx/2-i/2,i,i,0,360);
};
}

void WidgetPDial::draw() {
  int cx=x(),cy=y(),sx=w(),sy=h();


//clears the button face
pdialcolor(224,223,227);
fl_pie(cx-1,cy-1,sx+2,sy+2,0,360);

//Draws the button face (gradinet)
drawgradient(cx,cy,sx,0.9,1.0);

double val=(value()-minimum())/(maximum()-minimum());

//draws the scale
pdialcolor(220,220,250);
double a1=angle1(),a2=angle2();
for (int i=0;i<12;i++){
   double a=-i/12.0*360.0-val*(a2-a1)-a1;
   //fl_pie(cx,cy,sx,sy,a+270-3,a+3+270);
   fl_pie(cx,cy,sx,sy,a+80-3,a+3+80);
};

//drawgradient(cx,cy,sx,0.0,0.75);
drawgradient(cx,cy,sx,0.0,0.75);


//draws the value
double a=-(a2-a1)*val-a1;





//draws the max and min points
pdialcolor(0,100,200);
int xp=(int)(cx+sx/2.0+sx/2.0*sin(angle1()/180.0*3.141592));
int yp=(int)(cy+sy/2.0+sy/2.0*cos(angle1()/180.0*3.141592));
fl_pie(xp-2,yp-2,4,4,0,360);

xp=(int)(cx+sx/2.0+sx/2.0*sin(angle2()/180.0*3.141592));
yp=(int)(cy+sy/2.0+sy/2.0*cos(angle2()/180.0*3.141592));
fl_pie(xp-2,yp-2,4,4,0,360);





fl_push_matrix();

  fl_translate(cx+sx/2,cy+sy/2);
  fl_rotate(a-90.0); 

  fl_translate(sx/2,0);


  fl_begin_polygon();
   pdialcolor(0,0,0);
   fl_vertex(-10,-4);
   fl_vertex(-10,4);
   fl_vertex(0,0);  
  fl_end_polygon();
 

fl_pop_matrix();
}

void WidgetPDial::pdialcolor(int r,int g,int b) {
  if (active_r()) fl_color(r,g,b);
   else fl_color(160-(160-r)/3,160-(160-b)/3,160-(160-b)/3);
}

void WidgetPDial::value_cb2() {
  tipwin->value(value());
}

void WidgetPDial::value_cb(Fl_Widget*, void*data) {
  WidgetPDial *val = (WidgetPDial*)data;
        val->value_cb2();
}

void WidgetPDial::tooltip(const char * c) {
  tipwin->setText(c);
textset=true;
}
