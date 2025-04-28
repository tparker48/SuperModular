
/* ------------------------------------------------------------
name: "FaustSynth"
Code generated with Faust 2.23.4 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef __mydsp_H__
#define __mydsp_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}

    // -- widget's layouts

    virtual void openTabBox(const char *label) = 0;
    virtual void openHorizontalBox(const char *label) = 0;
    virtual void openVerticalBox(const char *label) = 0;
    virtual void closeBox() = 0;

    // -- active widgets

    virtual void addButton(const char *label, REAL *zone) = 0;
    virtual void addCheckButton(const char *label, REAL *zone) = 0;
    virtual void addVerticalSlider(const char *label, REAL *zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char *label, REAL *zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char *label, REAL *zone, REAL init, REAL min, REAL max, REAL step) = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char *label, REAL *zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char *label, REAL *zone, REAL min, REAL max) = 0;

    // -- soundfiles

    virtual void addSoundfile(const char *label, const char *filename, Soundfile **sf_zone) = 0;

    // -- metadata declarations

    virtual void declare(REAL *zone, const char *key, const char *val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

protected:
    std::vector<std::string> fControlsLevel;

public:
    PathBuilder() {}
    virtual ~PathBuilder() {}

    std::string buildPath(const std::string &label)
    {
        std::string res = "/";
        for (size_t i = 0; i < fControlsLevel.size(); i++)
        {
            res += fControlsLevel[i];
            res += "/";
        }
        res += label;
        std::replace(res.begin(), res.end(), ' ', '_');
        return res;
    }

    void pushLabel(const std::string &label) { fControlsLevel.push_back(label); }
    void popLabel() { fControlsLevel.pop_back(); }
};

#endif // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{

protected:
    // Complete path map
    std::map<std::string, FAUSTFLOAT *> fPathZoneMap;

    // Label zone map
    std::map<std::string, FAUSTFLOAT *> fLabelZoneMap;

public:
    MapUI() {}
    virtual ~MapUI() {}

    // -- widget's layouts
    void openTabBox(const char *label)
    {
        pushLabel(label);
    }
    void openHorizontalBox(const char *label)
    {
        pushLabel(label);
    }
    void openVerticalBox(const char *label)
    {
        pushLabel(label);
    }
    void closeBox()
    {
        popLabel();
    }

    // -- active widgets
    void addButton(const char *label, FAUSTFLOAT *zone)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addCheckButton(const char *label, FAUSTFLOAT *zone)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addVerticalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addHorizontalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }

    // -- passive widgets
    void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }
    void addVerticalBargraph(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label] = zone;
    }

    // -- soundfiles
    virtual void addSoundfile(const char *label, const char *filename, Soundfile **sf_zone) {}

    // -- metadata declarations
    void declare(FAUSTFLOAT *zone, const char *key, const char *val)
    {
    }

    // set/get
    void setParamValue(const std::string &path, FAUSTFLOAT value)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end())
        {
            *fPathZoneMap[path] = value;
        }
        else if (fLabelZoneMap.find(path) != fLabelZoneMap.end())
        {
            *fLabelZoneMap[path] = value;
        }
    }

    FAUSTFLOAT getParamValue(const std::string &path)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end())
        {
            return *fPathZoneMap[path];
        }
        else if (fLabelZoneMap.find(path) != fLabelZoneMap.end())
        {
            return *fLabelZoneMap[path];
        }
        else
        {
            return FAUSTFLOAT(0);
        }
    }

    // map access
    std::map<std::string, FAUSTFLOAT *> &getMap() { return fPathZoneMap; }

    int getParamsCount() { return int(fPathZoneMap.size()); }

    std::string getParamAddress(int index)
    {
        std::map<std::string, FAUSTFLOAT *>::iterator it = fPathZoneMap.begin();
        while (index-- > 0 && it++ != fPathZoneMap.end())
        {
        }
        return (*it).first;
    }

    std::string getParamAddress(FAUSTFLOAT *zone)
    {
        std::map<std::string, FAUSTFLOAT *>::iterator it = fPathZoneMap.begin();
        do
        {
            if ((*it).second == zone)
                return (*it).first;
        } while (it++ != fPathZoneMap.end());
        return "";
    }

    static bool endsWith(std::string const &str, std::string const &end)
    {
        size_t l1 = str.length();
        size_t l2 = end.length();
        return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
    }
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char *key, const char *value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager
{

    virtual ~dsp_memory_manager() {}

    virtual void *allocate(size_t size) = 0;
    virtual void destroy(void *ptr) = 0;
};

/**
 * Signal processor definition.
 */

class dsp
{

public:
    dsp() {}
    virtual ~dsp() {}

    /* Return instance number of audio inputs */
    virtual int getNumInputs() = 0;

    /* Return instance number of audio outputs */
    virtual int getNumOutputs() = 0;

    /**
     * Trigger the ui_interface parameter with instance specific calls
     * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
     *
     * @param ui_interface - the user interface builder
     */
    virtual void buildUserInterface(UI *ui_interface) = 0;

    /* Returns the sample rate currently used by the instance */
    virtual int getSampleRate() = 0;

    /**
     * Global init, calls the following methods:
     * - static class 'classInit': static tables initialization
     * - 'instanceInit': constants and instance state initialization
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void init(int sample_rate) = 0;

    /**
     * Init instance state
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void instanceInit(int sample_rate) = 0;

    /**
     * Init instance constant state
     *
     * @param sample_rate - the sampling rate in Hertz
     */
    virtual void instanceConstants(int sample_rate) = 0;

    /* Init default control parameters values */
    virtual void instanceResetUserInterface() = 0;

    /* Init instance state (delay lines...) */
    virtual void instanceClear() = 0;

    /**
     * Return a clone of the instance.
     *
     * @return a copy of the instance on success, otherwise a null pointer.
     */
    virtual dsp *clone() = 0;

    /**
     * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
     *
     * @param m - the Meta* meta user
     */
    virtual void metadata(Meta *m) = 0;

    /**
     * DSP instance computation, to be called with successive in/out audio buffers.
     *
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
     * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
     *
     */
    virtual void compute(int count, FAUSTFLOAT **inputs, FAUSTFLOAT **outputs) = 0;

    /**
     * DSP instance computation: alternative method to be used by subclasses.
     *
     * @param date_usec - the timestamp in microsec given by audio driver.
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
     * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
     *
     */
    virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT **inputs, FAUSTFLOAT **outputs) { compute(count, inputs, outputs); }
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp
{

protected:
    dsp *fDSP;

public:
    decorator_dsp(dsp *dsp = nullptr) : fDSP(dsp) {}
    virtual ~decorator_dsp() { delete fDSP; }

    virtual int getNumInputs() { return fDSP->getNumInputs(); }
    virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
    virtual void buildUserInterface(UI *ui_interface) { fDSP->buildUserInterface(ui_interface); }
    virtual int getSampleRate() { return fDSP->getSampleRate(); }
    virtual void init(int sample_rate) { fDSP->init(sample_rate); }
    virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
    virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
    virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
    virtual void instanceClear() { fDSP->instanceClear(); }
    virtual decorator_dsp *clone() { return new decorator_dsp(fDSP->clone()); }
    virtual void metadata(Meta *m) { fDSP->metadata(m); }
    // Beware: subclasses usually have to overload the two 'compute' methods
    virtual void compute(int count, FAUSTFLOAT **inputs, FAUSTFLOAT **outputs) { fDSP->compute(count, inputs, outputs); }
    virtual void compute(double date_usec, int count, FAUSTFLOAT **inputs, FAUSTFLOAT **outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
};

/**
 * DSP factory class.
 */

class dsp_factory
{

protected:
    // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
    virtual ~dsp_factory() {}

public:
    virtual std::string getName() = 0;
    virtual std::string getSHAKey() = 0;
    virtual std::string getDSPCode() = 0;
    virtual std::string getCompileOptions() = 0;
    virtual std::vector<std::string> getLibraryList() = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;

    virtual dsp *createDSPInstance() = 0;

    virtual void setMemoryManager(dsp_memory_manager *manager) = 0;
    virtual dsp_memory_manager *getMemoryManager() = 0;
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
#include <xmmintrin.h>
#ifdef __SSE2__
#define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
#else
#define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
#endif
#else
#define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/

// BEGIN-FAUSTDSP

/* ------------------------------------------------------------
name: "untitled"
Code generated with Faust 2.79.4 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydspSIG0 {

private:

    int iVec0[2];
    int iRec0[2];

public:

    int getNumInputsmydspSIG0() {
        return 0;
    }
    int getNumOutputsmydspSIG0() {
        return 1;
    }

    void instanceInitmydspSIG0(int sample_rate) {
        for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
            iVec0[l0] = 0;
        }
        for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
            iRec0[l1] = 0;
        }
    }

    void fillmydspSIG0(int count, float* table) {
        for (int i1 = 0; i1 < count; i1 = i1 + 1) {
            iVec0[0] = 1;
            iRec0[0] = (iVec0[1] + iRec0[1]) % 65536;
            table[i1] = std::sin(9.58738e-05f * float(iRec0[0]));
            iVec0[1] = iVec0[0];
            iRec0[1] = iRec0[0];
        }
    }

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];
static float mydsp_faustpower3_f(float value) {
    return value * value * value;
}

class mydsp : public dsp {

private:

    FAUSTFLOAT fEntry0;
    FAUSTFLOAT fEntry1;
    int iVec1[3];
    FAUSTFLOAT fHslider0;
    int fSampleRate;
    float fConst0;
    float fConst1;
    float fRec1[2];
    float fRec3[2];
    float fVec2[2];
    float fConst2;
    float fVec3[2];
    float fConst3;
    int IOTA0;
    float fVec4[2048];
    float fRec2[2];
    float fConst4;
    float fRec4[2];
    float fVec5[2];
    float fVec6[2];

public:
    mydsp() {
    }

    void metadata(Meta* m) {
        m->declare("basics.lib/name", "Faust Basic Element Library");
        m->declare("basics.lib/version", "1.21.0");
        m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
        m->declare("filename", "untitled.dsp");
        m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/name", "Faust Filters Library");
        m->declare("filters.lib/pole:author", "Julius O. Smith III");
        m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
        m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/version", "1.7.1");
        m->declare("maths.lib/author", "GRAME");
        m->declare("maths.lib/copyright", "GRAME");
        m->declare("maths.lib/license", "LGPL with exception");
        m->declare("maths.lib/name", "Faust Math Library");
        m->declare("maths.lib/version", "2.8.1");
        m->declare("name", "untitled");
        m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
        m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
        m->declare("oscillators.lib/name", "Faust Oscillator Library");
        m->declare("oscillators.lib/sawN:author", "Julius O. Smith III");
        m->declare("oscillators.lib/sawN:license", "STK-4.3");
        m->declare("oscillators.lib/version", "1.6.0");
        m->declare("platform.lib/name", "Generic Platform Library");
        m->declare("platform.lib/version", "1.3.0");
    }

    virtual int getNumInputs() {
        return 0;
    }
    virtual int getNumOutputs() {
        return 1;
    }

    static void classInit(int sample_rate) {
        mydspSIG0* sig0 = newmydspSIG0();
        sig0->instanceInitmydspSIG0(sample_rate);
        sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
        deletemydspSIG0(sig0);
    }

    virtual void instanceConstants(int sample_rate) {
        fSampleRate = sample_rate;
        fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
        fConst1 = 1.0f / fConst0;
        fConst2 = 0.5f * fConst0;
        fConst3 = 0.083333336f * fConst0;
        fConst4 = 4.0f / fConst0;
    }

    virtual void instanceResetUserInterface() {
        fEntry0 = FAUSTFLOAT(0.0f);
        fEntry1 = FAUSTFLOAT(0.0f);
        fHslider0 = FAUSTFLOAT(5e+02f);
    }

    virtual void instanceClear() {
        for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
            iVec1[l2] = 0;
        }
        for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
            fRec1[l3] = 0.0f;
        }
        for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
            fRec3[l4] = 0.0f;
        }
        for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
            fVec2[l5] = 0.0f;
        }
        for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
            fVec3[l6] = 0.0f;
        }
        IOTA0 = 0;
        for (int l7 = 0; l7 < 2048; l7 = l7 + 1) {
            fVec4[l7] = 0.0f;
        }
        for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
            fRec2[l8] = 0.0f;
        }
        for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
            fRec4[l9] = 0.0f;
        }
        for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
            fVec5[l10] = 0.0f;
        }
        for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
            fVec6[l11] = 0.0f;
        }
    }

    virtual void init(int sample_rate) {
        classInit(sample_rate);
        instanceInit(sample_rate);
    }

    virtual void instanceInit(int sample_rate) {
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }

    virtual mydsp* clone() {
        return new mydsp();
    }

    virtual int getSampleRate() {
        return fSampleRate;
    }

    virtual void buildUserInterface(UI* ui_interface) {
        ui_interface->openVerticalBox("untitled");
        ui_interface->addHorizontalSlider("hz", &fHslider0, FAUSTFLOAT(5e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+04f), FAUSTFLOAT(0.01f));
        ui_interface->addNumEntry("wave_select_1", &fEntry0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
        ui_interface->addNumEntry("wave_select_2", &fEntry1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
        ui_interface->closeBox();
    }

    virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
        FAUSTFLOAT* output0 = outputs[0];
        int iSlow0 = int(float(fEntry0));
        int iSlow1 = int(float(fEntry1));
        float fSlow2 = float(fHslider0);
        float fSlow3 = fConst1 * fSlow2;
        float fSlow4 = std::max<float>(fSlow2, 23.44895f);
        float fSlow5 = std::max<float>(2e+01f, std::fabs(fSlow4));
        float fSlow6 = fConst1 * fSlow5;
        float fSlow7 = fConst2 / fSlow5;
        float fSlow8 = fConst3 / fSlow5;
        float fSlow9 = std::max<float>(0.0f, std::min<float>(2047.0f, fConst2 / fSlow4));
        int iSlow10 = int(fSlow9);
        int iSlow11 = iSlow10 + 1;
        float fSlow12 = std::floor(fSlow9);
        float fSlow13 = fSlow9 - fSlow12;
        float fSlow14 = fSlow12 + (1.0f - fSlow9);
        float fSlow15 = fConst4 * fSlow2;
        float fSlow16 = std::max<float>(2e+01f, std::fabs(fSlow2));
        float fSlow17 = fConst1 * fSlow16;
        float fSlow18 = fConst2 / fSlow16;
        float fSlow19 = fConst3 / fSlow16;
        for (int i0 = 0; i0 < count; i0 = i0 + 1) {
            iVec1[0] = 1;
            int iTemp0 = 1 - iVec1[1];
            float fTemp1 = ((iTemp0) ? 0.0f : fSlow3 + fRec1[1]);
            fRec1[0] = fTemp1 - std::floor(fTemp1);
            float fTemp2 = ((iTemp0) ? 0.0f : fSlow6 + fRec3[1]);
            fRec3[0] = fTemp2 - std::floor(fTemp2);
            float fTemp3 = 2.0f * fRec3[0];
            float fTemp4 = mydsp_faustpower3_f(fTemp3 + -1.0f);
            fVec2[0] = fTemp4 + (1.0f - fTemp3);
            float fTemp5 = fSlow7 * (fTemp4 + (1.0f - (fTemp3 + fVec2[1])));
            fVec3[0] = fTemp5;
            float fTemp6 = float(iVec1[2]);
            float fTemp7 = fSlow8 * fTemp6 * (fTemp5 - fVec3[1]);
            fVec4[IOTA0 & 2047] = fTemp7;
            float fTemp8 = fSlow14 * fVec4[(IOTA0 - iSlow10) & 2047] + fSlow13 * fVec4[(IOTA0 - iSlow11) & 2047];
            fRec2[0] = 0.999f * fRec2[1] + fTemp7 - fTemp8;
            float fTemp9 = ((iTemp0) ? 0.0f : fSlow17 + fRec4[1]);
            fRec4[0] = fTemp9 - std::floor(fTemp9);
            float fTemp10 = 2.0f * fRec4[0];
            float fTemp11 = mydsp_faustpower3_f(fTemp10 + -1.0f);
            fVec5[0] = fTemp11 + (1.0f - fTemp10);
            float fTemp12 = fSlow18 * (fTemp11 + (1.0f - (fTemp10 + fVec5[1])));
            fVec6[0] = fTemp12;
            output0[i0] = FAUSTFLOAT(((iSlow0) ? ((iSlow1) ? fTemp7 - fTemp8 : fSlow19 * fTemp6 * (fTemp12 - fVec6[1])) : ((iSlow1) ? fSlow15 * fRec2[0] : ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[0]), 65535))])));
            iVec1[2] = iVec1[1];
            iVec1[1] = iVec1[0];
            fRec1[1] = fRec1[0];
            fRec3[1] = fRec3[0];
            fVec2[1] = fVec2[0];
            fVec3[1] = fVec3[0];
            IOTA0 = IOTA0 + 1;
            fRec2[1] = fRec2[0];
            fRec4[1] = fRec4[0];
            fVec5[1] = fVec5[0];
            fVec6[1] = fVec6[0];
        }
    }

};


// END-FAUSTDSP

#endif