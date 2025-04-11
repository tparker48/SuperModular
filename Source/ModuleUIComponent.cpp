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
                if (result == 0)
                {
                    // nothing
                }
                else if (result == 1)
                {
                    moduleGrid->yankModule(getId());

                    // clear patch cable connections
                    for (auto child : getChildren()) {
                        auto jack = dynamic_cast<CVJack*>(child);
                        if (jack) {
                            if (jack->isConnected()) {
                                auto other = jack->getConnection();
                                jack->clearConnection();
                                other->clearConnection();
                            }
                        }
                    }
                    delete this;
                }
            });
    }
    else {
        // left click
        moduleGrid->placeModule(id, this->getBounds());
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
