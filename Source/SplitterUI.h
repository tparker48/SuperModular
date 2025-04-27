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
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colour(250, 94, 80));
        g.fillAll();
    }

    void resized() override {
        auto margin = getWidth() * 0.10;
        auto cvY = getHeight() * 0.3;
        auto paddingY = getHeight() * 0.2;

        getCvInputJack(0)->setBounds(Rectangle<int>(margin, cvY + 2 * paddingY, 25, 25));

        getCvOutputJack(0)->setBounds(Rectangle<int>(getWidth()- margin - 25, cvY + 0*paddingY, 25, 25));
        getCvOutputJack(1)->setBounds(Rectangle<int>(getWidth()- margin - 25, cvY + 1*paddingY, 25, 25));
        getCvOutputJack(2)->setBounds(Rectangle<int>(getWidth()- margin - 25, cvY + 2*paddingY, 25, 25));
        getCvOutputJack(3)->setBounds(Rectangle<int>(getWidth()- margin - 25, cvY + 3*paddingY, 25, 25));
        getCvOutputJack(4)->setBounds(Rectangle<int>(getWidth()- margin - 25, cvY + 4*paddingY, 25, 25));
        
    }
};