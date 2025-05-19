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
        paintComponentLabel(g, getCvOutputJack(0), "left", TOP, getHeight() * 0.05, textCol, true, getWidth() * 0.66);
        paintComponentLabel(g, getCvOutputJack(1), "right", TOP, getHeight() * 0.05, textCol, true, getWidth() * 0.66);
        paintComponentLabel(g, getCvOutputJack(2), "mono", TOP, getHeight() * 0.05, textCol, true, getWidth() * 0.66);
    }

    void resized() override {
        auto cvY = getHeight() * 0.75;
        auto paddingY = getHeight() * 0.17;

        getCvOutputJack(0)->setCentrePosition(getWidth() / 2, cvY - paddingY);
        getCvOutputJack(1)->setCentrePosition(getWidth() / 2, cvY);
        getCvOutputJack(2)->setCentrePosition(getWidth() / 2, cvY + paddingY);
    }


    std::string getName() override {
        return "A.In";
    }
};