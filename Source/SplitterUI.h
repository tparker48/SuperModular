/*
  ==============================================================================

    SplitterUI.h
    Created: 17 Apr 2025 12:37:37am
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"


class SplitterUI : public ModuleUI {
public:
    static const int hp = 1;

    SplitterUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 1, 5) {

        getCvOutputJack(0)->setJackColour(cvCol);
        getCvOutputJack(1)->setJackColour(cvCol);
        getCvOutputJack(2)->setJackColour(cvCol);
        getCvOutputJack(3)->setJackColour(cvCol);
        getCvOutputJack(4)->setJackColour(cvCol);
        getCvInputJack(0)->setJackColour(cvCol);
    }

    void paintModule(Graphics& g) override {
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, getCvInputJack(0),  "in",    TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "out 1", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(1), "out 2", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(2), "out 3", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(3), "out 4", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(4), "out 5", TOP, getHeight() * 0.03, textCol);
    }

    void resized() override {
        auto cvY = getHeight() * 0.2;
        auto paddingY = getHeight() * 0.15;

        
        getCvOutputJack(0)->setCentrePosition(getWidth() / 2, getHeight() * 0.15);
        getCvOutputJack(1)->setCentrePosition(getWidth() / 2, getHeight() * 0.15 + 1 * paddingY);
        getCvOutputJack(2)->setCentrePosition(getWidth() / 2, getHeight() * 0.15 + 2 * paddingY);
        getCvOutputJack(3)->setCentrePosition(getWidth() / 2, getHeight() * 0.15 + 3 * paddingY);
        getCvOutputJack(4)->setCentrePosition(getWidth() / 2, getHeight() * 0.15 + 4 * paddingY);

        getCvInputJack(0)->setCentrePosition(getWidth() / 2, getHeight()*0.95 - getCvInputJack(0)->getWidth()/2);
    }
    
private:
    Colour bgCol = Colour(0xFFF5EFE4);
    Colour textCol = Colour(0xFF60695C);
    Colour knobCol = Colour(0xFF2E1F27);
    Colour dotCol = Colour(0xFFF29E4C);
    Colour cvCol = Colour(0xFF2E1F27);
};