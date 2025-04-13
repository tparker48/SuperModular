/*
  ==============================================================================

    StateChangeMessage.cpp
    Created: 12 Apr 2025 7:16:35pm
    Author:  Tom

  ==============================================================================
*/

#include "StateChangeMessage.h"


StateChangeMessage::StateChangeMessage(ModuleState& state, PLUGIN_STATE_MESSAGE_OP_CODE opCode) {
    op = opCode;
    this->state = state;
}

void StateChangeMessage::applyMessage(PluginState& pluginState) {
    switch (op) {
    case ADD:
        pluginState.moduleStates.push_back(state);
        break;
    case UPDATE:
        std::replace_if(
            pluginState.moduleStates.begin(),
            pluginState.moduleStates.end(),
            [this](ModuleState other) { return state.getId() == other.getId(); },
            state
        );
        break;
    case DELETE:
        std::remove_if(
            pluginState.moduleStates.begin(),
            pluginState.moduleStates.end(),
            [this](ModuleState other) { return state.getId() == other.getId(); }
        );
        break;
    }
}