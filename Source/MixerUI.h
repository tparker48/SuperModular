/*
  ==============================================================================

    MixerUI.h
    Created: 20 Apr 2025 7:30:08pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class MixerUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    MixerUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, numInputs, 1) {

        levelSliders = { &lev1, &lev2, &lev3, &lev4,  &lev5 };

        for (int i = 0; i < numInputs; i++) {
            levelSliders[i]->setRange(0.0001, 1.3, 0.0);
            levelSliders[i]->setValue(0.3);
            levelSliders[i]->setSkewFactor(0.75);
            levelSliders[i]->setColours(knobCol, dotCol);
            addAndMakeVisible(levelSliders[i]);
        }
        for (int i = 0; i < numInputs; i++) {
            getCvInputJack(i)->setJackColour(cvCol);
        }
        for (int i = 0; i < getNumCVOutputs(); i++) {
            getCvOutputJack(i)->setJackColour(cvCol);
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
        g.setColour(bgCol);
        g.fillAll();

        for (int i = 0; i < getNumCVInputs(); i++) {
            paintComponentLabel(g, getCvInputJack(i), "in " + std::to_string(i), LEFT, getWidth() * 0.1, textCol);
        }
        paintComponentLabel(g, getCvOutputJack(0), "out", LEFT, getWidth() * 0.1, textCol);
    }

    void resized() override {
        auto margin = getWidth() * 0.15;
        auto cvY = getHeight() * 0.15;
        auto cvW = getCvInputJack(0)->getWidth();
        auto cvSpacing = cvW + getHeight() * 0.08;
        for (int i = 0; i < numInputs; i++) {
            getCvInputJack(i)->setCentrePosition(
                getWidth()/2 - margin,
                cvY + cvSpacing * i
            );
            levelSliders[i]->setSize(40, 40);
            levelSliders[i]->setCentrePosition(
                getWidth()/2 + margin,
                cvY + cvSpacing * i
            );
        }
        getCvOutputJack(0)->setCentrePosition(getWidth()*0.88, getHeight()*0.95);
    }

private:
    Dial lev1, lev2, lev3, lev4, lev5;
    std::vector<Dial*> levelSliders;
    static const int numInputs = 5;

    // Colour bgCol = Colour(0xFFF5EFE4);
    // Colour textCol = Colour(0xFF60695C);
    // Colour knobCol = Colour(0xFF2E1F27);
    // Colour dotCol = Colour(0xFFF29E4C);
    // Colour cvCol = Colour(0xFF2E1F27);
};