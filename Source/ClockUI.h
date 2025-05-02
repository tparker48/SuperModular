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
        addAndMakeVisible(hz);
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
        g.setColour(Colour(255, 60, 46));
        g.fillAll();
        g.setFont(16.0);
        g.setColour(Colour(255, 242, 209));
        auto bounds = hz.getBounds();
        bounds.setY(bounds.getY() - (bounds.getHeight() * 0.2));
        g.drawText("hz", bounds, Justification::centredTop);

    }

    void resized() override {
        hz.setSize(50, 50);
        hz.setCentrePosition(getWidth() / 2, getHeight() * .20);

        getCvOutputJack(0)->setSize(25,25);
        getCvOutputJack(0)->setCentrePosition(getWidth() * .5, getHeight() * .8);

    }

private:
    Dial hz;
};