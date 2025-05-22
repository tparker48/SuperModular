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
        ModuleUI(id, mg, cm, stateWriter, 2, 4) {
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
        dutyCycleSlider.setValue(0.50);
        dutyCycleSlider.setTitle("dutyCycle");
        dutyCycleSlider.setName("dutyCycle");
        dutyCycleSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(dutyCycleSlider);

        dutyCycleAmtSlider.setRange(0.0, 0.5);
        dutyCycleAmtSlider.setValue(0.25);
        dutyCycleAmtSlider.setTitle("dutyCycleAmt");
        dutyCycleAmtSlider.setName("dutyCycleAmt");
        dutyCycleAmtSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(dutyCycleAmtSlider);

        lfoToggle.setToggleState(false, false);
        addAndMakeVisible(lfoToggle);

        hzSlider.setColours(knobCol, dotCol);
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
        auto sectionH = (float)getHeight() * 0.333;
        g.setColour(Colours::white);
        fillRect(g,0.0f, (float)getHeight()-sectionH, (float)getWidth(), sectionH);

        g.setColour(Colours::black);
        drawRect(g,0.0f, (float)getHeight()-sectionH, (float)getWidth(), sectionH, 2.0f);

        paintComponentLabel(g, &hzSlider, "hz", TOP, getWidth() * 0.05, textCol, true, getWidth()*0.3);
        paintComponentLabel(g, &lfoToggle, "lfo", TOP, getWidth() * 0.05, textCol, true, getWidth() * 0.3);
        paintComponentLabel(g, &dutyCycleSlider, "pulse width", TOP, getWidth() * 0.05, textCol, true, getWidth() * 0.65);
        paintComponentLabel(g, getCvInputJack(0), "pwm", TOP, getWidth() * 0.08, textCol);
        paintComponentLabel(g, getCvInputJack(1), "hz", TOP, getWidth() * 0.08, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "sin", LEFT, getWidth() * 0.1, textCol);
        paintComponentLabel(g, getCvOutputJack(1), "tri", LEFT, getWidth() * 0.1, textCol);
        paintComponentLabel(g, getCvOutputJack(2), "saw", LEFT, getWidth() * 0.1, textCol);
        paintComponentLabel(g, getCvOutputJack(3), "sq", LEFT, getWidth() * 0.1, textCol);
    }

    void resizeModule() override {
        auto margin = getWidth() * 0.15;
        auto hzY = getHeight() * 0.1;
        auto cvY = getHeight() * 0.94;
        auto middleX = getWidth() * 0.5;
        auto cvInPaddingY = getHeight() * 0.15;
        auto cvOutPaddingY = getHeight() * 0.07;

        getCvInputJack(0)->setCentrePosition(margin, cvY - cvInPaddingY);
        getCvInputJack(1)->setCentrePosition(margin, cvY);
        getCvOutputJack(3)->setCentrePosition(getWidth() - margin, cvY);
        getCvOutputJack(2)->setCentrePosition(getWidth() - margin, cvY - cvOutPaddingY);
        getCvOutputJack(1)->setCentrePosition(getWidth() - margin, cvY - 2*cvOutPaddingY);
        getCvOutputJack(0)->setCentrePosition(getWidth() - margin, cvY - 3*cvOutPaddingY);

        hzSlider.setSize(40, 40);
        hzSlider.setCentrePosition(getWidth() * .3, getHeight() * .25);

        lfoToggle.setSize(40, 40);
        lfoToggle.setCentrePosition(getWidth() * .7, getHeight() * .25);

        dutyCycleSlider.setSize(40, 40);
        dutyCycleSlider.setCentrePosition(getWidth() * 0.5, getHeight() * 0.5);

        fmSlider.setSize(30, 30);
        fmSlider.setCentrePosition(getWidth()*0.38, cvY);

        dutyCycleAmtSlider.setSize(30, 30);
        dutyCycleAmtSlider.setCentrePosition(getWidth()*0.38, cvY - cvInPaddingY);
    }

    std::string getName() override {
        return "Oscillator";
    }

private:
    Dial hzSlider, fmSlider, dutyCycleSlider, dutyCycleAmtSlider;
    Toggle lfoToggle;
    Colour textCol = Colours::black;
};