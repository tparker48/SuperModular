/*
  ==============================================================================

    Module.cpp
    Created: 5 Apr 2025 7:25:39pm
    Author:  Tom

  ==============================================================================
*/

#include "Module.h"

void Module::mouseDown(const juce::MouseEvent& e) {
    myDragger.startDraggingComponent(this, e);
    auto moduleGrid = ModuleGrid::getInstance();
    moduleGrid->yankModule(id);
}

void Module::mouseUp(const juce::MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        // right click
        juce::PopupMenu m;
        m.addItem(1, "delete");
        m.showMenuAsync(juce::PopupMenu::Options(),
            [this](int result)
            {
                if (result == 0)
                {
                    // nothing
                }
                else if (result == 1)
                {
                    auto moduleGrid = ModuleGrid::getInstance();
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
        auto moduleGrid = ModuleGrid::getInstance();
        moduleGrid->placeModule(id, this->getBounds());
    }
    
}

void Module::mouseDrag(const juce::MouseEvent& e) {
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
