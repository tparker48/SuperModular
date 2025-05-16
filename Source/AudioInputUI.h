/*
  ==============================================================================

    AudioInputUI.h
    Created: 7 May 2025 11:03:13pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"

class AudioInputUI : public ModuleUI {
public:
    static const int hp = 1;

    AudioInputUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 0, 3) {
        for (int i = 0; i < getNumCVOutputs(); i++) {
            getCvOutputJack(i)->setJackColour(cvCol);
        }
    }

    void applyState(ModuleState& moduleState) override {

    }

    void paintModule(Graphics& g) override {
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, getCvOutputJack(0), "left", LEFT, getHeight() * 0.05, textCol);
        paintComponentLabel(g, getCvOutputJack(1), "right", LEFT, getHeight() * 0.05, textCol);
        paintComponentLabel(g, getCvOutputJack(2), "mono", LEFT, getHeight() * 0.05, textCol);
    }

    void resized() override {
        auto margin = getWidth() * 0.05;
        auto cvY = getHeight() * 0.66;
        auto paddingY = getHeight() * 0.15;

        getCvOutputJack(0)->setBounds(getWidth()- margin - 20, cvY - paddingY, 20, 20);
        getCvOutputJack(1)->setBounds(getWidth() - margin - 20, cvY, 20, 20);
        getCvOutputJack(2)->setBounds(getWidth() - margin - 20, cvY + paddingY, 20, 20);
    }
};