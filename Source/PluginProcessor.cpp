/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "TestRunner.h"

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
    initModuleProcessorFactoryMap(moduleFactories);

    if (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone) {
        TestRunner runner;
        runner.runAllTests();
    }
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
    for (auto module : modules) {
        module.second->prepareToPlay(sampleRate, samplesPerBlock);
    }
}

void SuperModularAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    for (int i = 0; i < modules.size(); i++) {
        delete modules[i];
    }
    modules.clear();
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
    applyStateUpdates();

    for (auto modulePair : modules) {
        modulePair.second->preBlockProcessing();
    }

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        for (auto modulePair : modules) {
            modulePair.second->processSample();
        }
    }

    if (audioOutModule) {
        if (auto outModule = dynamic_cast<AudioOutputProcessor*>(audioOutModule)) {
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

    // delete current modules
    for (auto module : modules) {
        delete module.second;
    }
    modules.clear();
    audioOutModule = nullptr;

    // add all modules
    for (auto module : localState.moduleStates) {
        addModule(module);
    }
    // prepare all modules
    for (auto module : modules) {
        module.second->prepareToPlay(getSampleRate(), getBlockSize());
    }
    // wire all modules
    for (auto module : localState.moduleStates) {
        updateModule(module);
    }
    
}

void SuperModularAudioProcessor::applyStateUpdates() {
    std::vector<StateChangeMessage> messages;
    sharedState.recieve_updates(messages);
    for (auto message : messages) {
        switch (message.op) {
        case ADD:
            addModule(message.state);
            break;
        case UPDATE:
            updateModule(message.state);
            break;
        case DELETE:
            deleteModule(message.state);
            break;
        }
    }
}

void SuperModularAudioProcessor::addModule(ModuleState state) {
    int id = state.getId();
    ModuleType type = (ModuleType)state.getTypeId();
    auto newModule = moduleFactories[type](id);
    newModule->prepareToPlay(getSampleRate(), getBlockSize());

    if (type == AudioOutput) {
        if (audioOutModule == nullptr) {
            audioOutModule = newModule;
        }
        else {
            return;
        }
        
    }

    modules[id] = newModule;
}
void SuperModularAudioProcessor::updateModule(ModuleState state) {
    if (modules.find(state.getId()) == modules.end()) { return; }

    int id = state.getId();
    auto module = modules[id];

    module->updateFromState(state);

    // update input cv
    for (int cvId = 0; cvId < state.getNumCvInputs(); cvId++) {
        auto otherModuleId = state.getInputCvConnection(cvId).first;
        auto otherCvId = state.getInputCvConnection(cvId).second;

        if (otherModuleId == -1 || otherCvId == -1) {
            module->getCVInputJack(cvId)->clearPtr();
        }
        else {
            auto other = modules[otherModuleId];
            module->getCVInputJack(cvId)->wirePtr(other->getCVOutputJack(otherCvId)->getPtr());
        }
    }
}
void SuperModularAudioProcessor::deleteModule(ModuleState state) {
    int id = state.getId();
    if (modules[state.getId()] == audioOutModule) {
        audioOutModule = nullptr;
    }
    delete modules[state.getId()];
    modules.erase(state.getId());
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperModularAudioProcessor();
}
