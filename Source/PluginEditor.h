/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleFactory.h"
#include "PluginProcessor.h"
#include "ModuleUI.h"
#include "SharedStateWriter.h"
#include "MenuLookAndFeel.h"

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
    void timerCallback();
    void loadState();
    void clearState();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void mouseUp(const MouseEvent& e) override;

    

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
    int numRows = 2;
    int hpWidth = 0;
    int moduleHeight = 0;

    void showPopupMenu(const MouseEvent& e);

    template <typename M>
    void addNewModule(ModuleType type, Point<int> location);

    std::map<ModuleType, ModuleUIFactory> moduleFactories;

    MenuLookAndFeel menuLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperModularAudioProcessorEditor)
};
