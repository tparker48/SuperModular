/*
  ==============================================================================

    faustSquare.h
    Created: 28 Apr 2025 1:25:19pm
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

#ifndef  __squaredsp_H__
#define  __squaredsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS squaredsp
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

static float squaredsp_faustpower3_f(float value) {
	return value * value * value;
}
static float squaredsp_faustpower2_f(float value) {
	return value * value;
}

class squaredsp : public dsp {

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
	int IOTA0;
	float fVec1[4096];
	float fVec2[2];
	float fVec3[2];
	float fVec4[4096];
	float fConst4;
	float fConst5;

public:
	squaredsp() {
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
		fConst4 = 0.5f * fConst0;
		fConst5 = 0.041666668f * squaredsp_faustpower2_f(fConst0);
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
		IOTA0 = 0;
		for (int l3 = 0; l3 < 4096; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fVec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4096; l6 = l6 + 1) {
			fVec4[l6] = 0.0f;
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

	virtual squaredsp* clone() {
		return new squaredsp();
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
			float fTemp0 = std::max<float>(fRec0[0], 23.44895f);
			float fTemp1 = std::max<float>(2e+01f, std::fabs(fTemp0));
			float fTemp2 = ((1 - iVec0[1]) ? 0.0f : fRec1[1] + fConst3 * fTemp1);
			fRec1[0] = fTemp2 - std::floor(fTemp2);
			float fTemp3 = 2.0f * fRec1[0];
			float fTemp4 = fTemp3 + -1.0f;
			fVec1[IOTA0 & 4095] = fTemp4;
			float fTemp5 = squaredsp_faustpower3_f(fTemp4);
			fVec2[0] = fTemp5 + (1.0f - fTemp3);
			float fTemp6 = (fTemp5 + (1.0f - (fTemp3 + fVec2[1]))) / fTemp1;
			fVec3[0] = fTemp6;
			float fTemp7 = float(iVec0[2]) * (fTemp6 - fVec3[1]) / fTemp1;
			fVec4[IOTA0 & 4095] = fTemp7;
			float fTemp8 = std::max<float>(0.0f, std::min<float>(2047.0f, fConst4 / fTemp0));
			int iTemp9 = int(fTemp8);
			int iTemp10 = iTemp9 + 1;
			float fTemp11 = std::floor(fTemp8);
			float fTemp12 = fTemp8 - fTemp11;
			float fTemp13 = fTemp11 + (1.0f - fTemp8);
			output0[i0] = FAUSTFLOAT(((fRec0[0] < 3e+01f) ? fTemp3 + (-1.0f - (fTemp13 * fVec1[(IOTA0 - iTemp9) & 4095] + fTemp12 * fVec1[(IOTA0 - iTemp10) & 4095])) : fConst5 * (fTemp7 - fVec4[(IOTA0 - iTemp9) & 4095] * fTemp13 - fTemp12 * fVec4[(IOTA0 - iTemp10) & 4095])));
			iVec0[2] = iVec0[1];
			iVec0[1] = iVec0[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			IOTA0 = IOTA0 + 1;
			fVec2[1] = fVec2[0];
			fVec3[1] = fVec3[0];
		}
	}

};

#endif



// END-FAUSTDSP