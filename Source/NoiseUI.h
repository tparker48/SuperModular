/*
  ==============================================================================

    NoiseUI.h
    Created: 30 Apr 2025 9:58:05pm
    Author:  Tom

  ==============================================================================
*/


#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class NoiseUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 1;

    NoiseUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 0, 3) {

    }

    void sliderValueChanged(juce::Slider* slider) override
    {

    }

    void startListeners() override {

    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colour(255, 60, 46));
        g.fillAll();

    }

    void resized() override {

    }

private:
};