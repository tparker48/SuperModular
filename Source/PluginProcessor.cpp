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
    initModuleFactoryMap(moduleFactories);
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
    //osc.prepareToPlay(sampleRate, samplesPerBlock, 440.0);
    //lfo.prepareToPlay(sampleRate, samplesPerBlock, 10.0);
    //audioOut.prepareToPlay(sampleRate, samplesPerBlock);

    // wire osc cv_out to audio_output cv_in
    //audioOut.getCVInputJack(0)->wirePtr(osc.getCVOutputJack(0)->getPtr());
    //audioOut.getCVInputJack(1)->wirePtr(osc.getCVOutputJack(0)->getPtr());
    //osc.getCVInputJack(0)->wirePtr(lfo.getCVOutputJack(0)->getPtr());
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
    //buffer.clear();

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++)
    //    buffer.clear(i, 0, buffer.getNumSamples());

    // update state
    std::vector<StateChangeMessage> messages;
    sharedState.recieve_updates(messages);
    for (auto message : messages) {
        switch (message.op) {
        case ADD: {
            if (message.state.getTypeId() == AudioOutputModule::typeId) {
                if (audioOutModule == nullptr) {
                    auto newModule = moduleFactories[message.state.getTypeId()](message.state.getId());
                    modules[message.state.getId()] = newModule;
                    newModule->prepareToPlay(getSampleRate(), getBlockSize());
                    audioOutModule = newModule;
                }
                break;
            }
            auto newModule = moduleFactories[message.state.getTypeId()](message.state.getId());
            modules[message.state.getId()] = newModule;
            newModule->prepareToPlay(getSampleRate(), getBlockSize());
            break;
        }
        case UPDATE:
            modules[message.state.getId()]->updateFromState(message.state);
            
            // update input cv
            for (int cvId = 0; cvId < message.state.getNumCvInputs(); cvId++) {
                auto mod1 = modules[message.state.getId()];
                auto connectionState = message.state.getInputCvConnection(cvId);
                auto otherModuleId = connectionState.first;
                auto otherCvId = connectionState.second;

                if (otherModuleId == -1 || otherCvId == -1) {
                    mod1->getCVInputJack(cvId)->clearPtr();
                    continue;
                }

                auto mod2 = modules[otherModuleId];
                mod1->getCVInputJack(cvId)->wirePtr(mod2->getCVOutputJack(otherCvId)->getPtr());
            }
            break;
        case DELETE:
            delete modules[message.state.getId()];
            modules.erase(message.state.getId());
            break;
        }
    }

    //
    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        for (auto modulePair : modules) {
            modulePair.second->processSample();
        }
    }
    if (audioOutModule) {
        if (auto outModule = dynamic_cast<AudioOutputModule*>(audioOutModule)) {
            outModule->processBlock(buffer);
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
    return new SuperModularAudioProcessorEditor(*this, &sharedState);
}

//==============================================================================
void SuperModularAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    sharedState.readFullState(localState);
    std::unique_ptr<XmlElement> xml(localState.toXml());
    copyXmlToBinary(*xml, destData);
}

void SuperModularAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName("SuperModularPluginState"))
            localState = PluginState(xmlState.get());

    // write plugin state to shared obj, and flag a reload for the editor
    sharedState.writeFullState(localState, true);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperModularAudioProcessor();
}
