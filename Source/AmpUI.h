/*
  ==============================================================================

    AmpUI.h
    Created: 5 May 2025 9:28:08pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"

class AmpUI : public ModuleUI, public Slider::Listener {
public:
    AmpUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) 
        : ModuleUI(id, mg, cm, stateWriter, 2, 1)  {

        // Initialize Gain Knob
        gain.setValue(0.68);
        gain.setRange(0.001, 2.0);
        gain.setSkewFactor(0.75);
        gain.setColours(knobCol, dotCol);
        addAndMakeVisible(gain);

        modAmt.setValue(0.3);
        modAmt.setRange(0.00, 1.0);
        modAmt.setColours(knobCol, dotCol);
        addAndMakeVisible(modAmt);

        getCvInputJack(0)->setJackColour(cvCol);
        getCvInputJack(1)->setJackColour(cvCol);
        getCvOutputJack(0)->setJackColour(cvCol);
    }

    void startListeners() override {
        gain.addListener(this);
        modAmt.addListener(this);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &gain) {
            stateWriter->setModuleProperty(getId(), "gain", gain.getValue());
        }
        if (slider == &modAmt) {
            stateWriter->setModuleProperty(getId(), "modAmt", modAmt.getValue());
        }
    }

    void applyState(ModuleState& moduleState) override {
        auto newGain = moduleState.state["gain"];
        if (!newGain.isVoid()) {
            gain.setValue(newGain);
        }

        auto newModAmt = moduleState.state["modAmt"];
        if (!newModAmt.isVoid()) {
            modAmt.setValue(newModAmt);
        }
    }

    void paintModule(Graphics& g) override {
        paintComponentLabel(g, &gain, "gain", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.50);
        paintComponentLabel(g, getCvInputJack(1), "mod", TOP, getHeight() * 0.04, textCol, true, getWidth() * 0.50);
        paintComponentLabel(g, getCvInputJack(0), "in", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.38);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.38);
    }

    void resized() override {
        gain.setSize(40, 40);
        gain.setCentrePosition(getWidth() / 2, getHeight() * 0.3);
        getCvInputJack(1)->setCentrePosition(getWidth()/2, getHeight() * 0.50);
        modAmt.setSize(30, 30);
        modAmt.setCentrePosition(getWidth() / 2, getHeight() * 0.58);
        getCvInputJack(0)->setCentrePosition(getWidth()/2 - getWidth() * 0.22, getHeight() * 0.95);
        getCvOutputJack(0)->setCentrePosition(getWidth()/2 + getWidth() * 0.22, getHeight() * 0.95);
    }

    std::string getName() override {
        return "Amp";
    }

private:
    Dial gain, modAmt;

    //Colour bgCol = Colour(0xFFF5EFE4);
    //Colour textCol = Colour(0xFF60695C);
    //Colour knobCol = Colour(0xFF2E1F27);
    //Colour dotCol = Colour(0xFFF29E4C);
    //Colour cvCol = Colour(0xFF2E1F27);
};
