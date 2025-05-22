/*
  ==============================================================================

    ClockUI.h
    Created: 1 May 2025 5:56:13pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class ClockUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 1;

    ClockUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 0, 1) {
        hz.setValue(0.68);
        hz.setRange(0.0, 20.0);
        hz.setSkewFactor(0.6);
        hz.setTitle("hz");
        hz.setName("hz");
        hz.setColours(knobCol, dotCol);
        addAndMakeVisible(hz);

        getCvOutputJack(0)->setJackColour(cvCol);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &hz) {
            stateWriter->setModuleProperty(getId(), "hz", hz.getValue());
        }
    }

    void startListeners() override {
        hz.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);

        auto stateHz = moduleState.state.getProperty("hz");
        if (!stateHz.isVoid()) {
            hz.setValue(stateHz);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colours::white);
        fill(g);
        paintComponentLabel(g, &hz, "rate", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getHeight() * 0.03, textCol);

    }

    void resizeModule() override {
        hz.setSize(50, 50);
        hz.setCentrePosition(getWidth() / 2, getHeight() * .30);
        getCvOutputJack(0)->setCentrePosition(getWidth() * .5, getHeight() * .93);
    }

    std::string getName() override {
        return "Clock";
    }

private:
    Dial hz;

    //Colour bgCol = Colour(0xFF829191);
    //Colour textCol = Colour(0xFFEFF7CF);
    //Colour knobCol = Colour(0xFF2E1F27);
    //Colour dotCol = Colour(0xFFF29E4C);
    //Colour cvCol = Colour(0xFF2E1F27);
};