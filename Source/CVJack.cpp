/*
  ==============================================================================

    CVJack.cpp
    Created: 6 Apr 2025 4:31:38pm
    Author:  Tom

  ==============================================================================
*/

#include "CVJack.h"
#include "PatchCableManager.h"


CVJack::CVJack(CVJackType type) {
    jackType = type;
    if (jackType == CVInput) {
        cable.setBounds(0, 0, 0, 0);
        cable.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(cable);
    }
}

void CVJack::mouseDown(const juce::MouseEvent& e) {
    auto cableManager = PatchCableManager::getInstance();

    if (!cableManager->isDraggingCable()) {
        cableManager->setDraggedCable(this);
        return;
    }
}

void CVJack::mouseUp(const juce::MouseEvent& e) {
    auto cableManager = PatchCableManager::getInstance();
    cableManager->clearDraggedCable();

    CVJack* source = this;

    // mouseUp runs on the originally clicked CVJack. 
    // we have to use getComponentAt to see if our mouseUp event is over another CVJack
    Component* editor = getParentComponent()->getParentComponent();
    auto event = e.getEventRelativeTo(editor);
    Component* componentUnderMouse = editor->getComponentAt(event.getPosition());
    CVJack* target = dynamic_cast<CVJack*>(componentUnderMouse);
    if (!target) {
        return;
    }

    if (source == target) {
        clearConnection();
    }

    if (source->getType() == target->getType()) {
        return;
    }

    CVJack* input, * output;
    if (target->getType() == CVInput) {
        input = target;
        output = source;
    }
    else {
        input = source;
        output = target;
    }
    output->clearConnection();
    input->clearConnection();
    output->setConnection(input);
    input->setConnection(output);
}

void CVJack::mouseDrag(const juce::MouseEvent& e) {
    auto cableManager = PatchCableManager::getInstance();
    cableManager->updateDraggedCablePosition(e);
}

juce::Line<float> CVJack::getPatchLine() {
    if (isConnected()) {
        return juce::Line<float>(
            getScreenPosition().getX(),
            getScreenPosition().getY(),
            connection->getScreenPosition().getX(),
            connection->getScreenPosition().getY()
        );
    }
    else {
        return juce::Line<float>(-1.0, -1.0, -1.0, -1.0);
    }
}

void CVJack::paint(juce::Graphics& g) {
    float scale = 0.8;
    auto w = getWidth() * scale;
    auto h = getHeight() * scale;
    g.fillEllipse(
        (getWidth() - w) / 2,
        (getHeight() - h) / 2,
        w,
        h
    );
}