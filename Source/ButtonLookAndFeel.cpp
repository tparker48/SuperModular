#include "ButtonLookAndFeel.h"

void ButtonLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& t, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
	
	// background
	auto buttonArea = t.getLocalBounds();
	auto centre = buttonArea.getCentre();
	buttonArea.setSize(buttonArea.getWidth() * .65, buttonArea.getHeight() * .65);
	buttonArea.setCentre(centre);

	g.setColour(dark);
	g.fillRect(buttonArea);


	auto lightUpArea = buttonArea;
	
	
	if (t.getToggleState()) {
		lightUpArea.setHeight(buttonArea.getHeight() * .15);
		lightUpArea.setWidth(buttonArea.getHeight() * .60);
		lightUpArea.setCentre(buttonArea.getCentreX(), buttonArea.getCentreY() - buttonArea.getHeight() * .17);
	}
	else {
		lightUpArea.setHeight(buttonArea.getHeight() * .15);
		lightUpArea.setWidth(buttonArea.getHeight() * .40);
		lightUpArea.setCentre(buttonArea.getCentreX(), buttonArea.getCentreY() + buttonArea.getHeight() * .17);
	}
	g.setColour(light);
	g.fillRect(lightUpArea);
	
}
