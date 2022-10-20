/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 8 Feb 2022 3:23:58pm
    Author:  Austin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
//initializing
WaveformDisplay::WaveformDisplay(DJAudioPlayer* _player,
                                AudioFormatManager& formatManagerToUse,
                                AudioThumbnailCache& cacheToUse)
                                :player(_player),
                                audioThumb(1000, formatManagerToUse, cacheToUse),
                                fileLoaded(false),
                                position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

//change listener of audiothumb
    audioThumb.addChangeListener(this);
}
//==============================================================================




//==============================================================================
WaveformDisplay::~WaveformDisplay()
{
}
//==============================================================================





//==============================================================================
void WaveformDisplay::paint (juce::Graphics& g)
{
/* This demo code just fills the component's background and
draws some placeholder text to get you started.

You should replace everything in this method with your own
drawing code..
*/

//clear the background
    g.fillAll(Colours::black);

//draw an outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

//waveform colour
    g.setColour (juce::Colours::darkcyan);
    
//checking condition to do something when file loaded
    if (fileLoaded)
    {
//draw thumbnail (refer to draw channel guide)
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f);

//drawing line and arrow for playhead
        g.setColour(Colours::white);
        g.drawLine(position * getWidth(), 0, position * getWidth(), getHeight());
        g.drawArrow(juce::Line<float>(position *getWidth(), 0, position * getWidth(), 10), 20, 20, 10);
        g.drawArrow(juce::Line<float>(position * getWidth(), getHeight(), position * getWidth(), getHeight()-10), 20, 20, 10);

//drawing audio file name on centredTop
        g.setColour(juce::Colours::orange);
        g.drawText(audioName, getLocalBounds(), juce::Justification::centredTop, true);

//drawing instuctions on waveform
        g.setColour(juce::Colours::orange);
        g.drawText("Click on Waveform to play back.", getLocalBounds(), juce::Justification::centredBottom, true);
    }

    else
    {
//drawing file not loaded when no file is loaded
        g.setFont(20.0f);
        g.drawText("File not loaded in Deck...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}
//==============================================================================




//==============================================================================
void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}
//==============================================================================




//==============================================================================
void WaveformDisplay::loadURL(URL audioURL)
{
//debugger
    //DBG("wfd:: loadURL");

//clear old audiothumb
    audioThumb.clear();

//turning audioURL to input source and setting it
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

//checking condition that file is loaded or not
    if (fileLoaded)
    {
        //DBG("wfd: loaded!");
        audioName = audioURL.getFileName();
        repaint();
    }

    else
    {
        //DBG("wfd:: not loaded!");
    }
}
//==============================================================================




//==============================================================================
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    //DBG("wfd:: change recieved!");

//repainting gui
    repaint();
}
//==============================================================================




//==============================================================================
void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position && !isnan(pos))
    {
//storing of location
        position = pos;

//repainting gui
        repaint();
    }
}
//==============================================================================




//==============================================================================
void WaveformDisplay::mouseDown(const MouseEvent& e)
{
//when mouse is down, condiiton becomes true
    isDown = true;

//function call to mouse dragged
    mouseDrag(e);

    //DBG("Mouse is down");
}
//==============================================================================




//==============================================================================
void WaveformDisplay::mouseUp(const MouseEvent& e)
{
//when mouse is up, condition for mouse down becomes false
    isDown = false;
    //DBG("Mouse is up");
}
//==============================================================================




//==============================================================================
void WaveformDisplay::mouseDrag(const MouseEvent& e)
{
//set range for calculation
    visibleRange.setStart(0.0);
    visibleRange.setEnd(1.0);

//setting possition
    player->setPositionRelative(jmax(0.0, xToTime((float)e.x)));

    //DBG("Mouse is dragged");
}
//==============================================================================




//==============================================================================
double WaveformDisplay::xToTime(const float x) const
{
//calculation for position
    double cursPosition = (x / (float)getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();

    //DBG(cursPosition);

//returning position
    return cursPosition;
}
//==============================================================================