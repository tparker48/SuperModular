/*
  ==============================================================================

    CircularMessageBuffer.h
    Created: 9 Apr 2025 6:25:46pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginState.h"
#include "StateChangeMessage.h"

const int MESSAGE_BUFFER_SIZE = 256;

class CircularMessageBuffer {
public:
    void consumeMessages(std::vector<StateChangeMessage>& messages) {
        int i = readPtr.get();
        while (i != writePtr.get()) {
            messages.push_back(messageBuffer[i]);
            i = (i + 1) % MESSAGE_BUFFER_SIZE;
        }
        readPtr.set(i);
    }

    void sendMessages(std::vector<StateChangeMessage> messages) {
        int writePos;
        for (int i = 0; i < messages.size(); i++) {
            writePos = writePtr.get();
            if (writePos == (readPtr.get() + MESSAGE_BUFFER_SIZE - 1) % MESSAGE_BUFFER_SIZE) {
                // full buffer, drop messages!
                break;
            }
            messageBuffer[writePos] = messages[i];
            writePos = (writePos + 1) % MESSAGE_BUFFER_SIZE;
            writePtr = writePos;

        }
    }
    void sendMessage(StateChangeMessage message) {
        sendMessages(std::vector<StateChangeMessage>{message});
    }

private:
    StateChangeMessage messageBuffer[MESSAGE_BUFFER_SIZE];
    Atomic<unsigned char> readPtr = 0;
    Atomic<unsigned char> writePtr = 0;
};


class SharedState {
public:
    void writeFullState(PluginState& state, bool flagReload = false) {
        stateLock.lock();
        this->state = state;
        if (flagReload) shouldReloadState = true;
        stateLock.unlock();
    }

    bool readFullState(PluginState& state, bool checkForNewState = false) {
        bool stateWasRead = false;

        stateLock.lock();

        if (!checkForNewState || shouldReloadState) {
            state = this->state;
            shouldReloadState = false;
            stateWasRead = true;
        }
        
        stateLock.unlock();
        return stateWasRead;
    }

protected:
    PluginState state;
    std::mutex stateLock;
    bool shouldReloadState = false;
};