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
        hzIn = getCvInputJack(0);
        ampIn = getCvInputJack(1);
        waveOut = getCvOutputJack(0);

        hzSlider.setRange(24.0, 12000.0);
        hzSlider.setValue(220.0);
        hzSlider.setSkewFactor(.2);
        hzSlider.setTitle("hz");
        hzSlider.setName("hz");
        addAndMakeVisible(hzSlider);

        gainSlider.setRange(0.001, 1.0);
        gainSlider.setValue(1.0);
        gainSlider.setSkewFactor(.75);
        gainSlider.setTitle("gain");
        gainSlider.setName("gain");
        addAndMakeVisible(gainSlider);

        fmSlider.setRange(0.0, 1.0);
        fmSlider.setValue(0.5);
        fmSlider.setTitle("fmAmt");
        fmSlider.setName("fmAmt");
        fmSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(fmSlider);

        amSlider.setRange(0.0, 1.0);
        amSlider.setValue(0.5);
        amSlider.setTitle("amAmt");
        amSlider.setName("amAmt");
        amSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(amSlider);

        // sin, tri, saw, sq
        waveType.setValue(0);
        waveType.setRange(0, 3, 1);
        waveType.setTitle("waveSelect");
        waveType.setName("waveSelect");
        addAndMakeVisible(waveType);

        lfoToggle.setToggleState(false, false);
        addAndMakeVisible(lfoToggle);

        hzSlider.setColours(knobCol, dotCol);
        gainSlider.setColours(knobCol, dotCol);
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
        if (slider == &gainSlider) {
            stateWriter->setModuleProperty(getId(), "gain", gainSlider.getValue());
        }
        if (slider == &waveType) {
            stateWriter->setModuleProperty(getId(), "wave", waveType.getValue());
        }
        if (slider == &fmSlider) {
            stateWriter->setModuleProperty(getId(), "fmAmt", fmSlider.getValue());
        }
        if (slider == &amSlider) {
            stateWriter->setModuleProperty(getId(), "amAmt", amSlider.getValue());
        }
    }

    void buttonClicked(juce::Button* button) override {
        if (button == &lfoToggle) {
            stateWriter->setModuleProperty(getId(), "lfo_toggle", (int)lfoToggle.getToggleState());
        }
    }

    void startListeners() override {
        hzSlider.addListener(this);
        gainSlider.addListener(this);
        waveType.addListener(this);
        lfoToggle.addListener(this);
        fmSlider.addListener(this);
        amSlider.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
        auto hz = moduleState.state.getProperty("hz");
        if (!hz.isVoid()) {
            hzSlider.setValue(hz);
        }
        auto gain = moduleState.state.getProperty("gain");
        if (!gain.isVoid()) {
            gainSlider.setValue(gain);
        }
        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType.setValue(wave);
        }
        auto fmAmt = moduleState.state.getProperty("fmAmt");
        if (!fmAmt.isVoid()) {
            fmSlider.setValue(fmAmt);
        }
        auto amAmt = moduleState.state.getProperty("amAmt");
        if (!amAmt.isVoid()) {
            amSlider.setValue(amAmt);
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
        paintComponentLabel(g, &gainSlider, "gain", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, &lfoToggle, "lfo", TOP, getWidth()*0.05, textCol);
        paintComponentLabel(g, hzIn, "hz", TOP, getWidth()*0.05, textCol);
        paintComponentLabel(g, ampIn, "amp", TOP, getWidth()*0.05, textCol);
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

        hzIn->setCentrePosition(margin, cvY - paddingY);
        ampIn->setCentrePosition(margin, cvY);
        waveOut->setCentrePosition(getWidth() - margin, cvY);

        hzSlider.setSize(40, 40);
        hzSlider.setCentrePosition(getWidth() * .3, getHeight() * .20);

        gainSlider.setSize(40, 40);
        gainSlider.setCentrePosition(getWidth() * .7, getHeight() * .2);

        lfoToggle.setSize(40, 40);
        lfoToggle.setCentrePosition(getWidth() * .5, getHeight() * .40);

        waveType.setSize(40, 40);
        waveType.setCentrePosition(getWidth() * .5, getHeight() * .60);

        fmSlider.setSize(30, 30);
        fmSlider.setCentrePosition(margin + 30, cvY - paddingY);

        amSlider.setSize(30, 30);
        amSlider.setCentrePosition(margin + 30, cvY);
    }

private:
    CVJackComponent* hzIn, * ampIn, * waveOut;
    Dial hzSlider, gainSlider, fmSlider, amSlider;
    Dial waveType;
    Toggle lfoToggle;
    /*
    Colour bgCol = Colour(0xFFBAD9B5);
    Colour textCol = Colour(0xFF393424);
    Colour knobCol = Colour(0xFF393424);
    Colour dotCol = Colour(0xFFEFF7CF);
    Colour cvCol = Colour(0xFF393424);
   */
    Colour bgCol = Colour(0xFF84A98C);
    Colour textCol = Colour(0xFFF8F4F9);
    Colour knobCol = Colour(0xFF1C3738);
    Colour dotCol = Colour(0xFFF8F4F9);
    Colour cvCol = Colour(0xFF1C3738);
};