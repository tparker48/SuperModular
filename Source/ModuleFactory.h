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


enum ModuleType {
    TestModule,
    AudioOutput,
    Oscillator,
    Splitter,
    JoystickModule,
    Mixer
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
