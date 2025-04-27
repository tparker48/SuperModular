/*
  ==============================================================================

    MixerUI.h
    Created: 20 Apr 2025 7:30:08pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"


class MixerUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    MixerUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, numInputs, 1) {

        levelSliders = { &lev1, &lev2, &lev3, &lev4,  &lev5 };
        levelLabels = { &lab1, &lab2, &lab3, &lab4,  &lab5 };

        for (int i = 0; i < numInputs; i++) {
            levelSliders[i]->setSliderStyle(Slider::RotaryHorizontalDrag);
            levelSliders[i]->setRange(0.0, 1.0, 0.0);
            levelSliders[i]->setValue(1.0);
            levelSliders[i]->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
            levelLabels[i]->attachToComponent(getCvInputJack(i), true);
            levelLabels[i]->setColour(levelLabels[i]->textColourId, Colour(20, 20, 20));
            levelLabels[i]->setText(std::to_string(i), dontSendNotification);
            addAndMakeVisible(levelSliders[i]);
        }
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        stateWriter->setModuleProperties(getId(), {
            { "l1", levelSliders[0]->getValue() },
            { "l2", levelSliders[1]->getValue() },
            { "l3", levelSliders[2]->getValue() },
            { "l4", levelSliders[3]->getValue() },
            { "l5", levelSliders[4]->getValue() }
        });
    }

    void startListeners() override {
        for (int i = 0; i < numInputs; i++) {
            levelSliders[i]->addListener(this);
        }
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
        auto newL1 = moduleState.state.getProperty("l1");
        auto newL2 = moduleState.state.getProperty("l2");
        auto newL3 = moduleState.state.getProperty("l3");
        auto newL4 = moduleState.state.getProperty("l4");
        auto newL5 = moduleState.state.getProperty("l5");
        if (!newL1.isVoid()) {
            levelSliders[0]->setValue(newL1);
        }
        if (!newL2.isVoid()) {
            levelSliders[1]->setValue(newL2);
        }
        if (!newL3.isVoid()) {
            levelSliders[2]->setValue(newL3);
        }
        if (!newL4.isVoid()) {
            levelSliders[3]->setValue(newL4);
        }
        if (!newL5.isVoid()) {
            levelSliders[4]->setValue(newL5);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colours::lightsalmon);
        g.fillAll();
    }

    void resized() override {
        auto margin = getWidth() * 0.15;
        auto cvY = getHeight() * 0.05;
        auto cvSpacing = 25 + getHeight() * 0.08;
        for (int i = 0; i < numInputs; i++) {
            getCvInputJack(i)->setBounds(margin, cvY + cvSpacing * i, 25, 25);
            levelSliders[i]->setBounds(getWidth()-margin-30, cvY + cvSpacing * i - 12, 50, 50);
        }
        getCvOutputJack(0)->setBounds(getWidth() - 25 - margin/2, getHeight() - 25 - margin, 25, 25);
    }

private:
    Slider lev1, lev2, lev3, lev4, lev5;
    Label lab1, lab2, lab3, lab4, lab5;
    std::vector<Slider*> levelSliders;
    std::vector<Label*> levelLabels;
    static const int numInputs = 5;
};