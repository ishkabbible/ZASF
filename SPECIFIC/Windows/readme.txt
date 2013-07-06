
*************************************************************************************
ZynAddSubFX 2.2.1 - Windows Port
*************************************************************************************

This is the updated project file (solution) for Microsoft Visual Studio 2005.
The project includes all dependencies (except of Microsoft DirectX), so it will 
compile with just a click on the button. No extra configuration neccessary.

Change:
VSTSDK and ASIOSDK may be downloaded separately due to licensing issues. We can't 
longer provide those with the source code package. There are two placeholder 
folders: 'VSTSDK\SOURCES' and 'ASIOSDK\SOURCES', where the Steinberg SDKs schould be 
placed.

After compiling the entire solution you will have 4 different executables (each 
supporting a specific Windows audio interface) and a VST plugin.

The version numbers of the included dependency libraries are inside the 'version.txt' 
files in the subfolders of the dependency directory.

The project is based on the original linux surce code version 2.2.1. Some significant 
changes were applied. Especially to the VST plugin part. The synthesis engine uses 
32 bit floating point model (even to support the VST interface). However, the 
internal synthesis source code was not changed - only some neccessary compiler claims 
fixed to make the project compile. So there are allot of new preprocessor directives 
to ensure portability.

FLTK (a quite old version was originally used) was modified (windows handling) to
support the VST interface at least on a basic level. Unfortunately there is no 
native VSTGUI and VST support yet, so that the usage as a VST plugin is a little bit 
limited but possible. The plugin acts particulary like an external application with 
its own GUI, which is quite uncommon for a VST plugin.

Also there is a threading model used inside the VST code, which generally is a bad 
idea, we think. As far as we know, threading in a VST plugin isn't a very good idea, 
because the VST host application normally processes VST plugins in serial, not in 
parallel. It may cause some strange side effects, if the plugin that uses internal 
threads is used with a number of other plugins doing the same. So normally (unlike on 
Linux systems) audio plugins on Windows do not use threads for their internal 
processing - even not with GUI handling. Threading should be managed completely by 
the host application and plugin instances schould be single and simple processing 
threads - nothing more, nothing less - that does not mean that an audio plugin can't 
be complex ...

Missing:
Unfortunately there is no documentation for ZynAddSubFX yet. This is a pitty, because 
this synthesizer is a complex synthesis monster with countless internal parameters 
and windows. This synthesizer is even multi timbe (can play up to 16 MIDI channels 
simultanuous) and provides a pristine audio quality and performance. It is highly 
underrated and has an enormous sonic potential. Even after years (made 2002) it can 
be considered as a professional software synthesizer today - and it is free and it is 
open source ...

*************************************************************************************