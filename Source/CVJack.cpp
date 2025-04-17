/*
  ==============================================================================

    CVJack.cpp
    Created: 6 Apr 2025 4:31:38pm
    Author:  Tom

  ==============================================================================
*/

#include "CVJack.h"


CVJack::CVJack(CVJackType type, int id, int parentId, PatchCableManager* cm, SharedStateWriter* stateWriterPtr):
        stateWriter(stateWriterPtr) {
    cableManager = cm;
    jackId = id;
    moduleId = parentId;
    jackType = type;
    if (jackType == CVInput) {
        cable.setBounds(0, 0, 0, 0);
        cable.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(cable);
    }
}

void CVJack::mouseDown(const MouseEvent& e) {
    if (!cableManager->isDraggingCable()) {
        cableManager->setDraggedCable(this);
        return;
    }
}

void CVJack::mouseUp(const MouseEvent& e) {
    cableManager->clearDraggedCable();

    CVJack* source = this;

    // mouseUp runs on the originally clicked CVJack. 
    // we have to use getComponentAt to see if our mouseUp event is over another CVJack
    Component* editor = getParentComponent()->getParentComponent();
    auto event = e.getEventRelativeTo(editor);
    Component* componentUnderMouse = editor->getComponentAt(event.getPosition());
    CVJack* target = dynamic_cast<CVJack*>(componentUnderMouse);

    if (!target) {
        // did not end drag on another CV jack
        return;
    }
    if (source == target) {
        // started and ended on same CV jack
        if (auto other = getConnection()) {
            if (getType() == CVInput) {
                stateWriter->removePatchCable(
                    getModuleId(),
                    getId(),
                    other->getModuleId(),
                    other->getId());
            }
            else {
                stateWriter->removePatchCable(
                    other->getModuleId(),
                    other->getId(),
                    getModuleId(),
                    getId());
            }

            clearConnection();
            return;
        }
    }
    if (source->getType() == target->getType()) {
        // invalid CV jack connection (input to input) (output to output)
        return;
    }
    if (source->getConnection() && 
        source->getConnection() == target) {
        // input and output jacks are already connected
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
    
    // Update Shared State
    if (input->connection) {
        stateWriter->removePatchCable(input->getModuleId(), input->getId(), input->connection->getModuleId(), input->connection->getId());
    }
    if (output->connection) {
        stateWriter->removePatchCable(output->connection->getModuleId(), output->connection->getId(), output->getModuleId(), output->getId());
    }
    stateWriter->addPatchCable(input->getModuleId(), input->getId(), output->getModuleId(), output->getId());

    // Update UI state
    output->clearConnection();
    input->clearConnection();
    output->setConnection(input);
    input->setConnection(output);


}

void CVJack::mouseDrag(const MouseEvent& e) {
    cableManager->updateDraggedCablePosition(e);
}

Line<float> CVJack::getPatchLine() {
    if (isConnected()) {
        return Line<float>(
            getScreenPosition().getX(),
            getScreenPosition().getY(),
            connection->getScreenPosition().getX(),
            connection->getScreenPosition().getY()
        );
    }
    else {
        return Line<float>(-1.0, -1.0, -1.0, -1.0);
    }
}

void CVJack::paint(Graphics& g) {
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