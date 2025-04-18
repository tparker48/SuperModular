/*
  ==============================================================================

    SharedPluginState.h
    Created: 9 Apr 2025 6:25:46pm
    Author:  Tom

  ==============================================================================

  This class is responsible for: 
   - Storing the full state of the plugin. All modules, module coordinates, patch 
   connections.
   - Allowing the processor thread and a single editor thread to both access this
   state in a thread-safe manner, without locking the processor thread. (Locking
   the editor thread for very short periods is fine).

    NOTE: design assumes only 1 processor (reader) and 1 editor (writer)
    NOTE: design assumes underlying architecture supports non-locking atomic chars,
        otherwise locking may take place
*/

#pragma once

#include <JuceHeader.h>
#include "PluginState.h"
#include "StateChangeMessage.h"


class SharedPluginState {
public:
    void send_updates(std::vector<StateChangeMessage> messages);
    void send_update(StateChangeMessage message);
    void switchWriteIdx();
    void recieve_updates(std::vector<StateChangeMessage>& messages);

    void writeFullState(PluginState& state, bool flagReload = false);
    void readFullState(PluginState& state);
    bool readFullStateIfNew(PluginState& state);

private:
    Atomic<char> shareFlags = 0b00000000;
    std::vector<StateChangeMessage> messageBuffers[2];
    std::mutex stateLock;
    bool shouldReloadState = false;
    PluginState state;

    void startRead();
    void endRead();
    char getWriteIdx();
    char getReadIdx();
};
