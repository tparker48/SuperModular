/*
  ==============================================================================

    StateChangeMessage.h
    Created: 12 Apr 2025 7:16:35pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginState.h"

enum PLUGIN_STATE_MESSAGE_OP_CODE {
    ADD,
    UPDATE,
    DELETE
};

// Contains a list of changes to make
class StateChangeMessage {
public:
    StateChangeMessage(ModuleState& state, PLUGIN_STATE_MESSAGE_OP_CODE opCode) {
        op = opCode;
        this->state = state;
    }

    ModuleState state;
    PLUGIN_STATE_MESSAGE_OP_CODE op;
};