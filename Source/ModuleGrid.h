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
    bool moduleIsPlaced(MODULE_ID id);
    void placeModule(MODULE_ID id, ModuleBounds bounds);
    void yankModule(MODULE_ID id);
    ModuleBounds closestAvailablePosition(ModuleBounds bounds);

private:
    bool isOverlap(ModuleBounds bounds);

    std::map<MODULE_ID, ModuleBounds> placedModules;

    int rackHeight = -1;
    int hpWidth = -1;
    int numRacks = -1;
    int hpPerRack = -1;
};