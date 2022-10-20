/*
  ==============================================================================

    DeckGUI.cpp
    Created: 7 Feb 2022 9:09:10pm
    Author:  Austin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse)
    : player(_player),
    waveformDisplay(player, formatManagerToUse, cacheToUse)
{
//buttons
//play/stop toggle button
    stopButton.setToggleState(true, NotificationType::dontSendNotification);
    stopButton.onClick = [this]() { stop(); };

//loop/noloop toggle button
    loopButton.setToggleState(true, NotificationType::dontSendNotification);
    loopButton.onClick = [this]() { noloop(); };

//buttons to add to gui to be displayed
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

//adding event listener to buttons
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);

//volume slider
    addAndMakeVisible(volSlider);
    volSlider.setTextValueSuffix (" Hz");

//volume label
    addAndMakeVisible(volLabel);
    volLabel.setText("Gain: ", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

//speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.setTextValueSuffix(" x");

//speed label
    addAndMakeVisible(spdLabel);
    spdLabel.setText("Speed: ", juce::dontSendNotification);
    spdLabel.attachToComponent(&speedSlider, true);

//cutOff slider
    addAndMakeVisible(cutOffSlider);
    cutOffSlider.setTextValueSuffix(" Hz");

//cutOff label
    addAndMakeVisible(cutOffLabel);
    cutOffLabel.setText("Cut Off: ", juce::dontSendNotification);
    cutOffLabel.attachToComponent(&cutOffSlider, true);

//qSlider
    addAndMakeVisible(qSlider);

//q label
    addAndMakeVisible(qLabel);
    qLabel.setText("Resonance: ", juce::dontSendNotification);
    qLabel.attachToComponent(&qSlider, true);

//adding event listener to sliders
    volSlider.addListener(this);
    speedSlider.addListener(this);
    cutOffSlider.addListener(this);
    qSlider.addListener(this);

//setting parameters for volume slider
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(1);
    volSlider.setNumDecimalPlacesToDisplay(2);

//setting parameters for speed slider
    speedSlider.setRange(0.01, 100.0);
    speedSlider.setValue(1);
    speedSlider.setNumDecimalPlacesToDisplay(2);

//setting parameters for cutoff slider
    cutOffSlider.setRange(20.0, 20000.0);
    cutOffSlider.setSkewFactor(0.5);
    cutOffSlider.setValue(440.0);
    cutOffSlider.setNumDecimalPlacesToDisplay(2);

//setting parameters for resonance slider
    qSlider.setRange(0.3, 20.0);
    qSlider.setSkewFactor(0.5);
    qSlider.setValue(1.0 / std::sqrt(2.0));
    qSlider.setNumDecimalPlacesToDisplay(2);

//waveform to add to gui to be displayed
    addAndMakeVisible(waveformDisplay);

//setting parameters combobox for pass type
    addAndMakeVisible(passToDeckCombo);
    passToDeckCombo.addItem("LOW-PASS", 1);
    passToDeckCombo.addItem("HIGH-PASS", 2);
    passToDeckCombo.addItem("BAND-PASS", 3);
    passToDeckCombo.setSelectedId(3);
    passToDeckCombo.onChange = [this] { typeParams(); };

//starting timer for every 200 miliseconds
    startTimer(200);

}
//==============================================================================




//==============================================================================
DeckGUI::~DeckGUI()
{
//stop timer
    stopTimer();
}
//==============================================================================




//==============================================================================
void DeckGUI::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

//clear the background
    g.fillAll(Colours::black);
    g.setColour(juce::Colours::black);

//display customization
    stopButton.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::black);
    stopButton.setColour(TextButton::ColourIds::buttonColourId, juce::Colours::black);
    loadButton.setColour(TextButton::ColourIds::buttonColourId, juce::Colours::black);
    passToDeckCombo.setColour(ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
    loopButton.setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::black);

//draw an outline around the component
    g.drawRect(getLocalBounds(), 1);

//draw some placeholder text
    g.setColour(juce::Colours::orange);
    g.setFont(14.0f);

//toggle play button paint
    if (playState == PlayState::STOP)
    {
        stopButton.setButtonText("PLAY");
    }
    if (playState == PlayState::PLAY)
    {
        stopButton.setButtonText("STOP");
    }

//toggle loop button paint
    if (loopState == LoopState::NOLOOP)
    {
        loopButton.setButtonText("REFRESH DECK TO ON LOOP");
    }
    if (loopState == LoopState::LOOP)
    {
        loopButton.setButtonText("REFRESH DECK TO OFF LOOP");
    }
}
//==============================================================================




//==============================================================================
void DeckGUI::resized()
{
//calculation for rows
    double rowH = getHeight() / 8;
    double rowW = getWidth() / 3;

//set bounds for waveform display
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 4);

//set bounds for play and stop button
    stopButton.setBounds(rowW * 2, rowH * 4, rowW, rowH);

//set bounds for load button
    loadButton.setBounds(rowW * 2, rowH * 5, rowW, rowH);

//set bounds for pass type combobox
    passToDeckCombo.setBounds(rowW * 2, rowH * 6, rowW, rowH);

//set bounds for loop button
    loopButton.setBounds(rowW * 2, rowH * 7, rowW, rowH);

//adjustment of label
    auto sliderLeft = 50;
    
//set bounds for slider
    volSlider.setBounds(sliderLeft + 30, rowH * 4, rowW * 2 - sliderLeft - 30, rowH);
    speedSlider.setBounds(sliderLeft + 30, rowH * 5, rowW * 2 - sliderLeft - 30, rowH);
    cutOffSlider.setBounds(sliderLeft + 30, rowH * 6, rowW * 2 - sliderLeft - 30, rowH);
    qSlider.setBounds(sliderLeft + 30, rowH * 7, rowW * 2 - sliderLeft - 30, rowH);
}
//==============================================================================




//==============================================================================
//start playing from player
void DeckGUI::play()
{
    playState = PlayState::PLAY;
    stopButton.setToggleState(false, NotificationType::dontSendNotification);
    player->start();
}
//==============================================================================




//==============================================================================
//stop playing from player
void DeckGUI::stop()
{
    playState = PlayState::STOP;
    stopButton.setToggleState(true, NotificationType::dontSendNotification);
    player->stop();
}
//==============================================================================




//==============================================================================
void DeckGUI::loop()
{
    loopState = LoopState::LOOP;
    loopButton.setToggleState(false, NotificationType::dontSendNotification);
    player->setLooping(true);

//load URL and get results from player
    player->loadURL(openedfile);
    player->loadURL(openedfilePL);
    player->loadURL(openedfileDD);
}
//==============================================================================


//==============================================================================
void DeckGUI::noloop()
{
    loopState = LoopState::NOLOOP;
    loopButton.setToggleState(true, NotificationType::dontSendNotification);
    player->setLooping(false);

//load URL and get results from player
    player->loadURL(openedfile);
    player->loadURL(openedfilePL);
    player->loadURL(openedfileDD);
}
//==============================================================================




//==============================================================================
void DeckGUI::buttonClicked(Button* button)
{
//stop button was clicked
    if (button == &stopButton)
    {
        if (playState == PlayState::STOP)
        {
            player->setGain(volSlider.getValue());
            player->setSpeed(speedSlider.getValue());

            stopButton.onClick = [this]() { play(); };
        }
        else if (playState == PlayState::PLAY)
        {
            stopButton.onClick = [this]() { stop(); };
        }
    }

//load button was clicked
    if (button == &loadButton)
    {
//condition to check for user to select a file
        FileChooser chooser{ "Select a file..." };

//condition to check if user has choosen a file
       if (chooser.browseForFileToOpen())
        {
            openedfile = URL(chooser.getResult());

//load URL and get results from player
            player->loadURL(URL{ chooser.getResult() });

//load URL and get results from waveform
            waveformDisplay.loadURL(URL{ chooser.getResult() });
        }
    }

//loop button was clicked
    if (button == &loopButton)
    {
        if (loopState == LoopState::NOLOOP)
        {
            loopButton.onClick = [this]() { loop(); };

            playState = PlayState::STOP;
        }
        if (loopState == LoopState::LOOP)
        {
            loopButton.onClick = [this]() { noloop(); };

            playState = PlayState::STOP;
        }
    }
}
//==============================================================================




//==============================================================================
void DeckGUI::sliderValueChanged(Slider* slider)
{
//volume slider
    if (slider == &volSlider)
    {
//deligate slider to control the volume of the audio to player 1
        player->setGain(slider->getValue());
    }

//speed slider
    if (slider == &speedSlider)
    {
//deligate slider to control the speed of the audio to player 1
        player->setSpeed(slider->getValue());
    }

//cutoff slider
    if (slider == &cutOffSlider)
    {
//IIR filter function call to DJAudioplayer
        player->updateParameters(cutOffSlider.getValue(), qSlider.getValue(), passToDeckComboID);

//reset IIR filter
        player->reset();
    }

//q slider
    if (slider == &qSlider)
    {
//IIR filter function call to DJAudioplayer
        player->updateParameters(cutOffSlider.getValue(), qSlider.getValue(), passToDeckComboID);

//reset IIR filter
        player->reset();
    }
}
//==============================================================================




//==============================================================================
void DeckGUI::typeParams()
{
//get combo box selection ID
    passToDeckComboID = passToDeckCombo.getSelectedId();

//IIR filter function call to DJAudioplayer
    player->updateParameters(cutOffSlider.getValue(), qSlider.getValue(), passToDeckComboID);
}
//==============================================================================




//==============================================================================
//implementing file drag
bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
//debugging
    DBG("DeckGUI::isInterestedInFileDrag");

//always return true when file is dragged to the GUI
    return true;
}
//==============================================================================




//==============================================================================
//implement files dropped
void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    for (String filename : files)
    {
//debugging
        DBG("DeckGUI::filesDropped " << filename);

//url of audio file
        URL fileURL = URL{ File{filename} };

//storing url to be used in loop
        openedfileDD = URL{ File{filename} };

//load url into player
        player->loadURL(fileURL);

//load URL and get results from waveform
        waveformDisplay.loadURL(fileURL);
        return;
    }
}
//==============================================================================




//==============================================================================
void DeckGUI::timerCallback()
{
//set and getting waveform relative position
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
//==============================================================================




//==============================================================================
void DeckGUI::loadAudio(juce::URL audioURL)
{
    openedfilePL = URL(audioURL);

//load url of audio into player
    player->loadURL(audioURL);

//load URL and get results from waveform
    waveformDisplay.loadURL(audioURL);
}
//==============================================================================

