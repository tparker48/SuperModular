/*
  ==============================================================================

    SharedPluginState.cpp
    Created: 12 Apr 2025 7:13:55pm
    Author:  Tom

  ==============================================================================
*/

#include "SharedPluginState.h"

void SharedPluginState::send_updates(std::vector<StateChangeMessage> messages) {
    auto writeIdx = getWriteIdx();

    // push messages to the no-read buffer
    messageBuffers[writeIdx].insert(
        messageBuffers[writeIdx].end(),
        messages.begin(),
        messages.end());

    auto flagToWaitFor = writeIdx;
    auto flagToWrite = (~writeIdx) & 0b00000001;
    while (!shareFlags.compareAndSetBool(flagToWrite, flagToWaitFor)) {}
}

void SharedPluginState::send_update(StateChangeMessage message) {
    std::vector<StateChangeMessage> messages;
    messages.push_back(message);
    send_updates(messages);
}

void SharedPluginState::recieve_updates(std::vector<StateChangeMessage>& messages) {
    startRead();

    // load message from the read buffer and clear it
    auto readIdx = getReadIdx();
    messages = messageBuffers[readIdx];
    messageBuffers[readIdx].clear();

    endRead();
}

void SharedPluginState::writeFullState(PluginState& state) {
    stateLock.lock();
    this->state = state;
    shouldReloadState = true;
    stateLock.unlock();

}
void SharedPluginState::readFullState(PluginState& state) {
    // call this on timer in GUI
    stateLock.lock();
    if (shouldReloadState) {
        state = this->state;
        shouldReloadState = false;
    }
    stateLock.unlock();
}

void SharedPluginState::startRead() {
    shareFlags += 0b00000010;
}
void SharedPluginState::endRead() {
    shareFlags -= 0b00000010;
}
char SharedPluginState::getWriteIdx() {
    return shareFlags.get() & 0b00000001;
}
char SharedPluginState::getReadIdx() {
    return (~shareFlags.get()) & 0b00000001;
}