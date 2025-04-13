/*
  ==============================================================================

    ModuleFactory.h
    Created: 12 Apr 2025 8:53:29pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleUIComponent.h"

using ModuleFactory = ModuleUIComponent* (*)(int , ModuleGrid* , PatchCableManager* , SharedStateWriter* );
ModuleUIComponent* createTestModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer);
void initModuleFactoryMap(std::map<int, ModuleFactory>& factoryMap);