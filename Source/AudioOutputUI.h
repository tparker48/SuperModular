/*
  ==============================================================================

    AudioOutputUI.h
    Created: 16 Apr 2025 11:02:17pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"


class AudioOutputUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 1;

    AudioOutputUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 2, 0) {
        inputLeft = getCvInputJack(0);
        inputRight = getCvInputJack(1);

        gainSlider.setSliderStyle(Slider::RotaryHorizontalDrag);
        gainSlider.setValue(0.8);
        gainSlider.setRange(0.0, 1.0);
        gainSlider.setTitle("gain");
        gainSlider.setName("gain");
        gainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0,0);
        addAndMakeVisible(gainSlider);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &gainSlider) {
            stateWriter->setModuleProperty(getId(), "gain", gainSlider.getValue());
        }
    }

    void startListeners() override {
        gainSlider.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);

        auto gain = moduleState.state.getProperty("gain");
        if (!gain.isVoid()) {
            gainSlider.setValue(gain);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colour(238, 255, 230));
        g.fillAll();
        g.setFont(16.0);

        g.setColour(Colours::black);
        auto bounds = gainSlider.getBounds();
        bounds.setY(bounds.getY() - (bounds.getHeight() * 0.2));
        g.drawText("gain", bounds, Justification::centredTop);

        auto inputLeft = getCvInputJack(0);
        bounds = inputLeft->getBounds();
        bounds.setWidth(bounds.getWidth() * 1.4);
        g.drawText("L", bounds, Justification::centredRight);

        auto inputRight = getCvInputJack(1);
        bounds = inputRight->getBounds();
        bounds.setWidth(bounds.getWidth() * 1.4);
        g.drawText("R", bounds, Justification::centredRight);
    }

    void resized() override {
        auto margin = getWidth() * 0.10;
        auto gainY = getHeight() * 0.2;
        auto cvY = getHeight() * 0.66;
        auto middleX = getWidth() * 0.5;
        auto paddingY = getHeight() * 0.2;

        inputLeft->setBounds(margin, cvY - paddingY, 25, 25);
        inputRight->setBounds(margin, cvY, 25, 25);
        gainSlider.setBounds(middleX - 25, gainY, 50, 50);
    }

private:
    CVJackComponent*inputLeft, *inputRight;
    Slider gainSlider;
};