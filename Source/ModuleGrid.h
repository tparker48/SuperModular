/*
  ==============================================================================

    ModuleGrid.h
    Created: 5 Apr 2025 7:28:57pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


typedef Rectangle<int> ModuleBounds;
typedef int MODULE_ID;

class ModuleGrid {
public:
    ModuleGrid() {}
    ~ModuleGrid() {}

    void setRackDimensions(int rackCount, int moduleHeight, int hpSize, int hpPerRow);
    
    bool addModule(MODULE_ID id, Component* module);
    void removeModule(MODULE_ID id);
    Component* getModule(MODULE_ID id);

    bool moduleIsPlaced(MODULE_ID id);
    void placeModule(MODULE_ID id, ModuleBounds bounds);
    void yankModule(MODULE_ID id);

    bool moduleExists(MODULE_ID id) { return modules.find(id) != modules.end();  }

    void clearAllModules();

    ModuleBounds closestAvailablePosition(ModuleBounds bounds);

private:
    bool isOverlap(ModuleBounds bounds);

    std::map<MODULE_ID, Component*> modules;
    std::map<MODULE_ID, bool> placed;

    int rackHeight = -1;
    int hpWidth = -1;
    int numRacks = -1;
    int hpPerRack = -1;
};