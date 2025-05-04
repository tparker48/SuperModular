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
        joystick.setBackgroundColour(Colour(0xFF71677C));
        joystick.setPadColour(Colour(20, 20, 20));
        joystick.setDotColour(Colour(252, 56, 56));
        joystick.setRanges(0.0, 1.0, 0.0, 1.0);
        joystick.setValues(0.5, 0.5, false, false);
        addAndMakeVisible(joystick);

        x.attachToComponent(cvOuts[0], false);
        x.setColour(x.textColourId, Colour(20, 20, 20));
        x.setText("x", dontSendNotification);
        y.attachToComponent(cvOuts[1], false);
        y.setColour(y.textColourId, Colour(20, 20, 20));
        y.setText("y", dontSendNotification);
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
        g.setColour(Colour(0xFF71677C));
        g.fillAll();
    }

    void resized() override {
        int cvSize = 25;
        int cvY = getHeight() * 0.8;
        int margin = getWidth() * 0.1;
        int joySize = getWidth() * 0.8;
        joystick.setBounds(margin, margin, joySize, joySize);
        joystick.setDotRadius(getWidth()*0.05);

        cvOuts[0]->setBounds(getWidth() - cvSize - margin - cvSize*2, cvY, cvSize, cvSize);
        cvOuts[1]->setBounds(getWidth() - cvSize - margin, cvY, cvSize, cvSize);
    }

private:
    Joystick joystick;
    Label x, y;
};