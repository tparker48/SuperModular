/*
  ==============================================================================

    AudioOutputUI.h
    Created: 16 Apr 2025 11:02:17pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "Dial.h"


class AudioOutputUI : public ModuleUI, public Slider::Listener {
public:
    static const int hp = 1;

    AudioOutputUI(int id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter) :
        ModuleUI(id, mg, cm, stateWriter, 3, 0) {
        inputLeft = getCvInputJack(0);
        inputRight = getCvInputJack(1);
        inputMono = getCvInputJack(2);

        inputLeft->setJackColour(cvCol);
        inputRight->setJackColour(cvCol);
        inputMono->setJackColour(cvCol);

        gainSlider.setValue(0.68);
        gainSlider.setRange(0.001, 1.3);
        gainSlider.setSkewFactor(0.75);
        gainSlider.setTitle("gain");
        gainSlider.setName("gain");
        gainSlider.setColours(knobCol, dotCol);
        addAndMakeVisible(gainSlider);
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &gainSlider) {
            stateWriter->setModuleProperty(getId(), "gain", gainSlider.getValue());
        }
    }

    void startListeners() override {
        gainSlider.addListener(this);
    }

    void applyState(ModuleState& moduleState) override {
        ModuleUI::applyState(moduleState);

        auto gain = moduleState.state.getProperty("gain");
        if (!gain.isVoid()) {
            gainSlider.setValue(gain);
        }
    }

    void paintModule(Graphics& g) override {
        paintComponentLabel(g, &gainSlider, "gain", TOP, getHeight() * 0.03, textCol, true, getWidth()*0.66);
        paintComponentLabel(g, inputLeft, "left", TOP, getHeight() * 0.05, textCol, true, getWidth()*0.66);
        paintComponentLabel(g, inputRight, "right", TOP, getHeight() * 0.05, textCol, true, getWidth()*0.66);
        paintComponentLabel(g, inputMono, "mono", TOP, getHeight() * 0.05, textCol, true, getWidth()*0.66);
    }

    void resizeModule() override {
        auto margin = getWidth() * 0.05;
        auto gainY = getHeight() * 0.2;
        auto cvY = getHeight() * 0.75;
        auto middleX = getWidth() * 0.5;
        auto paddingY = getHeight() * 0.17;

        inputLeft->setCentrePosition(getWidth()/2, cvY - paddingY);
        inputRight->setCentrePosition(getWidth()/2, cvY);
        inputMono->setCentrePosition(getWidth()/2, cvY + paddingY);
        gainSlider.setBounds(middleX - 25, gainY, 50, 50);
    }


    std::string getName() override {
        return "A.Out";
    }

private:
    CVJackComponent*inputLeft, *inputRight, *inputMono;
    Dial gainSlider;

    //Colour bgCol = Colour(0xFFF5EFE4);
    //Colour textCol = Colour(0xFF60695C);
    //Colour knobCol = Colour(0xFF2E1F27);
    //Colour dotCol = Colour(0xFFF29E4C);
    //Colour cvCol = Colour(0xFF2E1F27);
};