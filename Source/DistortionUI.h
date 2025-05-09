/*
  ==============================================================================

    DistortionUI.h
    Created: 1 May 2025 5:43:16pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"

class DistortionUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    DistortionUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter)
        : ModuleUI(id, mg, cm, stateWriter, 2, 1) {

        // Initialize Gain Knob
        gain.setValue(0.68);
        gain.setRange(0.001, 36.0);
        gain.setSkewFactor(0.75);
        gain.setColours(knobCol, dotCol);
        addAndMakeVisible(gain);

        color.setValue(880.0);
        color.setRange(300.0, 2500.0);
        color.setSkewFactor(0.5);
        color.setColours(knobCol, dotCol);
        addAndMakeVisible(color);

        dryWet.setValue(0.5);
        dryWet.setRange(0.0, 1.0);
        dryWet.setColours(knobCol, dotCol);
        addAndMakeVisible(dryWet);

        getCvInputJack(0)->setJackColour(cvCol);
        getCvOutputJack(0)->setJackColour(cvCol);
    }

    void startListeners() override {
        gain.addListener(this);
        color.addListener(this);
        dryWet.addListener(this);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &gain) {
            stateWriter->setModuleProperty(getId(), "gain", gain.getValue());
        }
        if (slider == &color) {
            stateWriter->setModuleProperty(getId(), "color", color.getValue());
        }
        if (slider == &dryWet) {
            stateWriter->setModuleProperty(getId(), "dryWet", dryWet.getValue());
        }

    }

    void applyState(ModuleState& moduleState) override {
        auto newGain = moduleState.state["gain"];
        if (!newGain.isVoid()) {
            gain.setValue(newGain);
        }
        auto newColor = moduleState.state["color"];
        if (!newColor.isVoid()) {
            color.setValue(newColor);
        }
        auto newDryWet = moduleState.state["dryWet"];
        if (!newDryWet.isVoid()) {
            dryWet.setValue(newDryWet);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, &gain, "gain", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, &color, "color", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, &dryWet, "dryWet", TOP, getHeight() * 0.03, textCol);

        paintComponentLabel(g, getCvInputJack(0), "in", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getHeight() * 0.03, textCol);
    }

    void resized() override {
        gain.setSize(50, 50);
        gain.setCentrePosition(getWidth() / 2, getHeight() * 0.25);

        color.setSize(40, 40);
        color.setCentrePosition(getWidth() / 3, getHeight() * 0.45);

        dryWet.setSize(40, 40);
        dryWet.setCentrePosition(getWidth() * 2/3, getHeight() * 0.45);


        getCvInputJack(0)->setCentrePosition(getWidth() / 2 - getWidth() * 0.3, getHeight() * 0.95);
        getCvOutputJack(0)->setCentrePosition(getWidth() / 2 + getWidth() * 0.3, getHeight() * 0.95);
    }

private:
    Dial gain, color, dryWet;
};