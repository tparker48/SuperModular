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
    static const int hp = 3;

    SequencerUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 1, 1) {

        for (int i = 0; i < MAX_STEPS; i++) {
            stepSliders[i].setValue(0.0);
            stepSliders[i].setRange(-1.0, 1.0);
            stepSliders[i].setTitle(std::to_string(i));
            stepSliders[i].setName(std::to_string(i));
            stepSliders[i].setColours(knobCol, dotCol);
            addAndMakeVisible(stepSliders[i]);
        }
        rateSlider.setValue(0.5);
        rateSlider.setRange(0.0, 20.0);
        rateSlider.setSkewFactor(0.3);
        rateSlider.setTitle("rate");
        rateSlider.setName("rate");
        rateSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(rateSlider);

        numStepsSlider.setValue(MAX_STEPS);
        numStepsSlider.setRange(1, MAX_STEPS, 1);
        numStepsSlider.setTitle("steps");
        numStepsSlider.setName("steps");
        numStepsSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(numStepsSlider);

        getCvInputJack(0)->setJackColour(cvCol);
        getCvOutputJack(0)->setJackColour(cvCol);
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
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, getCvInputJack(0), "sync", TOP, getHeight() * 0.02, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getHeight() * 0.02, textCol);

        for (int i = 0; i < MAX_STEPS; i++) {
            paintComponentLabel(g, &stepSliders[i], std::to_string(i), TOP, getHeight() * 0.02, textCol);

        }

        paintComponentLabel(g, &rateSlider, "rate", TOP, getHeight() * 0.02, textCol);
        paintComponentLabel(g, &numStepsSlider, "steps", TOP, getHeight() * 0.02, textCol);
    }

    void resized() override {
        int margin = getHeight() * 0.05;
        int marginY = getHeight() * 0.3;
        int spacing = getHeight() * 0.17;
        int steKnobSize = getWidth() * 0.22;
        int knobSize = getWidth() * 0.25;

        rateSlider.setBounds(margin, margin, knobSize, knobSize);
        numStepsSlider.setBounds(margin + knobSize + spacing, margin, knobSize, knobSize);

        for (int i = 0; i < MAX_STEPS / 2; i++) {
            stepSliders[i].setSize(steKnobSize, steKnobSize);
            stepSliders[i].setCentrePosition(getWidth() / 2 - getWidth()*0.12, marginY + spacing * i);
        }
        for (int i = MAX_STEPS / 2; i < MAX_STEPS; i++) {
            stepSliders[i].setSize(steKnobSize, steKnobSize);
            stepSliders[i].setCentrePosition(getWidth() / 2 + getWidth() * 0.12, marginY + spacing * (i-4));
        }

        getCvOutputJack(0)->setSize(25, 25);
        getCvOutputJack(0)->setCentrePosition(getWidth() - margin - 12.5, getHeight() * 0.95);
        getCvInputJack(0)->setSize(25, 25);
        getCvInputJack(0)->setCentrePosition(0 + margin + 12.5, getHeight() * 0.95);
    }

private:
    static const  int MAX_STEPS = 8;
    Dial stepSliders[MAX_STEPS];
    Dial rateSlider, numStepsSlider;

    Colour bgCol = Colour(0xFFBAD9B5);
    Colour textCol = Colour(0xFF393424);
    Colour knobCol = Colour(0xFF393424);
    Colour dotCol = Colour(0xFFEFF7CF);
    Colour cvCol = Colour(0xFF393424);
};