/*
  ==============================================================================

    Module.cpp
    Created: 14 Apr 2025 9:27:46pm
    Author:  Tom

  ==============================================================================
*/

#include "Module.h"

void initModuleFactoryMap(std::map<int, ModuleFactory>& factoryMap)
{
    factoryMap[AudioOutputModule::typeId] = createAudioOutputModule;
    factoryMap[OscillatorModule::typeId] = createOscillatorModule;
}

Module* createAudioOutputModule(int id) {
    return new AudioOutputModule(id);
}
Module* createOscillatorModule(int id) {
    return new OscillatorModule(id);

}