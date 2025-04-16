/*
  ==============================================================================

    ModuleFactory.cpp
    Created: 12 Apr 2025 8:53:29pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleFactory.h"

void initModuleUIFactoryMap(std::map<int, ModuleUIFactory>& factoryMap) {
    factoryMap[TestModule::typeId] = createTestModule;
    factoryMap[AudioOutputUI::typeId] = createAudioOutputModule;
    factoryMap[OscillatorUI::typeId] = createOscillatorModule;
}

ModuleUIComponent* createTestModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new TestModule(id, mg, cables, writer);
}

ModuleUIComponent* createAudioOutputModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new AudioOutputUI(id, mg, cables, writer);
}

ModuleUIComponent* createOscillatorModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new OscillatorUI(id, mg, cables, writer);
}