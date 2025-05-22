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
        rate.setValue(3.0);
        rate.setRange(0.0, 1000.0);
        rate.setSkewFactor(0.4);
        rate.setTitle("rate");
        rate.setName("rate");
        rate.setColours(knobCol, dotCol);
        addAndMakeVisible(rate);

        fine.setValue(0.);
        fine.setRange(-1.0, 1.0);
        fine.setTitle("fine");
        fine.setName("fine");
        fine.setColours(knobCol, dotCol);
        addAndMakeVisible(fine);

        for (int i = 0; i < getNumCVOutputs(); i++) {
            getCvOutputJack(i)->setJackColour(cvCol);
        }
    }

    void sliderValueChanged(juce::Slider* slider) override {
        if (slider == &rate) {
            stateWriter->setModuleProperty(getId(), "rate", rate.getValue());
        }
        if (slider == &fine) {
            stateWriter->setModuleProperty(getId(), "fine", fine.getValue());
        }
    }

    void startListeners() override {
        rate.addListener(this);
        fine.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
        auto newRate = moduleState.state["rate"];
        if (!newRate.isVoid()) {
            rate.setValue(newRate);
        }

        auto newFine = moduleState.state["fine"];
        if (!newFine.isVoid()) {
            fine.setValue(newFine);
        }
    }

    void paintModule(Graphics& g) override {
        auto sectionH = getHeight() * 0.45;
        g.setColour(bgCol);
        fillRect(g, 0, getHeight() - sectionH, getWidth(), sectionH);
        g.setColour(Colours::black);
        drawRect(g, 0, getHeight() - sectionH, getWidth(), sectionH, 2.0);

        paintComponentLabel(g, &rate, "s/h hz", ComponentLabelStyle::TOP, getHeight() * 0.03, textCol, true, getWidth() * 0.80);
        paintComponentLabel(g, &fine, "fine", ComponentLabelStyle::TOP, getHeight() * 0.03, textCol, true, getWidth() * 0.66);
        paintComponentLabel(g, getCvOutputJack(0), "pink", ComponentLabelStyle::TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(1), "white", ComponentLabelStyle::TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(2), "s/h", ComponentLabelStyle::TOP, getHeight() * 0.03, textCol);
    }

    void resizeModule() override {
        rate.setSize(50,50);
        rate.setCentrePosition(getWidth() / 2, getHeight() * 0.25);

        fine.setSize(35,35);
        fine.setCentrePosition(getWidth() / 2, getHeight() * 0.45);

        getCvOutputJack(0)->setCentrePosition(getWidth() * .50, getHeight() * .65);
        getCvOutputJack(1)->setCentrePosition(getWidth() * .50, getHeight() * .65 + getCvOutputJack(1)->getWidth()   + getHeight() * 0.08);
        getCvOutputJack(2)->setCentrePosition(getWidth() * .50, getHeight() * .65 + 2*getCvOutputJack(1)->getWidth() + getHeight() * 0.16);
    }


    std::string getName() override {
        return "Noise";
    }

private:
    Dial rate, fine;
    // Colour bg = Colour(0xFFE16F7C);
    // Colour text = Colour(0xFFF5F8DE);
    // Colour knobCol = Colour(0xFF42213D);
    // Colour dotCol = Colour(0xFFF5FFC6);
    // Colour cvCol = Colour(0xFF42213D);
};