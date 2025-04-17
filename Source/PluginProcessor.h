/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SharedPluginState.h"
#include "ModuleProcessor.h"
#include "ModuleFactory.h"
#include "AudioOutputProcessor.h"

typedef AudioProcessorValueTreeState ValueTreeState;


//==============================================================================
/**
*/
class SuperModularAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SuperModularAudioProcessor();
    ~SuperModularAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void applyStateUpdates();
    void addModule(ModuleState state);
    void updateModule(ModuleState state);
    void deleteModule(ModuleState state);

private:
    //==============================================================================
    SharedPluginState sharedState;
    PluginState localState;

    std::map<ModuleType, ModuleFactory> moduleFactories;
    std::unordered_map<int, ModuleProcessor*> modules;
    ModuleProcessor* audioOutModule = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperModularAudioProcessor)
};
