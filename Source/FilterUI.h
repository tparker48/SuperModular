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

        
        hz.setColours(knobCol, dotCol);
        q.setColours(knobCol, dotCol);
        drive.setColours(knobCol, dotCol);
        getCvInputJack(0)->setJackColour(cvCol);
        getCvInputJack(1)->setJackColour(cvCol);
        getCvOutputJack(0)->setJackColour(cvCol);
        getCvOutputJack(1)->setJackColour(cvCol);
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
        g.setColour(bgCol);
        g.fillAll();

        paintComponentLabel(g, &hz, "cutoff", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, &q, "res", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, &drive, "drive", TOP, getWidth() * 0.05, textCol);
        paintComponentLabel(g, getCvInputJack(0), "input", TOP, getWidth() * 0.08, textCol);
        paintComponentLabel(g, getCvInputJack(1), "cutoff", TOP, getWidth() * 0.08, textCol);
        paintComponentLabel(g, getCvOutputJack(0), "lpf", TOP, getWidth() * 0.08, textCol);
        paintComponentLabel(g, getCvOutputJack(1), "hpf", TOP, getWidth() * 0.08, textCol);
    }

    void resized() override {
        hz.setSize(50, 50);
        hz.setCentrePosition(getWidth() * .5, getHeight() * .25);

        q.setSize(35, 35);
        q.setCentrePosition(getWidth() * .66, getHeight() * .45);

        drive.setSize(35, 35);
        drive.setCentrePosition(getWidth() * .33, getHeight() * .45);


        auto jackW = getCvInputJack(0)->getWidth();
        getCvInputJack(0)->setCentrePosition(jackW, getHeight() - jackW / 2 - getHeight() * 0.03);
        getCvInputJack(1)->setCentrePosition(jackW, getHeight() - jackW / 2 - getHeight() * 0.15);


        getCvOutputJack(0)->setCentrePosition(getWidth() - jackW, getHeight() - jackW/2 - getHeight() * 0.03);
        getCvOutputJack(1)->setCentrePosition(getWidth() - jackW, getHeight() - jackW/2 - getHeight()  * 0.15);
    }

private:
    Dial hz, q, drive;
    
    Colour cvCol = Colour(0xFF3D3B3C);
    Colour knobCol = Colour(0xFF3D3B3C);
    Colour dotCol = Colour(0xFFC2F970);
    Colour bgCol = Colour(0xFFA5A5A5);
    Colour textCol = Colour(0xFF3D3B3C);
};