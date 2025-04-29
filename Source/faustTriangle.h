/*
  ==============================================================================

    faultTriangle.h
    Created: 28 Apr 2025 1:25:10pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "faustHeader.h"


// BEGIN-FAUSTDSP

/* ------------------------------------------------------------
name: "untitled"
Code generated with Faust 2.79.4 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __triangledsp_H__
#define  __triangledsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS triangledsp
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

class triangledspSIG0 {

private:

	int iVec0[2];
	int iRec0[2];

public:

	int getNumInputstriangledspSIG0() {
		return 0;
	}
	int getNumOutputstriangledspSIG0() {
		return 1;
	}

	void instanceInittriangledspSIG0(int sample_rate) {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec0[l1] = 0;
		}
	}

	void filltriangledspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec0[0] = (iVec0[1] + iRec0[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec0[0]));
			iVec0[1] = iVec0[0];
			iRec0[1] = iRec0[0];
		}
	}

};

static triangledspSIG0* newtriangledspSIG0() { return (triangledspSIG0*)new triangledspSIG0(); }
static void deletetriangledspSIG0(triangledspSIG0* dsp) { delete dsp; }

static float ftbl0triangledspSIG0[65536];

class triangledsp : public dsp {

private:

	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	int iVec1[2];
	float fConst2;
	float fRec1[2];
	float fConst3;
	float fRec2[2];
	float fConst4;
	float fRec3[2];
	float fConst5;
	float fRec4[2];
	float fConst6;
	float fRec5[2];

public:
	triangledsp() {
	}

	void metadata(Meta* m) {
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "untitled.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "untitled");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
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
		triangledspSIG0* sig0 = newtriangledspSIG0();
		sig0->instanceInittriangledspSIG0(sample_rate);
		sig0->filltriangledspSIG0(65536, ftbl0triangledspSIG0);
		deletetriangledspSIG0(sig0);
	}

	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 0.5f * fConst0;
		fConst2 = 7.0f / fConst0;
		fConst3 = 3.0f / fConst0;
		fConst4 = 9.0f / fConst0;
		fConst5 = 5.0f / fConst0;
		fConst6 = 1.0f / fConst0;
	}

	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(933.0f);
	}

	virtual void instanceClear() {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec1[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec4[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec5[l7] = 0.0f;
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

	virtual triangledsp* clone() {
		return new triangledsp();
	}

	virtual int getSampleRate() {
		return fSampleRate;
	}

	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("untitled");
		ui_interface->addHorizontalSlider("hz", &fHslider0, FAUSTFLOAT(933.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(1e-05f));
		ui_interface->closeBox();
	}

	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		int iSlow1 = (7.0f * fSlow0) > fConst1;
		float fSlow2 = fConst2 * fSlow0;
		int iSlow3 = (3.0f * fSlow0) > fConst1;
		float fSlow4 = fConst3 * fSlow0;
		int iSlow5 = (9.0f * fSlow0) > fConst1;
		float fSlow6 = fConst4 * fSlow0;
		int iSlow7 = (5.0f * fSlow0) > fConst1;
		float fSlow8 = fConst5 * fSlow0;
		float fSlow9 = fConst6 * fSlow0;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec1[0] = 1;
			int iTemp0 = 1 - iVec1[1];
			float fTemp1 = ((iTemp0) ? 0.0f : fSlow2 + fRec1[1]);
			fRec1[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = ((iTemp0) ? 0.0f : fSlow4 + fRec2[1]);
			fRec2[0] = fTemp2 - std::floor(fTemp2);
			float fTemp3 = ((iTemp0) ? 0.0f : fSlow6 + fRec3[1]);
			fRec3[0] = fTemp3 - std::floor(fTemp3);
			float fTemp4 = ((iTemp0) ? 0.0f : fSlow8 + fRec4[1]);
			fRec4[0] = fTemp4 - std::floor(fTemp4);
			float fTemp5 = ((iTemp0) ? 0.0f : fSlow9 + fRec5[1]);
			fRec5[0] = fTemp5 - std::floor(fTemp5);
			output0[i0] = FAUSTFLOAT(0.81056947f * (ftbl0triangledspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec5[0]), 65535))] + ((iSlow7) ? 0.0f : 0.04f * ftbl0triangledspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec4[0]), 65535))]) + ((iSlow5) ? 0.0f : 0.012345679f * ftbl0triangledspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec3[0]), 65535))]) - (((iSlow3) ? 0.0f : 0.11111111f * ftbl0triangledspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec2[0]), 65535))]) + ((iSlow1) ? 0.0f : 0.020408163f * ftbl0triangledspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec1[0]), 65535))]))));
			iVec1[1] = iVec1[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
		}
	}

};

#endif


// END-FAUSTDSP