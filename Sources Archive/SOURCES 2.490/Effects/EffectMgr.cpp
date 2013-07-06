/*
  ZynAddSubFX - a software synthesizer

  EffectMgr.C - Effect manager, an interface betwen the program and effects
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

#include "EffectMgr.h"
#include "../Misc/ParamID.h"

EffectMgr::EffectMgr(int insertion_,pthread_mutex_t *mutex_,int nPart_, int posefx_)
        :insertion(insertion_),
        efxoutl(new REALTYPE[SOUND_BUFFER_SIZE]),
        efxoutr(new REALTYPE[SOUND_BUFFER_SIZE]),
        filterpars(NULL),nefx(0),efx(NULL),mutex(mutex_),dryonly(false)
{
    setpresettype("Peffect"); /**\todo Figure out what this is doing
                               * , as it might be another leaky abstraction.*/

	nPart = nPart_;
	posefx = posefx_;
//    efx=NULL;
//    nefx=0;
//    insertion=insertion_;
//    mutex=mutex_;
//    efxoutl=new REALTYPE[SOUND_BUFFER_SIZE];
//    efxoutr=new REALTYPE[SOUND_BUFFER_SIZE];
    for (int i=0;i<SOUND_BUFFER_SIZE;i++) {
        efxoutl[i]=0.0;
        efxoutr[i]=0.0;
    };
//    filterpars=NULL;
//    dryonly=false;
    defaults();
}


EffectMgr::~EffectMgr()
{
    if (efx!=NULL) delete efx;
    delete []efxoutl;
    delete []efxoutr;
	//if (parID) delete parID;
}

void EffectMgr::defaults()
{
    changeeffect(0);
    setdryonly(false);
}

/*
 * Change the effect
 */
void EffectMgr::changeeffect(int nefx_)
{
    cleanup();
    if (nefx==nefx_) return;
    nefx=nefx_;
    for (int i=0;i<SOUND_BUFFER_SIZE;i++) {
        efxoutl[i]=0.0;
        efxoutr[i]=0.0;
    };

	ParamID *neweffect;
	ParamID *neweffectfilter;

	//Filter identification set
	neweffect = new ParamID();
	{		
	neweffect->code = 0;
	neweffect->group = G_FX;
	neweffect->index = posefx;
	neweffect->location = W_FX;
	neweffect->maxval = 0;
	neweffect->minval = 0;
	neweffect->nPart = nPart;
	neweffect->nvoice = -1;
	neweffect->SetName("FX");
	neweffect->type = S_FX;	
	};

	neweffectfilter = new ParamID();
	{
	neweffectfilter->code = 0;
	neweffectfilter->group = G_FILTPARAM;
	neweffectfilter->index = posefx;
	neweffectfilter->location = W_DYNFILT;
	neweffectfilter->maxval = 0;
	neweffectfilter->minval = 0;
	neweffectfilter->nPart = nPart;
	neweffectfilter->nvoice = -1;
	neweffectfilter->SetName("FX:DynFilter");
	neweffectfilter->type = S_FX;
	}


    if (efx!=NULL) 
	{
		Effect* oldefx;
		oldefx = efx;
		efx = NULL;
		
		delete oldefx;
		//nasty was bug here!!!		
	};
    switch (nefx) { /**\todo replace leaky abstraction*/
    case 1:
        efx=new Reverb(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 2:
        efx=new Echo(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 3:
        efx=new Chorus(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 4:
        efx=new Phaser(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 5:
        efx=new Alienwah(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 6:
        efx=new Distorsion(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 7:
        efx=new EQ(insertion,efxoutl,efxoutr,neweffect);
        break;
    case 8:
        efx=new DynamicFilter(insertion,efxoutl,efxoutr,neweffect,neweffectfilter);
        break;
        //put more effect here
    default:
        efx=NULL;
        break;//no effect (thru)
    };

    if (efx!=NULL) filterpars=efx->filterpars;
}

/*
 * Obtain the effect number
 */
int EffectMgr::geteffect()
{
    return (nefx);
}

/*
 * Cleanup the current effect
 */
void EffectMgr::cleanup()
{
    if (efx!=NULL) efx->cleanup();
}


/*
 * Get the preset of the current effect
 */

unsigned char EffectMgr::getpreset()
{
    if (efx!=NULL) return(efx->Ppreset);
    else return(0);
}

/*
 * Change the preset of the current effect
 */
void EffectMgr::changepreset_nolock(unsigned char npreset)
{
    if (efx!=NULL) efx->setpreset(npreset);
}

/*
 * Change the preset of the current effect(with thread locking)
 */
void EffectMgr::changepreset(unsigned char npreset)
{
    //pthread_mutex_lock(mutex);
    changepreset_nolock(npreset);
    //pthread_mutex_unlock(mutex);
}


/*
 * Change a parameter of the current effect
 */
void EffectMgr::seteffectpar_nolock(int npar,unsigned char value)
{
    if (efx==NULL) return;
    efx->changepar(npar,value);
}

/*
 * Change a parameter of the current effect (with thread locking)
 */
void EffectMgr::seteffectpar(int npar,unsigned char value)
{
    //pthread_mutex_lock(mutex);
    seteffectpar_nolock(npar,value);
    //pthread_mutex_unlock(mutex);
}

/*
 * Get a parameter of the current effect
 */
unsigned char EffectMgr::geteffectpar(int npar)
{
    if (efx==NULL) return(0);
    return(efx->getpar(npar));
}


/*
 * Apply the effect
 */
void EffectMgr::out(REALTYPE *smpsl,REALTYPE *smpsr)
{
    int i;
    if (efx==NULL) {
        if (insertion==0)
            for (i=0;i<SOUND_BUFFER_SIZE;i++) {
                smpsl[i]=0.0;
                smpsr[i]=0.0;
                efxoutl[i]=0.0;
                efxoutr[i]=0.0;
            };
        return;
    };
    for (i=0;i<SOUND_BUFFER_SIZE;i++) {
        smpsl[i]+=denormalkillbuf[i];
        smpsr[i]+=denormalkillbuf[i];
        efxoutl[i]=0.0;
        efxoutr[i]=0.0;
    };
    efx->out(smpsl,smpsr);  //also calculates the efxoutl | efxourr in EffectMgr; same pointers

    REALTYPE volume=efx->volume;

    if (nefx==7) {//this is need only for the EQ effect
        /**\todo figure out why*/
        for (i=0;i<SOUND_BUFFER_SIZE;i++) {
            smpsl[i]=efxoutl[i];
            smpsr[i]=efxoutr[i];
        };
        return;
    };

    //Insertion effect
    if (insertion!=0) {
        REALTYPE v1,v2;
        if (volume<0.5) {
            v1=1.0;
            v2=volume*2.0;
        } else {
            v1=(1.0-volume)*2.0;
            v2=1.0;
        };
        if ((nefx==1)||(nefx==2)) v2*=v2;//for Reverb and Echo, the wet function is not liniar

        if (dryonly) {//this is used for instrument effect only
            for (i=0;i<SOUND_BUFFER_SIZE;i++) {
                smpsl[i]*=v1;
                smpsr[i]*=v1;
                efxoutl[i]*=v2;
                efxoutr[i]*=v2;
            };
        } else {//normal instrument/insertion effect
            for (i=0;i<SOUND_BUFFER_SIZE;i++) {
                smpsl[i]=smpsl[i]*v1+efxoutl[i]*v2;		//where was efxout here computed????
                smpsr[i]=smpsr[i]*v1+efxoutr[i]*v2;
            };
        };
    } else {//System effect
        for (i=0;i<SOUND_BUFFER_SIZE;i++) {
            efxoutl[i]*=2.0*volume;
            efxoutr[i]*=2.0*volume;
            smpsl[i]=efxoutl[i];
            smpsr[i]=efxoutr[i];
        };
    };

}

/*
 * Get the effect volume for the system effect
 */
REALTYPE EffectMgr::sysefxgetvolume()
{
    if (efx==NULL) return (1.0);
    else return(efx->outvolume);
}


/*
 * Get the EQ response
 */
REALTYPE EffectMgr::getEQfreqresponse(REALTYPE freq)
{
    if (nefx==7) return(efx->getfreqresponse(freq));
    else return(0.0);
}


void EffectMgr::setdryonly(bool value)
{
    dryonly=value;
}

void EffectMgr::add2XML(XMLwrapper *xml)
{
    xml->addpar("type",geteffect());

    if ((efx==NULL)||(geteffect()==0)) return;
    xml->addpar("preset",efx->Ppreset);

    xml->beginbranch("EFFECT_PARAMETERS");
    for (int n=0;n<128;n++) { /**\todo evaluate who should oversee saving
                                   * and loading of parameters*/
        int par=geteffectpar(n);
        if (par==0) continue;
        xml->beginbranch("par_no",n);
        xml->addpar("par",par);
        xml->endbranch();
    };
    if (filterpars!=NULL) {
        xml->beginbranch("FILTER");
        filterpars->add2XML(xml);
        xml->endbranch();
    };
    xml->endbranch();
}

void EffectMgr::getfromXML(XMLwrapper *xml)
{
    changeeffect(xml->getpar127("type",geteffect()));

    if ((efx==NULL)||(geteffect()==0)) return;

    efx->Ppreset=xml->getpar127("preset",efx->Ppreset);

    if (xml->enterbranch("EFFECT_PARAMETERS")) {
        for (int n=0;n<128;n++) {
            seteffectpar_nolock(n,0);//erase effect parameter
            if (xml->enterbranch("par_no",n)==0) continue;

            int par=geteffectpar(n);
            seteffectpar_nolock(n,xml->getpar127("par",par));
            xml->exitbranch();
        };
        if (filterpars!=NULL) {
            if (xml->enterbranch("FILTER")) {
                filterpars->getfromXML(xml);
                xml->exitbranch();
            };
        };
        xml->exitbranch();
    };
    cleanup();
}

