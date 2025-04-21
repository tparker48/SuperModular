#pragma once

#include <JuceHeader.h>

class Joystick;

class JoystickListener
{
public:
    virtual ~JoystickListener() {}

    virtual void joystickValueChanged(Joystick *joystick) = 0;
};

class Joystick : public Component
{
public:
    Joystick();
    ~Joystick();

    void setRanges(const double newHorizontalMinimum,
                   const double newHorizontalMaximum,
                   const double newVerticalMinimum,
                   const double newVerticalMaximum,
                   const double newInterval = 0);

    void setValues(double newHorizontalValue,
                   double newVerticalValue,
                   const bool sendUpdateMessage = true,
                   const bool sendMessageSynchronously = true);

    void setXValue(double newXValue,
                   const bool sendUpdateMessage = true,
                   const bool sendMessageSynchronously = true);

    void setYValue(double newYValue,
                   const bool sendUpdateMessage = true,
                   const bool sendMessageSynchronously = true);

    double getHorizontalValue() const throw() { return current_x + x_min; }
    double getVerticalValue() const throw() { return ((y_max - y_min) - current_y) + y_min; }
    double getHorizontalMaximum() const throw() { return x_max; }
    double getHorizontalMinimum() const throw() { return x_min; }
    double getVerticalMaximum() const throw() { return y_max; }
    double getVerticalMinimum() const throw() { return y_min; }

    const String getCurrentValueAsString();

    void addListener(JoystickListener *const listener);
    void removeListener(JoystickListener *const listener);

    virtual void startedDragging();
    virtual void stoppedDragging();

    virtual void valueChanged(double newHorizontalValue,
                              double newVerticalValue);
    void setBackgroundColour(const Colour &newBackgroundColour)
    {
        backgroundColour = newBackgroundColour;
    }

    void setPadColour(const Colour &newPadColour)
    {
        padColour = newPadColour;
    }

    void setDotColour(const Colour &newDotColour)
    {
        dotColour = newDotColour;
    }

    void setDotRadius(int newDotRadius)
    {
        dot_radius = newDotRadius;
    }

    void paint(Graphics &g);
    void mouseDown(const MouseEvent &e);
    void mouseDrag(const MouseEvent &e);
    void mouseUp(const MouseEvent &e);
    void mouseMove(const MouseEvent &e);
    bool keyStateChanged(const bool keyDown);
    void resized();

juce_UseDebuggingNewOperator

    private : void
              calculateRatio();
    void calculateDrawingSpot();
    void sendChanges();
    void restoreMouseIfHidden();
    const String getTextFromValue(double v);

    double current_x, current_y;
    double x_min, x_max, y_min, y_max;
    double interval;
    double x_snap, y_snap;
    Colour backgroundColour, padColour, dotColour;

    bool mouseWasHidden;

    int numDecimalPlaces;
    int draw_x, draw_y;
    int startPressX, startPressY;
    float x_ratio, y_ratio;
    int dot_radius;

    Array<void *> listeners;

    MouseInputSource mouseInputSource;
};