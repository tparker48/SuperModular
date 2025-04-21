// Joystick Component based on kunitoki's juced project
// see original @ https://github.com/kunitoki/juced/blob/master/juce/src/extended/controls/

#include <JuceHeader.h>
#include "Joystick.h"


Joystick::Joystick()
  : Component ("Joystick"),
    current_x (0),
    current_y (0),
    x_min (0),
    x_max (1),
    y_min (0),
    y_max (1),
    backgroundColour (Colours::black),
    padColour(Colours::grey),
    dotColour(Colours::white),
    dot_radius(9),
    numDecimalPlaces (4),
    mouseInputSource (Desktop::getInstance().getMainMouseSource())
{
    setOpaque (true);
    setWantsKeyboardFocus (true);

    calculateRatio();
}

Joystick::~Joystick()
{
    deleteAllChildren();
}


void Joystick::resized()
{
    x_ratio = getWidth() / (float) (x_max - x_min);
    y_ratio = getHeight() / (float) (y_max - y_min);

    calculateDrawingSpot();
}


const String Joystick::getCurrentValueAsString()
{
    String text;
    text << getTextFromValue (current_x+x_min) << ", " << getTextFromValue (((y_max-y_min)-current_y)+y_min) ;
    return text;
}


void Joystick::setRanges (const double newHorizontalMin,
                          const double newHorizontalMax,
                          const double newVerticalMin,
                          const double newVerticalMax,
                          const double newInt)
{
    bool horizontalChanged = false;
    bool verticalChanged = false;

    if (x_min != newHorizontalMin || x_max != newVerticalMax)
    {
        x_min = newHorizontalMin;
        x_max = newHorizontalMax;
        horizontalChanged = true;
    }

    if (y_min != newVerticalMin || y_max != newVerticalMax)
    {
        y_min = newVerticalMin;
        y_max = newVerticalMax;
        verticalChanged = true;
    }

    interval = newInt;
    numDecimalPlaces = 7;
    if (newInt != 0)
    {
        int v = abs ((int) (newInt * 10000000));
        while ((v % 10) == 0)
        {
            --numDecimalPlaces;
            v /= 10;
        }
    }

    if (horizontalChanged || verticalChanged)
        setValues (current_x, current_y, false);

    calculateRatio();
    calculateDrawingSpot();
}

void Joystick::setValues (double newHorizontalValue, double newVerticalValue,
                          const bool sendUpdateMessage,
                          const bool sendMessageSynchronously)
{
    if (newHorizontalValue <= x_min || x_max <= x_min)
    {
        newHorizontalValue = x_min;
    }
    else if (newHorizontalValue >= x_max)
    {
        newHorizontalValue = x_max;
    }
    else if (interval > 0)
    {
        newHorizontalValue = x_min + interval * floor ((newHorizontalValue - x_min) / interval + 0.5);
    }

    if (newVerticalValue <= y_min || y_max <= y_min)
    {
        newVerticalValue = y_min;
    }
    else if (newVerticalValue >= y_max)
    {
        newVerticalValue = y_max;
    }
    else if (interval > 0)
    {
        newVerticalValue = y_min + interval * floor ((newVerticalValue - y_min) / interval + 0.5);
    }   

    if (current_x != newHorizontalValue || current_y != newVerticalValue)
    {
        current_x = newHorizontalValue;
        current_y = newVerticalValue;
        if (sendUpdateMessage)
        {
            if (sendMessageSynchronously)
                sendChanges();
            else
                jassertfalse; // asyncronous message not implemented !

            valueChanged (current_x, current_y);
        }
    }
    calculateDrawingSpot();
    repaint();
}


void Joystick::setXValue(double newXValue,
    const bool sendUpdateMessage,
    const bool sendMessageSynchronously)
{
    setValues(newXValue, current_y, sendUpdateMessage, sendMessageSynchronously);
}


void Joystick::setYValue(double newYValue,
    const bool sendUpdateMessage,
    const bool sendMessageSynchronously)
{
    setValues(current_x, y_max - newYValue, sendUpdateMessage, sendMessageSynchronously);
}


void Joystick::paint (Graphics& g)
{   
    // clear
    g.setColour(backgroundColour);
    g.fillAll ();

    // pad
    g.setColour (padColour);
    g.fillRoundedRectangle(Rectangle<float>(0.0f, 0.0f, getWidth(), getHeight()), 8.0f);

    // point
    //Image point = ImageCache::getFromMemory(BinaryData::cross_png, BinaryData::cross_pngSize);
    //g.drawImage(point, Rectangle<float>(draw_x - dot_radius / 2, draw_y - dot_radius / 2, dot_radius, dot_radius));
    g.setColour(dotColour);
    g.fillEllipse(Rectangle<float>(draw_x - dot_radius / 2, draw_y - dot_radius / 2, dot_radius, dot_radius));


    // cover the point at the border
    g.setColour(backgroundColour);
    g.drawRoundedRectangle(Rectangle<float>(-2.0f, -2.0f, getWidth()+4, getHeight()+4), 8.0f, 3.0f);
}


void Joystick::mouseDown(const MouseEvent& e)
{
    mouseWasHidden = false;

    if (!isEnabled()) return;

    if (e.mods.isLeftButtonDown()
             || e.mods.isMiddleButtonDown())
    {
        startPressX = e.x;
        startPressY = e.y;

        mouseDrag (e);
    }

    grabKeyboardFocus();   
}

void Joystick::mouseDrag (const MouseEvent& e)
{
    if (!isEnabled()) return;
    
    // If shift is down, don't allow changes to the X value
    if (! e.mods.isShiftDown())
    {
        int x = startPressX + e.getDistanceFromDragStartX();
        if (x < 0)
            current_x = 0;
        else if (x >= getWidth())
            current_x = x_max - x_min;
        else
            current_x = x / x_ratio;
    }

    // If ctrl is down, don't allow changes to Y value
    if (! e.mods.isCtrlDown())
    {
        int y = startPressY + e.getDistanceFromDragStartY();
        if (y < 0)
            current_y = 0;
        else if (y > getHeight())
            current_y = y_max - y_min;
        else
            current_y = y / y_ratio;
    }

    valueChanged (current_x, current_y);
    sendChanges ();
    calculateDrawingSpot();
    repaint ();
}


void Joystick::mouseUp (const MouseEvent& e)
{
    if (!isEnabled()) return;
    
    valueChanged (current_x, current_y);
    sendChanges ();
    calculateDrawingSpot();
    restoreMouseIfHidden();
    repaint ();
}

void Joystick::mouseMove (const MouseEvent& e) {}


bool Joystick::keyStateChanged(const bool keyDown)
{
    return Component::keyStateChanged(keyDown);
}


void Joystick::addListener (JoystickListener* const listener)
{
    jassert (listener != 0);
    if (listener != 0)
        listeners.add (listener);
}

void Joystick::removeListener (JoystickListener* const listener)
{
    listeners.removeFirstMatchingValue(listener);
}

void Joystick::sendChanges()
{
    for (int i = listeners.size (); --i >= 0;)
        ((JoystickListener*) listeners.getUnchecked (i))->joystickValueChanged (this);
}


void Joystick::calculateRatio()
{
    // Calculate the x and y ratio
    x_ratio = getWidth() / (float) (x_max - x_min);
    y_ratio = getHeight() / (float) (y_max - y_min);
}


void Joystick::calculateDrawingSpot()
{
    // Determine the current drawing location
    draw_x = roundDoubleToInt (floor (current_x * x_ratio));
    draw_y = roundDoubleToInt (floor (current_y * y_ratio));

    // If at max value, don't go over edge
    if (current_x == x_min) draw_x++;
    if (current_y == y_min) draw_y++;
    if (current_x == (x_max - x_min)) draw_x--;
    if (current_y == (y_max - y_min)) draw_y--;
}

const String Joystick::getTextFromValue (double v)
{
    if (numDecimalPlaces > 0)
        return String (v, numDecimalPlaces);
    else
        return String (roundDoubleToInt (v));
}


void Joystick::restoreMouseIfHidden()
{
    if (mouseWasHidden)
    {
        mouseWasHidden = false;

        Component* c = mouseInputSource.getComponentUnderMouse();
        if (c == 0)
            c = this;

        Desktop::setMousePosition (Point<int>(getScreenX() + draw_x,
                                   getScreenY() + draw_y));
    }
}


void Joystick::startedDragging()
{
}

void Joystick::stoppedDragging()
{
}

void Joystick::valueChanged (double x, double y)
{
}
