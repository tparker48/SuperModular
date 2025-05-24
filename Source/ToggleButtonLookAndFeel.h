#pragma once
#include <JuceHeader.h>

class ToggleButtonLookAndFeel : public LookAndFeel_V4
{
public:
    void drawToggleButton(Graphics& g, ToggleButton& t, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
	
	    // background
	    auto buttonArea = t.getLocalBounds();
	    auto centre = buttonArea.getCentre();
	    buttonArea.setSize(buttonArea.getWidth() * .65, buttonArea.getHeight() * .65);
	    buttonArea.setCentre(centre);


	    g.setColour(Colours::black);
	    g.fillRect(Rectangle<float>(buttonArea.getX()+2.0, buttonArea.getY()+2.0, buttonArea.getWidth(), buttonArea.getHeight()));

	    g.setColour(Colours::white);
	    g.fillRect(buttonArea.toFloat());
	 (Colours::black);
	    g.drawRect(buttonArea.toFloat(), 1.0);

	
	    auto lightUpArea = buttonArea;
	    lightUpArea.setHeight(buttonArea.getHeight() * .3);
	    lightUpArea.setWidth(buttonArea.getHeight() * .60);
	    lightUpArea.setCentre(buttonArea.getCentreX(), buttonArea.getCentreY());

	    if (t.getToggleState()) {
		    g.setColour(Colours::black);
		    g.drawText("ON", buttonArea, Justification::centred);
	    }
	    else {
		    g.setColour(Colours::black);
		    g.drawText("OFF", buttonArea, Justification::centred);
	
	    }
    }

};
