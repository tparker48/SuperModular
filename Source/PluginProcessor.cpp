/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperModularAudioProcessor::SuperModularAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )      
#endif
{
}

SuperModularAudioProcessor::~SuperModularAudioProcessor()
{
}

//==============================================================================
const String SuperModularAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SuperModularAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SuperModularAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SuperModularAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SuperModularAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SuperModularAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SuperModularAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SuperModularAudioProcessor::setCurrentProgram (int index)
{
}

const String SuperModularAudioProcessor::getProgramName (int index)
{
    return {};
}

void SuperModularAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SuperModularAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{


}

void SuperModularAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SuperModularAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SuperModularAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.clear();

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
        buffer.clear(i, 0, buffer.getNumSamples());

    // do something
    std::vector<PluginStateUpdateMessage> messages;
    stateMessageQueue.recieve_messages(messages);
    for (auto message : messages) {
        int x;
        switch (message.op) {
        case ADD:
            x = 1;
            break;
        case UPDATE:
            x = 2;
            break;
        case DELETE:
            x = 3;
            break;
        }
    }
}

//==============================================================================
bool SuperModularAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SuperModularAudioProcessor::createEditor()
{
    return new SuperModularAudioProcessorEditor(*this, &stateMessageQueue);
}

//==============================================================================
void SuperModularAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(localState.toXml());
    copyXmlToBinary(*xml, destData);
}

void SuperModularAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName("SuperModularPluginState"))
            localState = PluginState(xmlState.get());

    stateMessageQueue.writeFullState(localState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperModularAudioProcessor();
}
