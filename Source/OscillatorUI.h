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


class OscillatorUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    OscillatorUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 2, 1) {
        hzIn = getCvInputJack(0);
        ampIn = getCvInputJack(1);
        waveOut = getCvOutputJack(0);

        hzSlider.setValue(220.0);
        hzSlider.setRange(24.0, 14000.0);
        hzSlider.setSkewFactor(0.3);
        hzSlider.setTitle("hz");
        hzSlider.setName("hz");
        addAndMakeVisible(hzSlider);

        // sin, tri, saw, sq
        waveType.setValue(0);
        waveType.setRange(0, 3, 1);
        waveType.setTitle("waveSelect");
        waveType.setName("waveSelect");
        addAndMakeVisible(waveType);
    }

    void sliderValueChanged(juce::Slider* slider) override {
        if (slider == &hzSlider) {
            stateWriter->setModuleProperty(getId(), "hz", hzSlider.getValue());
        }
        if (slider == &waveType) {
            stateWriter->setModuleProperty(getId(), "wave", waveType.getValue());
        }
    }

    void startListeners() override {
        hzSlider.addListener(this);
        waveType.addListener(this);
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
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colour(255, 249, 235));
        g.fillAll();
        g.setFont(16.0);

        g.setColour(Colours::black);
        auto bounds = hzSlider.getBounds();
        bounds.setY(bounds.getY() - (bounds.getHeight()));
        g.drawText("hz", bounds, Justification::centredBottom);

        bounds = hzIn->getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth() * 2); 
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("hz", bounds, Justification::centredBottom);

        bounds = ampIn->getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth()*2); 
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("amp", bounds, Justification::centredBottom);

        bounds = waveOut->getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth()*2); 
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("out", bounds, Justification::centredBottom);

        g.setFont(14.0);
        auto stretch = 0.48;

        bounds = waveType.getBounds();
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
        auto margin = getWidth() * 0.10;
        auto hzY = getHeight() * 0.1;
        auto cvY = getHeight() * 0.9;
        auto middleX = getWidth() * 0.5;
        auto paddingY = getHeight() * 0.15;

        hzIn->setBounds(margin, cvY - paddingY, 25, 25);
        ampIn->setBounds(margin, cvY, 25, 25);
        waveOut->setBounds(getWidth() - margin - 25, cvY, 25, 25);


        hzSlider.setSize(50, 50);
        hzSlider.setCentrePosition(getWidth() * .3, getHeight() * .25);

        waveType.setSize(50, 50);
        waveType.setCentrePosition(getWidth() * .5, getHeight()  * .50);
    }

private:
    CVJackComponent* hzIn, *ampIn, *waveOut;
    Dial hzSlider;
    Dial waveType;
};