/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleFactory.h"
#include "PluginProcessor.h"
#include "ModuleUIComponent.h"
#include "SharedStateWriter.h"

//==============================================================================
/**
*/

typedef ValueTreeState AudioProcessorValueTreeState;

class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    OtherLookAndFeel() {
        setColour(ResizableWindow::backgroundColourId, Colour(0xff070707));
    }
};

class SuperModularAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    SuperModularAudioProcessorEditor (SuperModularAudioProcessor&, SharedPluginState* sharedStatePtr);
    ~SuperModularAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void mouseUp(const MouseEvent& e) override;

    void timerCallback();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperModularAudioProcessor& audioProcessor;
    OtherLookAndFeel customLookAndFeel;

    ModuleGrid moduleGrid;
    PatchCableManager cableManager;

    SharedPluginState* sharedState;
    SharedStateWriter stateWriter;

    int hpPerRow = 20;
    int numRows = 3;
    int hpWidth = 0;
    int moduleHeight = 0;

    void showPopupMenu();

    template <typename M>
    void addNewModule();

    std::map<int, ModuleUIFactory> moduleFactories;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperModularAudioProcessorEditor)
};
