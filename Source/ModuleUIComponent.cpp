/*
  ==============================================================================

    Module.cpp
    Created: 5 Apr 2025 7:25:39pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleUIComponent.h"

void ModuleUIComponent::mouseDown(const MouseEvent& e) {
    myDragger.startDraggingComponent(this, e);
    moduleGrid->yankModule(id);
}

void ModuleUIComponent::mouseUp(const MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        // right click
        PopupMenu m;
        m.addItem(1, "delete");
        m.showMenuAsync(PopupMenu::Options(),
            [this](int result)
            {
                if (result == 1)
                {
                    moduleGrid->removeModule(getId());

                    for (auto cvIn : cvIns) {
                        if (cvIn->isConnected()) {
                            auto other = cvIn->getConnection();
                            cvIn->clearConnection();
                            other->clearConnection();
                            stateWriter->removePatchCable(
                                getId(), 
                                cvIn->getId(), 
                                other->getModuleId(), 
                                other->getId()
                            );
                        }
                    }
                    for (auto cvOut : cvOuts) {
                        if (cvOut->isConnected()) {
                            auto other = cvOut->getConnection();
                            cvOut->clearConnection();
                            other->clearConnection();
                            stateWriter->removePatchCable(
                                other->getModuleId(),
                                other->getId(),
                                getId(),
                                cvOut->getId()
                            );
                        }
                    }

                    stateWriter->deleteModule(getId());

                    delete this;
                }
            });
    }
    else {
        // left click
        moduleGrid->placeModule(id, getBounds());
        stateWriter->moveModule(getId(), getBounds());
    }
    
}

void ModuleUIComponent::mouseDrag(const MouseEvent& e) {
    myDragger.dragComponent(this, e, &moduleDragRules);

    for (auto child : getChildren()) {
        auto jack = dynamic_cast<CVJack*>(child);
        if (jack) {
            jack->refreshCablePosition();
            if (jack->isConnected()) {
                jack->getConnection()->refreshCablePosition();
            }
        }
    }
}
