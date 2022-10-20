/*
  ==============================================================================

    DeckGUI.h
    Created: 7 Feb 2022 9:09:10pm
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
class DeckGUI  : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint (juce::Graphics&) override;
    void resized() override;

//implement Button:::Listener 
    void buttonClicked(Button* button) override;

//implementing play and stop button
    void play();
    void stop();

//implementing play and stop button
    void loop();
    void noloop();

//combo box function
    int passToDeckComboID;
    void typeParams();


//storing choosen file
    URL openedfile;
    URL openedfilePL;
    URL openedfileDD;

//implement Slider:::Listener
    void sliderValueChanged(Slider* slider) override;

//implementing file drag
    bool isInterestedInFileDrag(const StringArray& files) override;

//implement files dropped
    void filesDropped(const StringArray& files, int x, int y) override;

//implement timer
    void timerCallback() override;

//loading audio function for PlaylistComponent
    void loadAudio(juce::URL audioURL);

private:
//enum class for play/stop button
    enum class PlayState
    {
        PLAY,
        STOP
    };

//enum class for loop button
    enum class LoopState
    {
        LOOP,
        NOLOOP
    };

//buttons
    PlayState playState{ PlayState::STOP };
    LoopState loopState{ LoopState::NOLOOP};
    TextButton stopButton{ "PLAY" };
    TextButton loadButton{ "LOAD" };
    TextButton loopButton{ "LOOP" };
    

//sliders
    Slider volSlider;
    Slider speedSlider;
    Slider cutOffSlider;
    Slider qSlider;

//labels
    Label volLabel;
    Label spdLabel;
    Label cutOffLabel;
    Label qLabel;

//pass typer combo box in deckGUI
    ComboBox passToDeckCombo{ "LOW-PASS" };

//pointer to DJAudioPlayer in DeckGUI
    DJAudioPlayer* player;

//waveform display
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};