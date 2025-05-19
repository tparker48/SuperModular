/*
  ==============================================================================

    JoystickUI.h
    Created: 20 Apr 2025 5:00:56pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Joystick.h"

class JoystickUI : public ModuleUI, public JoystickListener {
public:
    static const int hp = 4;

    JoystickUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 0, 2) {
        joystick.setBackgroundColour(Colour(bgCol));
        joystick.setRanges(0.0, 1.0, 0.0, 1.0);
        joystick.setValues(0.5, 0.5, false, false);
        addAndMakeVisible(joystick);

        getCvOutputJack(0)->setJackColour(cvCol);
        getCvOutputJack(1)->setJackColour(cvCol);
    }

    void joystickValueChanged(Joystick* joystickPtr) override {
        if (joystickPtr == &joystick) {
            auto newX = (2.0 * joystick.getHorizontalValue()) - 1.0;
            auto newY = (2.0 * joystick.getVerticalValue()) - 1.0;
            stateWriter->setModuleProperties(
                getId(), 
                { 
                    { "joyX", newX },
                    { "joyY", newY }
                });
        }
    }

    void startListeners() override {
        joystick.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        auto x = moduleState.state.getProperty("joyX");
        auto y = moduleState.state.getProperty("joyY");
        if (!x.isVoid() && !y.isVoid()) {
            joystick.setValues(
                ((float)x + 1.0) / 2.0,
                1.0 - ((float)y + 1.0) / 2.0,
                false,
                false);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colours::white);
        //fill(g);
        paintComponentLabel(g, getCvOutputJack(0), "x", BOTTOM, getWidth() * 0.05, textCol, true, getWidth()*0.10);
        paintComponentLabel(g, getCvOutputJack(1), "y", BOTTOM, getWidth() * 0.05, textCol, true, getWidth()*0.10);
    }

    void resized() override {
        int cvSize = getCvOutputJack(0)->getWidth();;
        int cvY = getHeight() * 0.85;
        int margin = getWidth() * 0.1;
        int joySize = getWidth() * 0.8;
        joystick.setBounds(margin, getHeight()*0.15, joySize, joySize);
        joystick.setDotRadius(getWidth()*0.05);

        getCvOutputJack(0)->setCentrePosition(
            getWidth() - cvSize - margin - cvSize*2,
            cvY
        );
        getCvOutputJack(1)->setCentrePosition(
            getWidth() - cvSize - margin, 
            cvY
        );
    }


    std::string getName() override {
        return "Joystick";
    }

private:
    Joystick joystick;

    //Colour bgCol = Colour(0xFF829191);
    //Colour textCol = Colour(0xFFEFF7CF);
    //Colour knobCol = Colour(0xFF2E1F27);
    //Colour dotCol = Colour(0xFFF29E4C);
    //Colour cvCol = Colour(0xFF2E1F27);
};