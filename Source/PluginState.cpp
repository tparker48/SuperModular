/*
  ==============================================================================

    PluginState.cpp
    Created: 12 Apr 2025 7:13:55pm
    Author:  Tom

  ==============================================================================
*/

#include "PluginState.h"


PluginState::PluginState(XmlElement* xml) {
    auto modules = xml->getChildByName("modules");

    if (modules) {
        for (auto* element : modules->getChildIterator()) {
            moduleStates.push_back(ModuleState(*element));
        }
    }
}

XmlElement* PluginState::toXml() {
    auto* xml = new XmlElement("SuperModularPluginState");
    auto modules = new XmlElement("modules");
    for (auto moduleState : moduleStates) {
        modules->addChildElement(moduleState.toXml());
    }
    xml->addChildElement(modules);
    return xml;
}

ModuleState* PluginState::getModule(int id) {
    auto module = std::find_if(moduleStates.begin(), moduleStates.end(), [id](ModuleState m) {return m.getId() == id; });
    if (module == moduleStates.end()) {
        return nullptr;
    }
    else {
        return &(*module);
    }
}
void PluginState::removeModule(int id) {
    std::remove_if(
        moduleStates.begin(),
        moduleStates.end(),
        [id](ModuleState m) {return m.getId() == id; });
}