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
        ModuleUI(id, mg, cm, stateWriter, 1, 1) {
        hzIn = getCvInputJack(0);
        waveOut = getCvOutputJack(0);

        hzSlider.setRange(24.0, 12000.0);
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

        // sin, tri, saw, sq
        waveType.setValue(0);
        waveType.setRange(0, 3, 1);
        waveType.setTitle("waveSelect");
        waveType.setName("waveSelect");
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
        if (slider == &hzSlider) {
            stateWriter->setModuleProperty(getId(), "hz", hzSlider.getValue());
        }
        if (slider == &waveType) {
            stateWriter->setModuleProperty(getId(), "wave", waveType.getValue());
        }
        if (slider == &fmSlider) {
            stateWriter->setModuleProperty(getId(), "fmAmt", fmSlider.getValue());
        }
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
        auto lfoState = moduleState.state.getProperty("lfo_toggle");
        if (!lfoState.isVoid()) {
            lfoToggle.setToggleState((bool)lfoState, false);
        }   
    }

    void paintModule(Graphics& g) override {
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, &hzSlider, "hz", TOP, getWidth()*0.05, textCol);
        paintComponentLabel(g, &lfoToggle, "lfo", TOP, getWidth()*0.05, textCol);
        paintComponentLabel(g, hzIn, "hz", TOP, getWidth()*0.05, textCol);
        paintComponentLabel(g, waveOut, "out", TOP, getWidth()*0.05, textCol);

        g.setFont(14.0);
        auto stretch = 0.48;

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
        g.drawText("sq", bounds, Justification::centred);
    }

    void resized() override {
        auto margin = getWidth() * 0.15;
        auto hzY = getHeight() * 0.1;
        auto cvY = getHeight() * 0.94;
        auto middleX = getWidth() * 0.5;
        auto paddingY = getHeight() * 0.12;

        hzIn->setCentrePosition(margin, cvY);
        waveOut->setCentrePosition(getWidth() - margin, cvY);

        hzSlider.setSize(50, 50);
        hzSlider.setCentrePosition(getWidth() * .3, getHeight() * .20);

        lfoToggle.setSize(40, 40);
        lfoToggle.setCentrePosition(getWidth() * .7, getHeight() * .2);

        waveType.setSize(45, 45);
        waveType.setCentrePosition(getWidth() * .5, getHeight() * .45);

        fmSlider.setSize(30, 30);
        fmSlider.setCentrePosition(margin + 30, cvY);
    }

private:
    CVJackComponent* hzIn, * waveOut;
    Dial hzSlider, fmSlider;
    Dial waveType;
    Toggle lfoToggle;

    // Colour bgCol = Colour(0xFF84A98C);
    // Colour textCol = Colour(0xFFF8F4F9);
    // Colour knobCol = Colour(0xFF1C3738);
    // Colour dotCol = Colour(0xFFF8F4F9);
    // Colour cvCol = Colour(0xFF1C3738);
};