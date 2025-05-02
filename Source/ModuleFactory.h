/*
  ==============================================================================

    ModuleUIFactory.h
    Created: 12 Apr 2025 8:53:29pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleUI.h"
#include "ModuleProcessor.h"

#include "AudioOutputProcessor.h"
#include "AudioOutputUI.h"

#include "OscillatorProcessor.h"
#include "OscillatorUI.h"

#include "SplitterProcessor.h"
#include "SplitterUI.h"

#include "JoystickUI.h"
#include "JoystickProcessor.h"

#include "MixerUI.h"
#include "MixerProcessor.h"

#include "SequencerUI.h"
#include "SequencerProcessor.h"

#include "FilterUI.h"
#include "FilterProcessor.h"

#include "NoiseUI.h"
#include "NoiseProcessor.h"

#include "DelayUI.h"
#include "DelayProcessor.h"

#include "ClockUI.h"
#include "ClockProcessor.h"

enum ModuleType {
    TestModule,
    AudioOutput,
    Oscillator,
    Splitter,
    JoystickModule,
    Mixer,
    Sequencer,
    Filter,
    Noise,
    DelayModule,
    Clock
};

using ModuleFactory = ModuleProcessor * (*)(int);
using ModuleUIFactory = ModuleUI * (*)(int, ModuleGrid*, PatchCableManager*, SharedStateWriter*);

// Factory Mapps
void initModuleProcessorFactoryMap(std::map<ModuleType, ModuleFactory>& factoryMap);
void initModuleUIFactoryMap(std::map<ModuleType, ModuleUIFactory>& factoryMap);

// Factories
ModuleProcessor* createAudioOutputProcessor(int id);
ModuleUI* createAudioOutputUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createOscillatorProcessor(int id);
ModuleUI* createOscillatorUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createSplitterProcessor(int id);
ModuleUI* createSplitterUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createJoystickProcessor(int id);
ModuleUI* createJoystickUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createMixerProcessor(int id);
ModuleUI* createMixerUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createSequencerProcessor(int id);
ModuleUI* createSequencerUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createFilterProcessor(int id);
ModuleUI* createFilterUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createNoiseProcessor(int id);
ModuleUI* createNoiseUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createDelayProcessor(int id);
ModuleUI* createDelayUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);

ModuleProcessor* createClockProcessor(int id);
ModuleUI* createClockUI(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);