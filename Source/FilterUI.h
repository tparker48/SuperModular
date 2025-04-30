/*
  ==============================================================================

    FilterUI.h
    Created: 29 Apr 2025 11:48:20pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"
#include "Toggle.h"


class FilterUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 2;

    FilterUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 2, 2) {
        hz.setValue(4000);
        hz.setRange(20.0, 18000.0);
        hz.setSkewFactor(.3);
        hz.setTitle("hz");
        hz.setName("hz");
        addAndMakeVisible(hz);

        q.setValue(0.8);
        q.setRange(0.0, 1.0);
        q.setTitle("resonance");
        q.setName("resonance");
        addAndMakeVisible(q);

        drive.setValue(1.0);
        drive.setRange(1.0, 15.0);
        drive.setTitle("drive");
        drive.setName("drive");
        addAndMakeVisible(drive);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &hz) {
            stateWriter->setModuleProperty(getId(), "hz", hz.getValue());
        }
        if (slider == &q) {
            stateWriter->setModuleProperty(getId(), "q", q.getValue());
        }
        if (slider == &drive) {
            stateWriter->setModuleProperty(getId(), "drive", drive.getValue());
        }
    }

    void startListeners() override {
        hz.addListener(this);
        q.addListener(this);
        drive.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);

        auto stateHz = moduleState.state.getProperty("hz");
        if (!stateHz.isVoid()) {
            hz.setValue(stateHz);
        }
        auto stateQ = moduleState.state.getProperty("q");
        if (!stateQ.isVoid()) {
            q.setValue(stateQ);
        }
        auto stateDrive = moduleState.state.getProperty("drive");
        if (!stateDrive.isVoid()) {
            drive.setValue(stateDrive);
        }
    }

    void paintModule(Graphics& g) override {
        g.setColour(Colour(153, 153, 153));
        g.fillAll();
        g.setFont(16.0);
    }

    void resized() override {
        hz.setSize(50, 50);
        hz.setCentrePosition(getWidth() * .5, getHeight() * .25);

        q.setSize(35, 35);
        q.setCentrePosition(getWidth() * .66, getHeight() * .45);

        drive.setSize(35, 35);
        drive.setCentrePosition(getWidth() * .33, getHeight() * .45);

        getCvInputJack(0)->setSize(25, 25);
        getCvInputJack(0)->setCentrePosition(25, getHeight() - 25 - 15);

        getCvInputJack(0)->setSize(25, 25);
        getCvInputJack(0)->setCentrePosition(25, getHeight() - 25 - 15 - 30);

        getCvOutputJack(0)->setSize(25, 25);
        getCvOutputJack(0)->setCentrePosition(getWidth() - 25 - 15, getHeight() - 25 - 15 - 30);

        getCvOutputJack(1)->setSize(25, 25);
        getCvOutputJack(1)->setCentrePosition(getWidth() - 25 - 15, getHeight() - 25 - 15);
    }

private:
    Dial hz, q, drive;
};