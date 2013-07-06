/*
  ZynAddSubFX - a software synthesizer

  Alienwah.h - "AlienWah" effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef ALIENWAH_H
#define ALIENWAH_H
//#include <complex>
#include "../globals.h"
#include "Effect.h"
#include "EffectLFO.h"

using namespace std;

struct COMPLEXTYPE {
    REALTYPE a,b;
};

#define MAX_ALIENWAH_DELAY 100

/**"AlienWah" Effect*/
class Alienwah:public Effect
{
public:
    /**
     * Constructor
     * @param insetion_ 1 for insertion Effect, 0 for others
     * @param efxoutl_ Pointer to Alienwah's left channel output buffer
     * @param efxoutr_ Pointer to Alienwah's left channel output buffer
     * @return Initialized Alienwah
     */
    Alienwah(const int &insetion_,REALTYPE *const efxoutl_,REALTYPE *const efxoutr_);
    ~Alienwah();
    void out(REALTYPE *const smpsl,REALTYPE *const smpsr);

    void setpreset(unsigned char npreset);
    void changepar(const int &npar,const unsigned char &value);
    unsigned char getpar(const int &npar)const;
    void cleanup();

private:
    //Alienwah Parameters
    EffectLFO lfo;//lfo-ul Alienwah
    unsigned char Pvolume;
    unsigned char Ppanning;
    unsigned char Pdepth;//the depth of the Alienwah
    unsigned char Pfb;//feedback
    unsigned char Plrcross;//feedback
    unsigned char Pdelay;
    unsigned char Pphase;


    //Control Parameters
    void setvolume(const unsigned char &Pvolume);
    void setpanning(const unsigned char &Ppanning);
    void setdepth(const unsigned char &Pdepth);
    void setfb(const unsigned char &Pfb);
    void setlrcross(const unsigned char &Plrcross);
    void setdelay(const unsigned char &Pdelay);
    void setphase(const unsigned char &Pphase);

    //Internal Values
    REALTYPE panning,fb,depth,lrcross,phase;
    COMPLEXTYPE *oldl,*oldr;
    COMPLEXTYPE oldclfol,oldclfor;
    int oldk;
};

#endif

