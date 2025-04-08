/*
  ==============================================================================

    ModuleGrid.h
    Created: 5 Apr 2025 7:28:57pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


typedef juce::Rectangle<int> ModuleBounds;
typedef int MODULE_ID;

static MODULE_ID nextModuleId = 0;

class ModuleGrid {
public:
    JUCE_DECLARE_SINGLETON(ModuleGrid, true);

    ModuleGrid() {}
    ~ModuleGrid() { clearSingletonInstance(); }

    void setRackDimensions(int rackCount, int moduleHeight, int hpSize, int hpPerRow);
    bool moduleIsPlaced(MODULE_ID id);
    void placeModule(MODULE_ID id, ModuleBounds bounds);
    void yankModule(MODULE_ID id);
    ModuleBounds closestAvailablePosition(ModuleBounds bounds);

private:
    bool isOverlap(ModuleBounds bounds);

    std::map<MODULE_ID, juce::Rectangle<int>> placedModules;

    int rackHeight = -1;
    int hpWidth = -1;
    int numRacks = -1;
    int hpPerRack = -1;
};