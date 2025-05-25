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
    PluginState() { numRows = 2; }
    PluginState(XmlElement* xml);

    XmlElement* toXml();

    ModuleState* getModule(int id);
    void removeModule(int id);

    void setNumRows(int newNumRows) { numRows = newNumRows;  }

    std::vector<ModuleState> moduleStates;
    int numRows;
};
