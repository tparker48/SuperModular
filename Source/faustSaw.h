/*
  ==============================================================================

    faustSaw.h
    Created: 28 Apr 2025 1:25:28pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "faustHeader.h"
using namespace faust;

// BEGIN-FAUSTDSP

/* ------------------------------------------------------------
name: "untitled"
Code generated with Faust 2.79.4 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __sawdsp_H__
#define  __sawdsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS sawdsp
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

static float sawdsp_faustpower3_f(float value) {
	return value * value * value;
}
static float sawdsp_faustpower2_f(float value) {
	return value * value;
}

class sawdsp : public dsp {

private:

	int iVec0[3];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec0[2];
	float fConst3;
	float fRec1[2];
	float fVec1[2];
	float fVec2[2];
	float fConst4;

public:
	sawdsp() {
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
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/sawN:author", "Julius O. Smith III");
		m->declare("oscillators.lib/sawN:license", "STK-4.3");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}

	static void classInit(int sample_rate) {
	}

	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = std::exp(-(1e+03f / fConst0));
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		fConst4 = 0.041666668f * sawdsp_faustpower2_f(fConst0);
	}

	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(24.0f);
	}

	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
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

	virtual sawdsp* clone() {
		return new sawdsp();
	}

	virtual int getSampleRate() {
		return fSampleRate;
	}

	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("untitled");
		ui_interface->addHorizontalSlider("hz", &fHslider0, FAUSTFLOAT(24.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(1e-05f));
		ui_interface->closeBox();
	}

	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = fConst2 * float(fHslider0);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			fRec0[0] = fSlow0 + fConst1 * fRec0[1];
			float fTemp0 = std::max<float>(2e+01f, std::fabs(fRec0[0]));
			float fTemp1 = ((1 - iVec0[1]) ? 0.0f : fRec1[1] + fConst3 * fTemp0);
			fRec1[0] = fTemp1 - std::floor(fTemp1);
			float fTemp2 = 2.0f * fRec1[0];
			float fTemp3 = fTemp2 + -1.0f;
			float fTemp4 = sawdsp_faustpower3_f(fTemp3);
			fVec1[0] = fTemp4 + (1.0f - fTemp2);
			float fTemp5 = (fTemp4 + (1.0f - (fTemp2 + fVec1[1]))) / fTemp0;
			fVec2[0] = fTemp5;
			output0[i0] = FAUSTFLOAT(((fRec0[0] < 3e+01f) ? fTemp3 : fConst4 * (float(iVec0[2]) * (fTemp5 - fVec2[1]) / fTemp0)));
			iVec0[2] = iVec0[1];
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
		}
	}

};

#endif


// END-FAUSTDSP