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
        ModuleUI(id, mg, cm, stateWriter, 1, 2) {
        lengthSlider.setValue(0.5);
        lengthSlider.setRange(0.001, 5.0);
        lengthSlider.setSkewFactor(0.5);
        lengthSlider.setTitle("length");
        lengthSlider.setName("length");
        addAndMakeVisible(lengthSlider);

        feedbackSlider.setValue(0.5);
        feedbackSlider.setRange(0.0, 0.95);
        feedbackSlider.setTitle("feedback");
        feedbackSlider.setName("feedback");
        addAndMakeVisible(feedbackSlider);

        dryWetSlider.setValue(0.5);
        dryWetSlider.setRange(0.0, 1.0);
        dryWetSlider.setTitle("dryWet");
        dryWetSlider.setName("dryWet");
        addAndMakeVisible(dryWetSlider);
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
        g.setColour(Colour(153, 153, 153));
        g.fillAll();
        g.setFont(16.0);

        g.setColour(Colours::black);
        auto bounds = lengthSlider.getBounds();
        bounds.setY(bounds.getY() - (bounds.getHeight() * 0.2));
        g.drawText("delay", bounds, Justification::centredTop);

        g.setColour(Colours::black);
        bounds = feedbackSlider.getBounds();
        bounds.setY(bounds.getY() - (bounds.getHeight() * 0.2));
        g.drawText("feedback", bounds, Justification::centredTop);

        g.setColour(Colours::black);
        bounds = dryWetSlider.getBounds();
        bounds.setY(bounds.getY() - (bounds.getHeight() * 0.2));
        g.drawText("dry/wet", bounds, Justification::centredTop);

        bounds = getCvInputJack(0)->getBounds();
        bounds.setWidth(bounds.getWidth() * 1.4);
        g.drawText("in", bounds, Justification::centredRight);

        bounds = getCvOutputJack(0)->getBounds();
        bounds.setWidth(bounds.getWidth() * 1.4);
        g.drawText("out", bounds, Justification::centredRight);


        bounds = getCvOutputJack(1)->getBounds();
        bounds.setWidth(bounds.getWidth() * 1.4);
        g.drawText("wet", bounds, Justification::centredRight);
    }

    void resized() override {
        lengthSlider.setSize(45, 45);
        lengthSlider.setCentrePosition(getWidth() * 0.3, getHeight()*0.20);

        feedbackSlider.setSize(45, 45);
        feedbackSlider.setCentrePosition(getWidth() * 0.7, getHeight() * 0.20);

        dryWetSlider.setSize(40, 40);
        dryWetSlider.setCentrePosition(getWidth() / 2, getHeight() * 0.5);

        getCvOutputJack(0)->setSize(25, 25);
        getCvOutputJack(0)->setCentrePosition(getWidth() * 0.8, getHeight() * .85);

        getCvOutputJack(1)->setSize(25, 25);
        getCvOutputJack(1)->setCentrePosition(getWidth() * 0.8, getHeight() * .85 + 25 + 15);

        getCvInputJack(0)->setSize(25, 25);
        getCvInputJack(0)->setCentrePosition(getWidth() * 0.2, getHeight() * .85);
    }

private:
    Dial lengthSlider, feedbackSlider, dryWetSlider;
};