/*
  ==============================================================================

    SequencerUI.h
    Created: 26 Apr 2025 5:32:15pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class SequencerUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 4;

    SequencerUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 1, 1) {

        for (int i = 0; i < MAX_STEPS; i++) {
            stepSliders[i].setValue(0.0);
            stepSliders[i].setRange(-1.0, 1.0);
            stepSliders[i].setTitle(std::to_string(i));
            stepSliders[i].setName(std::to_string(i));
            addAndMakeVisible(stepSliders[i]);
        }
        rateSlider.setValue(0.5);
        rateSlider.setRange(0.0, 20.0);
        rateSlider.setSkewFactor(0.3);
        rateSlider.setTitle("rate");
        rateSlider.setName("rate");
        addAndMakeVisible(rateSlider);

        numStepsSlider.setValue(MAX_STEPS);
        numStepsSlider.setRange(1, MAX_STEPS, 1);
        numStepsSlider.setTitle("steps");
        numStepsSlider.setName("steps");
        addAndMakeVisible(numStepsSlider);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &rateSlider) {
            stateWriter->setModuleProperty(getId(), "rate", rateSlider.getValue());
        }
        else if (slider == &numStepsSlider) {
            stateWriter->setModuleProperty(getId(), "steps", numStepsSlider.getValue());
        }
        else {
            for (int i = 0; i < MAX_STEPS; i++) {
                if (slider == &stepSliders[i]) {
                    stateWriter->setModuleProperty(getId(), Identifier("step" + std::to_string(i)), stepSliders[i].getValue());
                    break;
                }
            }
        }
    }

    void startListeners() override {
        rateSlider.addListener(this);
        numStepsSlider.addListener(this);
        for (int i = 0; i < MAX_STEPS; i++) {
            stepSliders[i].addListener(this);
        }
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
        auto steps = moduleState.state.getProperty("steps");
        if (!steps.isVoid()) {
            numStepsSlider.setValue(steps);
        }
        auto rate = moduleState.state.getProperty("rate");
        if (!rate.isVoid()) {
            rateSlider.setValue(rate);
        }
        for (int i = 0; i < MAX_STEPS; i++) {
            auto step = moduleState.state.getProperty(Identifier("step" + std::to_string(i)));
            if (!step.isVoid()) {
                stepSliders[i].setValue(step);
            }
        }

    }

    void paintModule(Graphics& g) override {
        g.setColour(Colour(228, 224, 255));
        g.fillAll();
    }

    void resized() override {
        int margin = getHeight() * 0.05;
        int spacing = getHeight() * 0.05;
        int steKnobSize = getWidth() * 0.20;
        int knobSize = getWidth() * 0.25;

        rateSlider.setBounds(margin, 5*margin, knobSize, knobSize);
        numStepsSlider.setBounds(margin, 5*margin + knobSize + spacing, knobSize, knobSize);

        for (int i = 0; i < MAX_STEPS / 2; i++) {
            stepSliders[i].setBounds(
                6 * margin, 
                margin + ((steKnobSize +spacing) * i),
                steKnobSize,
                steKnobSize
            );
        }
        for (int i = MAX_STEPS / 2; i < MAX_STEPS; i++) {
            stepSliders[i].setBounds(
                6 * margin + spacing + steKnobSize,
                margin + ((steKnobSize + spacing) * (i - 4)),
                steKnobSize,
                steKnobSize
            );
        }

        getCvOutputJack(0)->setBounds(getWidth() - margin - 25, getHeight() - margin - 25, 25, 25);
        getCvInputJack(0)->setSize(25,25);
        getCvInputJack(0)->setCentrePosition(getWidth() - margin, getHeight() - margin);
    }

private:
    static const  int MAX_STEPS = 8;
    Dial stepSliders[MAX_STEPS];
    Dial rateSlider, numStepsSlider;
};