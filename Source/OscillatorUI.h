/*
  ==============================================================================

    OscillatorUI.h
    Created: 16 Apr 2025 11:02:35pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"
#include "Toggle.h"


class OscillatorUI : public ModuleUI, public Slider::Listener, Button::Listener {
public:
    static const int hp = 2;

    OscillatorUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 2, 1) {
        hzSlider.setRange(24.0, 6000.0);
        hzSlider.setValue(220.0);
        hzSlider.setSkewFactor(.2);
        hzSlider.setTitle("hz");
        hzSlider.setName("hz");
        addAndMakeVisible(hzSlider);

        fmSlider.setRange(0.0, 1.0);
        fmSlider.setValue(0.5);
        fmSlider.setTitle("fmAmt");
        fmSlider.setName("fmAmt");
        fmSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(fmSlider);

        dutyCycleSlider.setRange(0.25, 0.75);
        dutyCycleSlider.setValue(0.5);
        dutyCycleSlider.setTitle("dutyCycle");
        dutyCycleSlider.setName("dutyCycle");
        dutyCycleSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(dutyCycleSlider);

        dutyCycleAmtSlider.setRange(0.0, 0.5);
        dutyCycleAmtSlider.setValue(0.5);
        dutyCycleAmtSlider.setTitle("dutyCycleAmt");
        dutyCycleAmtSlider.setName("dutyCycleAmt");
        dutyCycleAmtSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(dutyCycleAmtSlider);

        // sin, tri, saw, sq
        waveType.setValue(0);
        waveType.setRange(0, 3, 1);
        waveType.setTitle("wave");
        waveType.setName("wave");
        addAndMakeVisible(waveType);

        lfoToggle.setToggleState(false, false);
        addAndMakeVisible(lfoToggle);

        hzSlider.setColours(knobCol, dotCol);
        waveType.setColours(knobCol, dotCol);
        lfoToggle.setColours(knobCol, dotCol);

        for (int i = 0; i < getNumCVInputs(); i++) {
            getCvInputJack(i)->setJackColour(cvCol);
        }
        for (int i = 0; i < getNumCVOutputs(); i++) {
            getCvOutputJack(i)->setJackColour(cvCol);
        }
    }

    void sliderValueChanged(juce::Slider* slider) override {
        stateWriter->setModuleProperty(getId(), slider->getName(), slider->getValue());
    }

    void buttonClicked(juce::Button* button) override {
        if (button == &lfoToggle) {
            stateWriter->setModuleProperty(getId(), "lfo_toggle", (int)lfoToggle.getToggleState());
        }
    }

    void startListeners() override {
        hzSlider.addListener(this);
        waveType.addListener(this);
        lfoToggle.addListener(this);
        fmSlider.addListener(this);
        dutyCycleSlider.addListener(this);
        dutyCycleAmtSlider.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
        auto hz = moduleState.state.getProperty("hz");
        if (!hz.isVoid()) {
            hzSlider.setValue(hz);
        }
        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType.setValue(wave);
        }
        auto fmAmt = moduleState.state.getProperty("fmAmt");
        if (!fmAmt.isVoid()) {
            fmSlider.setValue(fmAmt);
        }
        auto dutyCycle = moduleState.state.getProperty("dutyCycle");
        if (!dutyCycle.isVoid()) {
            dutyCycleSlider.setValue(dutyCycle);
        }
        auto dutyCycleAmt = moduleState.state.getProperty("dutyCycleAmt");
        if (!dutyCycleAmt.isVoid()) {
            dutyCycleAmtSlider.setValue(dutyCycle);
        }
        auto lfoState = moduleState.state.getProperty("lfo_toggle");
        if (!lfoState.isVoid()) {
            lfoToggle.setToggleState((bool)lfoState, false);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, &hzSlider, "hz", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, &lfoToggle, "lfo", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, &dutyCycleSlider, "pulse width", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, getCvInputJack(0), "pwm", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, getCvInputJack(1), "hz", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getWidth() * 0.05, textCol);

        g.setFont(14.0);
        auto stretch = 0.55;

        auto bounds = waveType.getBounds();
        bounds.setCentre(bounds.getCentreX() - bounds.getWidth() * stretch, bounds.getCentreY() + bounds.getHeight() * stretch);
        g.drawText("sin", bounds, Justification::centred);

        bounds = waveType.getBounds();
        bounds.setCentre(bounds.getCentreX() - bounds.getWidth() * stretch, bounds.getCentreY() - bounds.getHeight() * stretch);
        g.drawText("tri", bounds, Justification::centred);

        bounds = waveType.getBounds();
        bounds.setCentre(bounds.getCentreX() + bounds.getWidth() * stretch, bounds.getCentreY() - bounds.getHeight() * stretch);
        g.drawText("saw", bounds, Justification::centred);

        bounds = waveType.getBounds();
        bounds.setCentre(bounds.getCentreX() + bounds.getWidth() * stretch, bounds.getCentreY() + bounds.getHeight() * stretch);
        g.drawText("pulse", bounds, Justification::centred);
    }

    void resized() override {
        auto margin = getWidth() * 0.15;
        auto hzY = getHeight() * 0.1;
        auto cvY = getHeight() * 0.94;
        auto middleX = getWidth() * 0.5;
        auto paddingY = getHeight() * 0.15;

        getCvInputJack(0)->setCentrePosition(margin, cvY - paddingY);
        getCvInputJack(1)->setCentrePosition(margin, cvY);
        getCvOutputJack(0)->setCentrePosition(getWidth() - margin, cvY);

        hzSlider.setSize(40, 40);
        hzSlider.setCentrePosition(getWidth() * .3, getHeight() * .15);

        lfoToggle.setSize(40, 40);
        lfoToggle.setCentrePosition(getWidth() * .7, getHeight() * .15);

        waveType.setSize(35, 35);
        waveType.setCentrePosition(getWidth() * .5, getHeight() * .35);

        dutyCycleSlider.setSize(35, 35);
        dutyCycleSlider.setCentrePosition(getWidth() * 0.5, getHeight() * 0.62);

        fmSlider.setSize(30, 30);
        fmSlider.setCentrePosition(margin + 30, cvY);

        dutyCycleAmtSlider.setSize(30, 30);
        dutyCycleAmtSlider.setCentrePosition(margin + 30, cvY - paddingY);
    }

private:
    Dial hzSlider, fmSlider, dutyCycleSlider, dutyCycleAmtSlider;
    Dial waveType;
    Toggle lfoToggle;
};