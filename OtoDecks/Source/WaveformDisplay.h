/*
  ==============================================================================

    WaveformDisplay.h
    Created: 8 Feb 2022 3:23:58pm
    Author:  Austin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
                        public ChangeListener
{
public:
    WaveformDisplay(DJAudioPlayer* player,
                    AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();

    void paint (juce::Graphics&) override;
    void resized() override;

//change listener for painting GUI
    void changeListenerCallback(ChangeBroadcaster* source) override;

//loading URL of audio file
    void loadURL(URL audioURL);

//set relative position of playhead
    void setPositionRelative(double pos);

//mouse events
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

//declaration for calculation used in mouse events
    double xToTime(const float x) const;

//by default set mouse down to be false
    bool isDown{ false };

private:
//loading audio thumbnail
    AudioThumbnail audioThumb;

//declaring fileloaded
    bool fileLoaded;

//creating a local variable to store position
    double position;
    juce::String audioName;

//pointer to DJAudioPlayer in DeckGUI
    DJAudioPlayer* player;
    
//declaring range for calculation
    Range<double> visibleRange;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
