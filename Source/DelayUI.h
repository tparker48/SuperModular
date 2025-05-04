/*
  ==============================================================================

    DelayUI.h
    Created: 1 May 2025 5:42:52pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class DelayUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    DelayUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 2, 2) {
        lengthSlider.setValue(0.5);
        lengthSlider.setRange(0.001, 5.0);
        lengthSlider.setSkewFactor(0.5);
        lengthSlider.setTitle("length");
        lengthSlider.setName("length");
        lengthSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(lengthSlider);

        feedbackSlider.setValue(0.5);
        feedbackSlider.setRange(0.0, 0.95);
        feedbackSlider.setTitle("feedback");
        feedbackSlider.setName("feedback");
        feedbackSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(feedbackSlider);

        dryWetSlider.setValue(0.5);
        dryWetSlider.setRange(0.0, 1.0);
        dryWetSlider.setTitle("dryWet");
        dryWetSlider.setName("dryWet");
        dryWetSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(dryWetSlider);

        getCvInputJack(0)->setJackColour(cvCol);
        getCvInputJack(1)->setJackColour(cvCol);
        getCvOutputJack(0)->setJackColour(cvCol);
        getCvOutputJack(1)->setJackColour(cvCol);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &lengthSlider) {
            stateWriter->setModuleProperty(getId(), "length", lengthSlider.getValue());
        }
        if (slider == &dryWetSlider) {
            stateWriter->setModuleProperty(getId(), "dryWet", dryWetSlider.getValue());
        }
        if (slider == &feedbackSlider) {
            stateWriter->setModuleProperty(getId(), "feedback", feedbackSlider.getValue());
        }
    }

    void startListeners() override {
        lengthSlider.addListener(this);
        dryWetSlider.addListener(this);
        feedbackSlider.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);

        auto length = moduleState.state.getProperty("length");
        if (!length.isVoid()) {
            lengthSlider.setValue(length);
        }

        auto dryWet = moduleState.state.getProperty("dryWet");
        if (!dryWet.isVoid()) {
            dryWetSlider.setValue(dryWet);
        }

        auto feedback = moduleState.state.getProperty("feedback");
        if (!feedback.isVoid()) {
            feedbackSlider.setValue(feedback);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(bgCol);
        g.fillAll();
        paintComponentLabel(g, &lengthSlider, "delay", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, &feedbackSlider, "feedback", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, &dryWetSlider, "dry/wet", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvInputJack(0), "delay", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvInputJack(1), "in", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "out", TOP, getHeight() * 0.03, textCol);
        paintComponentLabel(g, getCvOutputJack(1), "wet", TOP, getHeight() * 0.03, textCol);
    }

    void resized() override {
        lengthSlider.setSize(45, 45);
        lengthSlider.setCentrePosition(getWidth() * 0.3, getHeight()*0.20);

        feedbackSlider.setSize(45, 45);
        feedbackSlider.setCentrePosition(getWidth() * 0.7, getHeight() * 0.20);

        dryWetSlider.setSize(40, 40);
        dryWetSlider.setCentrePosition(getWidth() / 2, getHeight() * 0.5);

        getCvOutputJack(0)->setCentrePosition(getWidth() * 0.8, getHeight() * .80);
        getCvOutputJack(1)->setCentrePosition(getWidth() * 0.8, getHeight() * .80 + 25 + 15);
        getCvInputJack(0)->setCentrePosition(getWidth() * 0.2, getHeight() * .80);
        getCvInputJack(1)->setCentrePosition(getWidth() * 0.2, getHeight() * .80 + 25 + 15);
    }

private:
    Dial lengthSlider, feedbackSlider, dryWetSlider;

    Colour bgCol = Colour(0xFF4A4E69);
    Colour textCol = Colour(0xFFC9ADA7);
    Colour knobCol = Colour(0xFF22223B);
    Colour dotCol = Colour(0xFFC9ADA7);
    Colour cvCol = Colour(0xFF22223B);
};