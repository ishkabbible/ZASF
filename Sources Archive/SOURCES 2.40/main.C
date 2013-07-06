/*
  ZynAddSubFX - a software synthesizer

  main.c  -  Main file of the synthesizer
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

//-------------------------------------------------------------------------------
// Native Windows Port 2005 by J&H, Audio Development Team, Germany
//-------------------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef MSVC
#include <unistd.h>
#endif

//#ifndef VSTAUDIOOUT
#include <pthread.h>
//#endif


#ifdef OS_LINUX
#include <getopt.h>
#elif OS_WINDOWS
#include <windows.h>
//#include <winbase.h>
#endif

#include "Misc/Master.h"
#include "Misc/Util.h"
#include "Misc/Dump.h"
extern Dump dump;



#if (defined(NONEMIDIIN)||defined(VSTMIDIIN))
#include "Input/NULLMidiIn.h"
#endif



#ifndef DISABLE_GUI
#include "UI/MasterUI.h"
MasterUI *ui;
#endif

pthread_t thr1,thr2,thr3,thr4;
Master *master;



int swaplr=0;//1 for left-right swapping
bool usejackit=false;



MidiIn *Midi;



int Pexitprogram=0;//if the UI set this to 1, the program will exit



/*
 * User Interface thread
 */
 
 
void *thread3(void *arg){
#ifndef DISABLE_GUI
	//Fl::scheme("plastic"); // a little bit clumsy
	Fl::scheme("gtk+"); // quite nice
#ifdef OS_WINDOWS
	// anything is wrong on Windows (XP) with the default background color, so doing it manually here
	Fl::background(224, 223, 227); // standard Windows grey
#endif OS_WINDOWS
    ui->showUI();
    while (Pexitprogram==0) Fl::wait();
#endif
    return(0);
};











/*
 * Sequencer thread (test)
 */
void *thread4(void *arg){
    while (Pexitprogram==0){
	int type,par1,par2,again,midichan;
	for (int ntrack=0;ntrack<NUM_MIDI_TRACKS;ntrack++){
	    if (master->seq.play==0) break;
	    do{
		again=master->seq.getevent(ntrack,&midichan,&type,&par1,&par2);
//		printf("ntrack=%d again=%d\n",ntrack,again);
		if (type>0) {
//	    printf("%d %d  %d %d %d\n",type,midichan,chan,par1,par2);

//	if (cmdtype==MidiController) master->SetController(cmdchan,cmdparams[0],cmdparams[1]);

	
	
#ifndef VSTAUDIOOUT
		    //pthread_mutex_lock(&(master->mutex));
#endif
	    	    if (type==1){//note_on or note_off
			if (par2!=0) master->NoteOn(midichan,par1,par2);
			    else master->NoteOff(midichan,par1);
	    	    };
#ifndef VSTAUDIOOUT
		  // pthread_mutex_unlock(&(master->mutex));
#endif
		};
	    } while (again>0);
	    
	};
//if (!realtime player) atunci fac asta
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef OS_LINUX
	usleep(1000);
#elif OS_WINDOWS
	Sleep(1);
#endif
    };
    
    return(0);
}; //END OF SEQUENCER TRY









/*
 * Program initialisation
 */


void initprogram(){
#ifndef JACKAUDIOOUT
#ifndef JACK_RTAUDIOOUT
    fprintf(stderr,"\nSample Rate = \t\t%d\n",SAMPLE_RATE);
#endif
#endif
    fprintf(stderr,"Sound Buffer Size = \t%d samples\n",SOUND_BUFFER_SIZE);
    fprintf(stderr,"Internal latency = \t%.1f ms\n",SOUND_BUFFER_SIZE*1000.0/SAMPLE_RATE);
    fprintf(stderr,"ADsynth Oscil.Size = \t%d samples\n",OSCIL_SIZE);

    fflush(stderr);
    srand(time(NULL));
    denormalkillbuf=new REALTYPE [SOUND_BUFFER_SIZE];
    for (int i=0;i<SOUND_BUFFER_SIZE;i++) denormalkillbuf[i]=(RND-0.5)*1e-16;
    
    OscilGen::tmpsmps=new REALTYPE[OSCIL_SIZE];
    newFFTFREQS(&OscilGen::outoscilFFTfreqs,OSCIL_SIZE/2);
    
    master=new Master();
    master->swaplr=swaplr;





#if (defined(NONEMIDIIN)||(defined(VSTMIDIIN)))
    Midi=new NULLMidiIn();
#endif
#ifndef DISABLE_GUI
    ui=new MasterUI(master,&Pexitprogram);
#endif
}; //Init Program





/*
 * Program exit
 */
void exitprogram()
{


#ifndef DISABLE_GUI
    delete(ui);
#endif
    delete(Midi);
    delete(master); 

//    pthread_mutex_unlock(&master->mutex);
    delete(denormalkillbuf);
    delete(OscilGen::tmpsmps);
    deleteFFTFREQS(&OscilGen::outoscilFFTfreqs);
};




#ifdef OS_WINDOWS
#define ARGSIZE 100
 char winoptarguments[ARGSIZE];
 
 
 
 char getopt(int argc, char *argv[], const char *shortopts, int *index){
    winoptarguments[0]=0;
    char result=0;
    
    if (*index>=argc) return(-1);
    
    if (strlen(argv[*index])==2) 
	if (argv[*index][0]=='-') {
	    result=argv[*index][1];
	    if (*index+1<argc) {
		snprintf(winoptarguments,ARGSIZE,"%s",argv[*index+1]);
	    };
	};
    (*index)++;
    return(result);
 }; //command line arguments
 int opterr=0;
#undef ARGSIZE
 
#endif  //OS_Windows





#ifdef VSTAUDIOOUT

#include "Output/VSTaudiooutput.h"

#ifdef MSVC
extern "C" __declspec(dllexport) AEffect *amain(audioMasterCallback audioMaster);
#else
#define main main_plugin
extern "C" __declspec(dllexport) AEffect *main_plugin(audioMasterCallback audioMaster);
#endif

//-------------------------------------------------------------------------------






int instances = -1;  //uninitialised


AEffect *amain (audioMasterCallback audioMaster)
{
	//if (audioMaster(0,audioMasterVersion,0,0,0,0) != 0)
	//return(0);

	// initializations, shared by all instances !? not sure here //of course, dumb ass!!!!!!!!!
	if (instances == -1)
	{
		config.init();

		SAMPLE_RATE = config.cfg.SampleRate;	
		SOUND_BUFFER_SIZE = config.cfg.SoundBufferSize;
		OSCIL_SIZE = config.cfg.OscilSize;
		swaplr = config.cfg.SwapStereo;

		instances = 0;
    };


    AudioEffect *sintetizator = new VSTSynth(audioMaster); //calls constructor and init funcs below
    return sintetizator->getAeffect();
};
//-------------------------------------------------------------------------------



void* hInstance;



BOOL WINAPI DllMain (HINSTANCE hInst,DWORD dwReason,LPVOID lpvReserved)
{
    hInstance = hInst;

	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:

// initializing the thread library if static linked
#ifdef PTW32_STATIC_LIB
//#ifndef VSTAUDIOOUT
		pthread_win32_process_attach_np();
//#endif
#endif

		break;

	case DLL_PROCESS_DETACH:

// finalizing the thread library if static linked
#ifdef PTW32_STATIC_LIB
//#ifndef VSTAUDIOOUT
		pthread_win32_process_detach_np();
//#endif
#endif

		// The next was made because that "reloading crash" on multiple try to load. 
		// Well, because when FLTK is used in a DLL here, the window class 
		// remains registered and will never be unregistered on *native Win32* 
		// (NT/2000/XP) if the dll is unloaded. The class remains registered.

		// That is actually a strange OS behaviour, but then it also causes 
		// an access voilation in the Windows kernel files when reloading the 
		// dll, registering the class again and calling CreateWindowEx().

		// It has probably also something to do with the currently used threading model.

		// So we simply do a trick and unregister the window class here 
		// in hope, all applications unload the DLL immediately after usage.
		// Otherwise the problem still remains!

		// It works now at least on Cubase VST, the VST reference host ...

		UnregisterClass("FLTK", GetModuleHandle(0));

		break;
	}

    return (TRUE);
};
//-------------------------------------------------------------------------------






//-------------------------------------------------------------------------------
//Parts of the VSTSynth class

//contructor to initialise VST
VSTSynth::VSTSynth (audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, 1, 0)
{
	vmaster = 0;
	gui = 0;

	if (audioMaster)
	{
		setNumInputs(0);
		setNumOutputs(2);
		setUniqueID('ZASF');
		canProcessReplacing();
		isSynth();
		programsAreChunks();
		needIdle();
		//hasVu(false);
		//hasClip(false);
	};

    p_exitprogram = 0;

#ifdef SINGLEINSTANCE
	Pexitprogram = 0;
	initprogram1();
    vmaster = master;
	vmaster->swaplr = swaplr;
#else
	if (instances == 0) // shared initialization 
	{
		srand(time(NULL));
		denormalkillbuf = new REALTYPE [SOUND_BUFFER_SIZE];
		for (int i = 0; i < SOUND_BUFFER_SIZE; i++) 
		denormalkillbuf[i] = (RND - 0.5) * 1e-16;
    
		OscilGen::tmpsmps = new REALTYPE[OSCIL_SIZE];
		newFFTFREQS(&OscilGen::outoscilFFTfreqs, OSCIL_SIZE / 2);
    
		Midi = new NULLMidiIn();
	}
    vmaster = new Master();
    gui = new MasterUI(vmaster, &p_exitprogram);
    vmaster->swaplr = swaplr;

#endif // SINGLEINSTANCE


	editor = new VSTSynthEditor(this);
	instances++;

	//suspend();
};
//-------------------------------------------------------------------------------
VSTSynth::~VSTSynth()
{
    instances--;


#ifdef SINGLEINSTANCE
	exitprogram1();
#else
	delete gui;
    delete vmaster; 
	if (instances <= 0) // deleting finally all the shared stuff
	{
		delete Midi;
		Midi = 0;
		delete denormalkillbuf;
		denormalkillbuf = 0;
		delete OscilGen::tmpsmps;
		OscilGen::tmpsmps = 0;
		deleteFFTFREQS(&OscilGen::outoscilFFTfreqs);
	}
#endif // SINGLEINSTANCE
};






//-------------------------------------------------------------------------------
long VSTSynth::processEvents(VstEvents *events)
{
    for (int i = 0; i < events->numEvents; i++)
	{
		
		if ((events->events[i])->type != kVstMidiType) continue;

		VstMidiEvent *ev= (VstMidiEvent*) events->events[i];
		unsigned char *data = (unsigned char*)ev->midiData;
		int status = data[0] / 16;
		int cmdchan = data[0] & 0x0f;
		int cntl;

		switch (status)
		{
			case 0x8:
				vmaster->NoteOff(cmdchan,data[1]&0x7f);
				break;
			case 0x9:
				if (data[2] == 0) vmaster->NoteOff(cmdchan, data[1] & 0x7f);
				else vmaster->NoteOn(cmdchan, data[1] & 0x7f, data[2] & 0x7f);
				break;
			case 0xB: 
				cntl = Midi->getcontroller(data[1] & 0x7f);
				vmaster->SetController(cmdchan, cntl, data[2] & 0x7f);
				break;
			case 0xE: 
				vmaster->SetController(cmdchan, C_pitchwheel, data[1] + data[2] * (long int) 128 - 8192);
				break;
		};

    };
	return (1);
};

//-------------------------------------------------------------------------------
long VSTSynth::getChunk(void** data, bool isPreset)
{
    int size = 0;
    size = vmaster->getalldata((char**)data);
    return ((long)size);
};


//-------------------------------------------------------------------------------
long VSTSynth::setChunk(void *data, long size, bool isPreset)
{
    vmaster->putalldata((char*)data, size);
	vmaster->applyparameters();
	gui->refresh_master_ui();

    return (0);
};
//-------------------------------------------------------------------------------
// Not all hosts call that obviously, so we probably get those big problems with FLTK then.
long VSTSynth::fxIdle() // test
{
	Fl::wait(0.01);	
	return 0;
} 

//-------------------------------------------------------------------------------
// GUI implementation:
//-------------------------------------------------------------------------------
// The implementation of the FLTK directly inside the Editor class failed because of
// some strange behaviours. Especially the missing fxIdle() call requested on needIdle() 
// on some host applications caused probably those problems. But I am not that sure ...
// Probably there are also some other problems with the FLTK and VSTGUI here.

#ifdef GUIEMBEDDED
void *pParentWindow = 0;
#endif



VSTSynthEditor::VSTSynthEditor(AudioEffect *effect)
: AEffGUIEditor (effect) 
{
	bOpened = false;

	// try to put the FLTK GUI directly into the editor ...
	//VSTSynth* synth = (VSTSynth*)effect;
	//gui = new MasterUI(synth->vmaster, &(synth->p_exitprogram));

	// standard VST editors background
	hBackground = new CBitmap (1000);    //loads the included resource id=1000 bitmap
	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
#ifdef GUIEMBEDDED
	rect.right  = 820;
	rect.bottom = 640;
#else
	rect.right  = (short)hBackground->getWidth();
	rect.bottom = (short)hBackground->getHeight();
#endif
}
//-------------------------------------------------------------------------------
VSTSynthEditor::~VSTSynthEditor()
{
    //gui->hideUI_VST();
	//delete gui;

	if (hBackground)
	hBackground->forget ();
}
//-------------------------------------------------------------------------------
void VSTSynthEditor::resize(int width, int height)
{
#ifndef GUIEMBEDDED
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = width;
	rect.bottom = height;

	frame->setSize(width, height);
	update();
#endif
}
//-------------------------------------------------------------------------------
long VSTSynthEditor::open(void *ptr)
{
	AEffGUIEditor::open (ptr);

#ifdef GUIEMBEDDED
	pParentWindow = ptr;
#else
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	frame = new CFrame (size, ptr, this);
	frame->setBackground (hBackground);
#endif

	VSTSynth* synth = (VSTSynth*)effect;
	synth->gui->showUI_VST();
	//synth->gui->showUI();

	bOpened = true;
	return bOpened;
}
//-------------------------------------------------------------------------------

void VSTSynthEditor::close()
{
	bOpened = false;

#ifdef GUIEMBEDDED
	pParentWindow = 0;
#endif

	VSTSynth* synth = (VSTSynth*)effect;
    synth->gui->hideUI_VST();
	

#ifndef GUIEMBEDDED
	delete frame;
	frame = 0;
#endif
}
//-------------------------------------------------------------------------------
void VSTSynthEditor::idle()
{
	AEffGUIEditor::idle();
	Fl::wait(0.01);
}
//-------------------------------------------------------------------------------

#endif // VSTAUDIOOUT


