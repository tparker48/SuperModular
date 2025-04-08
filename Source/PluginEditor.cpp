/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperModularAudioProcessorEditor::SuperModularAudioProcessorEditor (SuperModularAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    double ratio = double(hpPerRow) / (double(numRows) * 5.0);
    double size = hpPerRow * 60.0;
    setSize(size, size / ratio);
    setLookAndFeel(&customLookAndFeel);

    auto cableManager = PatchCableManager::getInstance();
    addAndMakeVisible(cableManager->getDragCable());
}

SuperModularAudioProcessorEditor::~SuperModularAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    PatchCableManager::deleteInstance();
    ModuleGrid::deleteInstance();

    // delete all leftover modules
    std::vector<Module*> modules;
    for (auto child : getChildren()) {
        Module* module = dynamic_cast<Module*>(child);
        if (module) {
            modules.push_back(module);
        }
    }
    for (int i = 0; i < modules.size(); i++) {
        delete modules[i];
    }
}

//==============================================================================
void SuperModularAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));


    for (int i = 0; i < numRows+1; i++) {
        int moduleHeight = getHeight() / numRows;
        int barH = getHeight() / 40;
        int barY = i * moduleHeight;

        g.setColour(juce::Colour(0xff1f1f1f));
        g.fillRect(0, barY, getWidth(), barH);
        g.fillRect(0, barY + moduleHeight - barH, getWidth(), barH);

        if (i < numRows) {
            g.setColour(juce::Colour(0xff050505));
            g.fillRect(0, barY - 1, getWidth(), 2);
        }

        int hpWidth = getWidth() / hpPerRow;
        int holeRadius = getHeight() / 400;
        int holeDiam = holeRadius * 2;
        int holeY = barY + barH / 2 - holeRadius;
        for (int j = 0; j < hpPerRow; j++) {
            g.fillEllipse(j * hpWidth + hpWidth/2, holeY, holeDiam, holeDiam);
            g.fillEllipse(j * hpWidth + hpWidth/2, holeY + moduleHeight - barH, holeDiam, holeDiam);
        }
    }
}

void SuperModularAudioProcessorEditor::resized()
{
    hpWidth = (float)getWidth() / (float)hpPerRow;
    moduleHeight = (float)getHeight() / (float)numRows;

    auto mg = ModuleGrid::getInstance();
    mg->setRackDimensions(numRows, moduleHeight, hpWidth, hpPerRow);
}

void SuperModularAudioProcessorEditor::mouseUp(const juce::MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        showPopupMenu();
    } 
}

void SuperModularAudioProcessorEditor::showPopupMenu() {
    // right click
    juce::PopupMenu m;
    m.addItem(1, "TestModule");
    m.addItem(2, "TestModule2");
    m.showMenuAsync(juce::PopupMenu::Options(),
        [this](int result)
        {
            switch (result) {
            case 1:
                addNewModule<TestModule>();
                break;
            case 2:
                addNewModule<TestModule2>();
                break;
            default:
                break;
            }
        });

}

template <typename M>
void SuperModularAudioProcessorEditor::addNewModule() {
    auto moduleGrid = ModuleGrid::getInstance();
    auto bounds = moduleGrid->closestAvailablePosition(
        ModuleBounds(0, 0, hpWidth * M::hp, moduleHeight));

    if (bounds.getX() != -1) {
        auto newModule = new M();
        newModule->setBounds(bounds);
        moduleGrid->placeModule(newModule->getId(), bounds);
        addAndMakeVisible(newModule);
    }
}