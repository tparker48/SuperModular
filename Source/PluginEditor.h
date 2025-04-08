/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Module.h"

//==============================================================================
/**
*/

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel() {
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff070707));
    }
};


class SuperModularAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SuperModularAudioProcessorEditor (SuperModularAudioProcessor&);
    ~SuperModularAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseUp(const juce::MouseEvent& e) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperModularAudioProcessor& audioProcessor;

    OtherLookAndFeel customLookAndFeel;

    int hpPerRow = 20;
    int numRows = 3;
    int hpWidth = 0;
    int moduleHeight = 0;

    void showPopupMenu();

    template <typename M>
    void addNewModule();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperModularAudioProcessorEditor)
};
