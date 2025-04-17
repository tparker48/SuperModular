/*
  ==============================================================================

    OscillatorUI.h
    Created: 16 Apr 2025 11:02:35pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"


class OscillatorUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    OscillatorUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 2, 1) {
        hzIn = getCvInputJack(0);
        ampIn = getCvInputJack(1);
        waveOut = getCvOutputJack(0);

        hzSlider.setSliderStyle(Slider::RotaryHorizontalDrag);
        hzSlider.setValue(0.8);
        hzSlider.setRange(0.0, 1.0);
        hzSlider.setTitle("hz");
        hzSlider.setName("hz");
        hzSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        addAndMakeVisible(hzSlider);
    }

    void startListeners() {
        hzSlider.addListener(this);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &hzSlider) {
            stateWriter->setModuleProperty(getId(), "hz", hzSlider.getValue());
        }
    }

    void paint(Graphics& g) override {
        g.setColour(Colours::lightgoldenrodyellow);
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
        g.drawText("wave", bounds, Justification::centredBottom);

    }

    void resized() override {
        auto margin = getWidth() * 0.10;
        auto gainY = getHeight() * 0.2;
        auto cvY = getHeight() * 0.8;
        auto middleX = getWidth() * 0.5;
        auto paddingY = getHeight() * 0.2;

        hzIn->setBounds(margin, cvY - paddingY, 25, 25);
        ampIn->setBounds(margin, cvY, 25, 25);
        waveOut->setBounds(getWidth() - margin - 25, cvY, 25, 25);
        hzSlider.setBounds(middleX - 35, gainY, 70, 70);
    }

private:
    CVJack *hzIn, *ampIn, *waveOut;
    Slider hzSlider;
};