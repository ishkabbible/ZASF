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

//#define _CRTDBG_MAP_ALLOC




#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//#include <malloc.h>
//#include <crtdbg.h>


#ifndef MSVC
#include <unistd.h>
#endif

//#ifndef VSTAUDIOOUT
#include <pthread.h>
#include "Output\Vstaudiooutput.h"
//#endif


#ifdef OS_LINUX
#include <getopt.h>
#elif OS_WINDOWS
#include <windows.h>
//#include <winbase.h>
#endif

#include "Misc/Master.h"
#include "Misc/Util.h"
//#include "Misc/Dump.h"
//extern Dump dump;

#include "Misc/MidiRL.h"
//MidiRL midiRL;

//int LearningMidi;



#if (defined(NONEMIDIIN)||defined(VSTMIDIIN))
#include "Input/NULLMidiIn.h"
#endif




#include "UI/MasterUI.h"
//#include "Misc/MidiRL.h"
//MasterUI *ui;


//pthread_t thr1,thr2,thr3,thr4;
//Master *GlobalMaster;
int tid;
int ten;
int disable_thread;
int ontop;
int loadgui;
int unloadgui;
int main_x; 
int main_y;
void* synth_transfer;

float minEnvDB;


EXTERN_C IMAGE_DOS_HEADER __ImageBase;


pthread_t hGUIThread;
Config* config;



int swaplr=0;//1 for left-right swapping
bool usejackit=false;



MidiIn *Midi;



int Pexitprogram=0;//if the UI set this to 1, the program will exit



void *GUIThread (void *synth_)
{
	VSTSynth* synth;

	//synth = (VSTSynth*) synth_;

	while (ten) {	
	

		while (loadgui) {

			loadgui = 0;
			if (!synth_transfer) {MessageBox(NULL,"synth_transfer error","ERROR",0); pthread_exit(NULL); };
			synth = (VSTSynth*) synth_transfer;
			
			//make a gui
			//move this into GUI thread
			synth->gui = new MasterUI(synth->vmaster,new int);
			synth->midiRL->init(synth->vmaster,synth->gui);
			synth->midiRL->LoadGUI();
			synth->gui->midilearn->refresh();
			synth->gui->showUI_VST();
			synth->b_ten = 1;
			synth_transfer = NULL;
		}; //loadgui


			
	Fl::wait(0.01);
	Sleep(10);


		while (unloadgui) {			

			
			if (!synth_transfer) { 
				//MessageBox(NULL,"synth_transfer error","ERROR",0); 
				unloadgui =0;
				pthread_win32_thread_detach_np();
				pthread_exit(NULL); 				
				return NULL;
			};
			synth = (VSTSynth*) synth_transfer;


			// destroy GUI (close commanded)
			if (synth->gui) {
				
				
			
				synth->midiRL->init(synth->vmaster, NULL);
				synth->gui->hideUI_VST();		
			
			
				synth->gui_clean = synth->gui;
				synth->gui = NULL;
				delete synth->gui_clean;
				synth->gui_clean = NULL;

			};
			unloadgui = 0;
		}; //unload gui
   
   
   
	  
	}; //while (ten)
	pthread_win32_thread_detach_np();
   pthread_exit(NULL);
	
   return NULL;
  
}








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
int alloc_instances = -1;


AEffect *amain (audioMasterCallback audioMaster)
{
	//if (audioMaster(0,audioMasterVersion,0,0,0,0) != 0)
	//return(0);

	if (!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
	{
	MessageBox (NULL,"old vst version",NULL,0);
	return 0;
	};

	// initializations, shared by all instances !? not sure here //of course, dumb ass!!!!!!!!!
 	if (instances == -1)
	{		
		////VDX SAYS: BAD CODE FOR WIN7!!!!
		//
		config = new Config();
		config->init();											//inits with default values	but also reads cfg file

		SAMPLE_RATE = config->cfg.SampleRate;	
		SOUND_BUFFER_SIZE = config->cfg.SoundBufferSize;
		OSCIL_SIZE = config->cfg.OscilSize;
		swaplr = config->cfg.SwapStereo;
		ontop = config->cfg.DumpAppend;
		disable_thread = config->cfg.DumpNotesToFile;

		minEnvDB = float(config->cfg.minEnvelopeDB); 

		instances = 0;
    };
	AudioEffect *sintetizator;
	VSTSynth *cb_sintetizator;

 //   MessageBox(NULL,"Creating instance",NULL,0);
	//sintetizator = new VSTSynth(audioMaster); //calls constructor and init funcs below
	//
	////MessageBox(NULL,"Deleting instance",NULL,0);
	//cb_sintetizator = (AudioEffect*)sintetizator->getAeffect()->object;
	//delete cb_sintetizator;

	//MessageBox(NULL,"Creating instance",NULL,0);
	//sintetizator = new VSTSynth(audioMaster); //calls constructor and init funcs below
	//
	////MessageBox(NULL,"Deleting instance",NULL,0);
	//delete sintetizator->getAeffect();

	//MessageBox(NULL,"Creating instance",NULL,0);
	//sintetizator = new VSTSynth(audioMaster); //calls constructor and init funcs below
	//
	////MessageBox(NULL,"Deleting instance",NULL,0);
	//delete sintetizator->getAeffect();



	//MessageBox(NULL,"Creating final instance!",NULL,0);
    sintetizator = cb_sintetizator = new VSTSynth(audioMaster); //calls constructor and init funcs below
	//cb_sintetizator->b_unloadFLTK = config->cfg.DumpNotesToFile;


	SAMPLE_RATE = sintetizator->getSampleRate();
    return sintetizator->getAeffect();
};
//-------------------------------------------------------------------------------



void* hInstance;



BOOL WINAPI DllMain (HINSTANCE hInst,DWORD dwReason,LPVOID lpvReserved)
{
    hInstance = hInst;

	//test with a breakpoint here:
	//*((int*)0) = 98;

	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//_crtBreakAlloc=228125;

// initializing the thread library if static linked
#ifdef PTW32_STATIC_LIB
//#ifndef VSTAUDIOOUT
		pthread_win32_process_attach_np();

		extern int ten;
	    extern int loadgui;
	    extern int unloadgui;
	    extern void* synth_transfer;
		extern int main_x;
		extern int main_y;

		//Sleep(1000);

		main_x = 0;
		main_y = 0;
		Midi = NULL;

		//if (ten==0) { //start_thread

			ten = 0;
			loadgui = 0;
			unloadgui= 0;

			synth_transfer = NULL;

			{//create a gui thread

				pthread_attr_t tattr;
				int ret;
				
				ten = 1;
				ret = pthread_attr_init(&tattr);
				//ret = pthread_attr_*(&tattr,SOME_ATRIBUTE_VALUE_PARAMETER);
				ret = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);






				if (!tid) pthread_create(&hGUIThread,&tattr, GUIThread, NULL);
				pthread_attr_destroy(&tattr);

			};
		//}; //start thread

		


		
//#endif
#endif

		break;


	case DLL_THREAD_DETACH:
		//pthread_win32_thread_detach_np();



		break;

	case DLL_PROCESS_DETACH:

// finalizing the thread library if static linked
#ifdef PTW32_STATIC_LIB
//#ifndef VSTAUDIOOUT
		//Sleep(500);
		//pthread_win32_process_detach_np();
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
		
		//shared cleanup
		//MessageBox(NULL,"Shared Cleanup",NULL,0);
		extern int ten;
		extern int unloadgui;
		extern void* synth_transfer;

		synth_transfer = NULL;		
		unloadgui = 1;
		while (unloadgui) Sleep(10);

		ten = 0;

		//pthread_kill(hGUIThread, 9);
		

		config->save();

		
		if (Midi)
			delete Midi;
		Midi = NULL;
		delete denormalkillbuf;
		denormalkillbuf = 0;
		delete OscilGen::tmpsmps;
		OscilGen::tmpsmps = 0;
		deleteFFTFREQS(&OscilGen::outoscilFFTfreqs);

		
		delete config;

		
		


		Sleep(500);
		
		
		
		//UnregisterClass("FLTK", (HINSTANCE)&__ImageBase);
		UnregisterClass("FLTK", GetModuleHandle(0));
		//_CrtDumpMemoryLeaks();

		pthread_win32_process_detach_np();

		
		
		

		break;
	}

    return (TRUE);
};
//-------------------------------------------------------------------------------






//-------------------------------------------------------------------------------
//Parts of the VSTSynth class

//contructor to initialise VST

//***VDX: You can change here the number of parameters reported to the host!!!!
//AudioEffect (audioMasterCallback audioMaster, long numPrograms, long numParams);
VSTSynth::VSTSynth (audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, 1, 100)		//set 20 vst parameters 0-19
{
	

	

	vmaster = NULL;
	gui = NULL;
	midiRL = NULL;
	b_ten = 0;

	if (audioMaster)
	{
		setNumInputs(8);	//4 stereo inputs :D
		setNumOutputs(34);	//17 stereo outputs; 1 = mixed parts; rest are separate parts individually
		setUniqueID('ZASF');
		canProcessReplacing();
		isSynth();
		programsAreChunks();
		needIdle();
		//hasVu(false);
		//hasClip(false);
		//suspend();
	};

    p_exitprogram = 0;

	

	if (instances == 0) // shared initialization 
	{
		//shared init
		srand(time(NULL));
		denormalkillbuf = new REALTYPE [SOUND_BUFFER_SIZE];
		for (int i = 0; i < SOUND_BUFFER_SIZE; i++) 
		denormalkillbuf[i] = (RND - 0.5) * 1e-16;
    
		OscilGen::tmpsmps = new REALTYPE[OSCIL_SIZE];
		newFFTFREQS(&OscilGen::outoscilFFTfreqs, OSCIL_SIZE / 2);
    
		Midi = new NULLMidiIn();


		
	}
	
	
	Fl::background(224, 223, 227);
	
	




	editor = new VSTSynthEditor(this);
	instances++;

	suspend();
};
//-------------------------------------------------------------------------------
VSTSynth::~VSTSynth()
{
	
	
    instances--;
	//MessageBox(NULL,"deleting synth",NULL,0);


#ifdef SINGLEINSTANCE
	exitprogram1();
#else
	//delete this->midiRL;
	//delete this->gui;
    //delete this->vmaster; 

	//if (instances >=0) {

	//vmaster->shutup = 1;
	//Sleep(500);
	
	

	
	//};

	if (instances <= 0) // deleting finally all the shared stuff
	{
		
	}
#endif // SINGLEINSTANCE
};


void VSTSynth::close() {

	synth_transfer = (void*) this;
	unloadgui = 1;
	
	//Sleep(100);

	while (unloadgui) Sleep(10);
	
	//Sleep(100);

	//MessageBox(NULL,"close() called",NULL,0);
	
	//MessageBox(NULL,"del gui",NULL,0);
	/*if (gui){
		gui_clean = gui;
		gui = NULL;
		delete gui_clean;
		gui_clean = NULL;
	};*/
	//MessageBox(NULL,"del master",NULL,0);
	
	if (vmaster) {
		vmaster_clean = vmaster;
		vmaster = NULL;
		delete vmaster_clean;
		vmaster_clean = NULL;
	};
	
	//MessageBox(NULL,"del midiRL",NULL,0);
	if (midiRL) {
		midiRL_clean = midiRL;
		midiRL = 0;
		delete midiRL_clean;
		midiRL_clean = NULL;
	};
	


};

void VSTSynth::open() {
	//MessageBox(NULL,"open() called",NULL,0);
	midiRL = new MidiRL();
	
    vmaster = new Master(this);
	vmaster->shutup = 1;
    
	//gui = new MasterUI(vmaster, &p_exitprogram);

	midiRL->init(vmaster,gui);
	Sleep(200);
	vmaster->ShutUp();

	
    vmaster->swaplr = swaplr;

			

};






//-------------------------------------------------------------------------------
long VSTSynth::processEvents(VstEvents *events)
{
	if (vmaster == 0) return 0;
    for (int i = 0; i < events->numEvents; i++)
	{
		
		if ((events->events[i])->type != kVstMidiType) continue;

		VstMidiEvent *ev= (VstMidiEvent*) events->events[i];
		unsigned char *data = (unsigned char*)ev->midiData;
		int status = data[0] / 16;
		int cmdchan = data[0] & 0x0f;
		int cntl;
		//extern MidiRL midiRL;

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

				//***
				cntl = (data[1] & 0x7f);
				midiRL->Update_Realtime_Parameter (cmdchan+1,cntl,data[2] & 0x7f);
				break;
			case 0xE: 				
				vmaster->SetController(cmdchan, C_pitchwheel, data[1] + data[2] * (long int) 128 - 8192);
				break;
		};

    };
	return (1);
};


// Not all hosts call that obviously, so we probably get those big problems with FLTK then.
long VSTSynth::fxIdle() // test
{
	/*
	if (vmaster == 0) return 0;
	if (midiRL ==0) return 0;
	if (gui ==0) return 0;
	Fl::wait(0.01);	
	*/
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

	/*if (gui){
		gui_clean = gui;
		gui = NULL;
		delete gui_clean;
		gui_clean = NULL;
	};*/
	//delete gui;

	if (effect) {
		VSTSynth* synth = (VSTSynth*)effect;
		if (synth->gui){
			synth->gui->hideUI_VST();
			synth->gui_clean = synth->gui;
			synth->gui = NULL;
			delete synth->gui_clean;
			synth->gui_clean = NULL;
		//synth->gui->showUI();
		};
	};

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

	if (effect) {
		VSTSynth* synth = (VSTSynth*)effect;
		if (synth->gui){
			//synth->gui->showUI_VST();
		
		} else
		{
			if (synth->vmaster ==0) return 0;

			extern int loadgui;
			extern void* synth_transfer;

			synth_transfer = (void*)synth;
			loadgui = 1;	
			
			


			
			/*
			//move this into GUI thread
			synth->gui = new MasterUI(synth->vmaster,new int);
			synth->midiRL->init(synth->vmaster,synth->gui);
			synth->midiRL->LoadGUI();
			synth->gui->midilearn->refresh();
			synth->gui->showUI_VST();
			*/
		};
	};

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


	if (effect) {
	VSTSynth* synth = (VSTSynth*)effect;
    
	
	extern void* synth_transfer;

	synth_transfer = (void*) synth;	
	synth->b_ten = 0;
	unloadgui = 1;
	
	Sleep(200);

	};
	
	
	/* move to GUI thread
	if (synth->gui)
		synth->gui->hideUI_VST();


		//if (synth->vmaster->b_turbo) {
	if (synth->b_unloadFLTK) {
		if (synth->vmaster->b_turbo) 
		{
			//try releasing gui from memory
			
			synth->gui_clean = synth->gui;
			synth->gui = NULL;
			delete synth->gui_clean;
			synth->gui_clean = NULL;
		//synth->gui->showUI();
		} }
		else {
			synth->gui_clean = synth->gui;
			synth->gui = NULL;
			delete synth->gui_clean;
			synth->gui_clean = NULL;

		};
	
	};
	*/


		

	

#ifndef GUIEMBEDDED
	delete frame;
	frame = 0;
#endif
}
//-------------------------------------------------------------------------------
void VSTSynthEditor::idle()
{
	AEffGUIEditor::idle();
	
	/*
	if (effect)
	{   VSTSynth* synth = (VSTSynth*)effect;
	if (synth->gui) {		
		Fl::wait(0.01);
	};

	};
	*/
}
//-------------------------------------------------------------------------------

#endif // VSTAUDIOOUT


