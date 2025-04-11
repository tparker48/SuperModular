/*
  ==============================================================================

    ModuleGrid.cpp
    Created: 5 Apr 2025 7:28:57pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleGrid.h"

void ModuleGrid::setRackDimensions(int rackCount, int moduleHeight, int hpSize, int hpPerRow) {
    numRacks = rackCount;
    rackHeight = moduleHeight;
    hpWidth = hpSize;
    hpPerRack = hpPerRow;
}

bool ModuleGrid::moduleIsPlaced(MODULE_ID id) {
    return (placedModules.find(id) != placedModules.end());
}

void ModuleGrid::placeModule(MODULE_ID id, ModuleBounds bounds) {
    if (!isOverlap(bounds) && (!moduleIsPlaced(id))) {
        placedModules[id] = bounds;
    }
}

void ModuleGrid::yankModule(MODULE_ID id) {
    if (placedModules.find(id) != placedModules.end()) {
        placedModules.erase(id);
    }
}

ModuleBounds ModuleGrid::closestAvailablePosition(ModuleBounds bounds) {
    // iteratively check all possible positions
    std::vector<ModuleBounds> possibleBounds;
    int max_x = (hpWidth * hpPerRack) - bounds.getWidth();
    int max_y = numRacks * rackHeight;
    for (int x = 0; x <= max_x; x+=hpWidth) {
        for (int y = 0; y < max_y; y+=rackHeight) {
            auto testBounds = ModuleBounds(x, y, bounds.getWidth(), bounds.getHeight());
            if (!isOverlap(testBounds)) {
                possibleBounds.push_back(testBounds);
            }
        }
    }

    if (possibleBounds.empty()) {
        return ModuleBounds(-1, -1, -1, -1);
    }

    // sort by shortest distance
    auto target = bounds.getCentre();
    std::sort(possibleBounds.begin(), possibleBounds.end(), [this, target](ModuleBounds a, ModuleBounds b) {
        auto aDist = a.getCentre().getDistanceFrom(target);
        auto bDist = b.getCentre().getDistanceFrom(target);
        return aDist < bDist;
     });

    return possibleBounds[0];
}

bool ModuleGrid::isOverlap(ModuleBounds bounds) {
    // check if bounds is overlapping an existing module
    for (const auto& pair : placedModules) {
        if (pair.second.intersects(bounds)) {
            return true;
        }
    }
    return false;
}