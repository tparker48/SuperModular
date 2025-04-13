/*
  ==============================================================================

    PluginState.h
    Created: 12 Apr 2025 7:13:55pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleState.h"

// The full state of the plugin, maintained on the processor side
class PluginState {
public:
    PluginState() {}
    PluginState(XmlElement* xml);

    XmlElement* toXml();

    ModuleState* getModule(int id);
    void removeModule(int id);

    std::vector<ModuleState> moduleStates;
};
