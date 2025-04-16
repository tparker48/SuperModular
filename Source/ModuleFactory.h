/*
  ==============================================================================

    ModuleUIFactory.h
    Created: 12 Apr 2025 8:53:29pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleUIComponent.h"

using ModuleUIFactory = ModuleUIComponent* (*)(int , ModuleGrid* , PatchCableManager* , SharedStateWriter* );
void initModuleUIFactoryMap(std::map<int, ModuleUIFactory>& factoryMap);

ModuleUIComponent* createTestModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);
ModuleUIComponent* createAudioOutputModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);
ModuleUIComponent* createOscillatorModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);