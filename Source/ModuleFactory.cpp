/*
  ==============================================================================

    ModuleFactory.cpp
    Created: 12 Apr 2025 8:53:29pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleFactory.h"

// Factory Mapps
void initModuleProcessorFactoryMap(std::map<ModuleType, ModuleFactory>& factoryMap)
{
    factoryMap[AudioOutput] = createAudioOutputProcessor;
    factoryMap[Oscillator] = createOscillatorProcessor;
    factoryMap[Splitter] = createSplitterProcessor;
    factoryMap[JoystickModule] = createJoystickProcessor;
    factoryMap[Mixer] = createMixerProcessor;
    factoryMap[Sequencer] = createSequencerProcessor;
    factoryMap[Filter] = createFilterProcessor;
    factoryMap[Noise] = createNoiseProcessor;
    factoryMap[Clock] = createClockProcessor;
    factoryMap[DelayModule] = createDelayProcessor;
    factoryMap[Amp] = createAmpProcessor;
}

void initModuleUIFactoryMap(std::map<ModuleType, ModuleUIFactory>& factoryMap) {
    factoryMap[AudioOutput] = createAudioOutputUI;
    factoryMap[Oscillator] = createOscillatorUI;
    factoryMap[Splitter] = createSplitterUI;
    factoryMap[JoystickModule] = createJoystickUI;
    factoryMap[Mixer] = createMixerUI;
    factoryMap[Sequencer] = createSequencerUI;
    factoryMap[Filter] = createFilterUI;
    factoryMap[Noise] = createNoiseUI;
    factoryMap[Clock] = createClockUI;
    factoryMap[DelayModule] = createDelayUI;
    factoryMap[Amp] = createAmpUI;
}


// Factories
ModuleProcessor* createAudioOutputProcessor(int id) {
    return new AudioOutputProcessor(id);
}
ModuleUI* createAudioOutputUI(int id,  ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new AudioOutputUI(id, mg, cables, writer);
}

ModuleProcessor* createOscillatorProcessor(int id) {
    return new OscillatorProcessor(id);

}
ModuleUI* createOscillatorUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new OscillatorUI(id, mg, cables, writer);
}

ModuleProcessor* createSplitterProcessor(int id) {
    return new SplitterProcessor(id);

}
ModuleUI* createSplitterUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new SplitterUI(id, mg, cables, writer);
}

ModuleProcessor* createJoystickProcessor(int id) {
    return new JoystickProcessor(id);

}
ModuleUI* createJoystickUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new JoystickUI(id, mg, cables, writer);
}

ModuleProcessor* createMixerProcessor(int id) {
    return new MixerProcessor(id);

}
ModuleUI* createMixerUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new MixerUI(id, mg, cables, writer);
}

ModuleProcessor* createSequencerProcessor(int id) {
    return new SequencerProcessor(id);

}
ModuleUI* createSequencerUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new SequencerUI(id, mg, cables, writer);
}

ModuleProcessor* createFilterProcessor(int id) {
    return new FilterProcessor(id);

}
ModuleUI* createFilterUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new FilterUI(id, mg, cables, writer);
}

ModuleProcessor* createNoiseProcessor(int id) {
    return new NoiseProcessor(id);

}
ModuleUI* createNoiseUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new NoiseUI(id, mg, cables, writer);
}

ModuleProcessor* createClockProcessor(int id) {
    return new ClockProcessor(id);

}
ModuleUI* createClockUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new ClockUI(id, mg, cables, writer);
}

ModuleProcessor* createDelayProcessor(int id) {
    return new DelayProcessor(id);

}
ModuleUI* createDelayUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new DelayUI(id, mg, cables, writer);
}

ModuleProcessor* createAmpProcessor(int id) {
    return new AmpProcessor(id);

}
ModuleUI* createAmpUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new AmpUI(id, mg, cables, writer);
}