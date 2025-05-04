/*
  ==============================================================================

    NoiseUI.h
    Created: 30 Apr 2025 9:58:05pm
    Author:  Tom

  ==============================================================================
*/


#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class NoiseUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 1;

    NoiseUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 0, 3) {
        rate.setValue(3.0);
        rate.setRange(0.0, 1000.0);
        rate.setSkewFactor(0.4);
        rate.setTitle("rate");
        rate.setName("rate");
        rate.setColours(knobBg, knobDot);
        addAndMakeVisible(rate);

        fine.setValue(0.);
        fine.setRange(-1.0, 1.0);
        fine.setTitle("fine");
        fine.setName("fine");
        fine.setColours(knobBg, knobDot);
        addAndMakeVisible(fine);

        for (int i = 0; i < getNumCVOutputs(); i++) {
            getCvOutputJack(i)->setJackColour(cvColour);
        }
    }

    void sliderValueChanged(juce::Slider* slider) override {
        if (slider == &rate) {
            stateWriter->setModuleProperty(getId(), "rate", rate.getValue());
        }
        if (slider == &fine) {
            stateWriter->setModuleProperty(getId(), "fine", fine.getValue());
        }
    }

    void startListeners() override {
        rate.addListener(this);
        fine.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);
        auto newRate = moduleState.state["rate"];
        if (!newRate.isVoid()) {
            rate.setValue(newRate);
        }

        auto newFine = moduleState.state["fine"];
        if (!newFine.isVoid()) {
            fine.setValue(newFine);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(bg);
        g.fillAll();

        g.setColour(text);
        g.setFont(16);

        auto bounds = rate.getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth() * 2);
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("s/h rate", bounds, Justification::centredBottom);

        bounds = fine.getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth() * 2);
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("fine", bounds, Justification::centredBottom);

        bounds = getCvOutputJack(0)->getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth() * 2);
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("pink", bounds, Justification::centredBottom);

        bounds = getCvOutputJack(1)->getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth() * 2);
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("white", bounds, Justification::centredBottom);

        bounds = getCvOutputJack(2)->getBounds();
        bounds.setY(bounds.getY() - bounds.getHeight());
        bounds.setWidth(bounds.getWidth() * 2);
        bounds.setX(bounds.getX() - bounds.getWidth() / 4);
        g.drawText("s/h", bounds, Justification::centredBottom);
    }

    void resized() override {
        rate.setSize(50,50);
        rate.setCentrePosition(getWidth() / 2, getHeight() * 0.15);

        fine.setSize(35,35);
        fine.setCentrePosition(getWidth() / 2, getHeight() * 0.35);

        getCvOutputJack(0)->setSize(25, 25);
        getCvOutputJack(0)->setCentrePosition(getWidth() * .50, getHeight() * .60);

        getCvOutputJack(1)->setSize(25, 25);
        getCvOutputJack(1)->setCentrePosition(getWidth() * .50, getHeight() * .60 + 25 + 18);

        getCvOutputJack(2)->setSize(25, 25);
        getCvOutputJack(2)->setCentrePosition(getWidth() * .50, getHeight() * .60 + 50 + 36);
    }

private:
    Dial rate, fine;
    Colour bg = Colour(0xFFF4AFB4);
    Colour text = Colour(0xFF272838);
    Colour knobBg = Colour(0xFF272838);
    Colour knobDot = Colour(0xFFF5FFC6);
    Colour cvColour = Colour(0xFF272838);
};