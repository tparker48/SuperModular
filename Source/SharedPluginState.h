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

   The class does this in the following way:
    - under the hood, there are actually two state objects
    - one object is readable, one object is writable
    - an atomic char is maintained as well,  first 2 bits are used as flags. 
    - Bit 0 denotes which object is for reading, and which is for writing
    - Bit 1 is set to 1 when a read is taking place, and 0 otherwise
    
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
