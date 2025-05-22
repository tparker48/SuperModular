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
        addAndMakeVisible(gain);

        color.setValue(880.0);
        color.setRange(300.0, 2500.0);
        color.setSkewFactor(0.5);
        addAndMakeVisible(color);

        dryWet.setValue(0.5);
        dryWet.setRange(0.0, 1.0);
        addAndMakeVisible(dryWet);
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
        paintComponentLabel(g, &gain, "gain", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.25);
        paintComponentLabel(g, &color, "color", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.30);
        paintComponentLabel(g, &dryWet, "dry/wet", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.40);

        paintComponentLabel(g, getCvInputJack(1), "color", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.30);
        paintComponentLabel(g, getCvInputJack(0), "in", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.25);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.25);
    }

    void resizeModule() override {
        gain.setSize(50, 50);
        gain.setCentrePosition(getWidth() / 2, getHeight() * 0.25);

        color.setSize(40, 40);
        color.setCentrePosition(getWidth()/2 - getWidth()*0.25, getHeight() * 0.45);

        dryWet.setSize(40, 40);
        dryWet.setCentrePosition(getWidth()/2 + getWidth()*0.25, getHeight() * 0.45);

        getCvInputJack(1)->setCentrePosition(getWidth() / 2 - getWidth() * 0.3, getHeight() * 0.95 - getHeight()*0.15);
        getCvInputJack(0)->setCentrePosition(getWidth() / 2 - getWidth() * 0.3, getHeight() * 0.95);
        getCvOutputJack(0)->setCentrePosition(getWidth() / 2 + getWidth() * 0.3, getHeight() * 0.95);
    }


    std::string getName() override {
        return "Distortion";
    }

private:
    Dial gain, color, dryWet;
};