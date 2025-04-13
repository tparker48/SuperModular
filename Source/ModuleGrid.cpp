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

bool ModuleGrid::addModule(MODULE_ID id, Component* module) {
    auto bounds = closestAvailablePosition(module->getBounds());
    if (bounds.getX() != -1) {
        module->setBounds(bounds);
        modules[id] = module;
        placed[id] = true;
        return true;
    }
    return false;
}

void ModuleGrid::removeModule(MODULE_ID id) {
    modules.erase(id);
    placed.erase(id);
}

Component* ModuleGrid::getModule(MODULE_ID id) {
    if (modules.find(id) != modules.end()) {
        return modules[id];
    }
    else {
        return nullptr;
    }
}

void ModuleGrid::placeModule(MODULE_ID id, ModuleBounds bounds) {
    if (!isOverlap(bounds) && (!moduleIsPlaced(id))) {
        placed[id] = true;
        modules[id]->setBounds(bounds);
    }
}

void ModuleGrid::yankModule(MODULE_ID id) {
    if (placed.find(id) != placed.end()) {
        placed[id] = false;
    }
}

bool ModuleGrid::moduleIsPlaced(MODULE_ID id) {
    return (placed.find(id) != placed.end() && placed[id]==true);
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
    for (const auto& pair : modules) {
        if (placed[pair.first] == true) {
            if (pair.second->getBounds().intersects(bounds)) {
                return true;
            }
        }
    }
    return false;
}