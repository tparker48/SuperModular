/*
  ==============================================================================

    ModuleFactory.cpp
    Created: 12 Apr 2025 8:53:29pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleFactory.h"

void initModuleFactoryMap(std::map<int, ModuleFactory>& factoryMap) {
    factoryMap[TestModule::typeId] = createTestModule;
}

ModuleUIComponent* createTestModule(int id, ModuleGrid* mg, PatchCableManager* cables, SharedStateWriter* writer) {
    return new TestModule(id, mg, cables, writer);
}