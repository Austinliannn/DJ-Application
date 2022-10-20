/*
  ==============================================================================

    PlaylistComponent.h
    Created: 8 Feb 2022 7:26:18pm
    Author:  Austin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent : public Component,
                        public TableListBoxModel,
                        public Button::Listener,
                        public TextEditor::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* _parsingMetaData,
                    DeckGUI* _deckGUI1,
                    DeckGUI* _deckGUI2
    );
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

//returning number of rows
    int getNumRows() override;

//row background
    void paintRowBackground(Graphics & , 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override;

//paint cell
    void paintCell (Graphics & ,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;

//specification of cell
    Component* refreshComponentForCell(int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component* existingComponentToUpdate) override;

//buttons for music library
    TextButton addMusicButton{ "ADD MUSIC" };

//search box for music library
    TextEditor searchBox{ "" };
    TextButton submitButton{ "SUBMIT" };

//loading combo box in deckGUI
    ComboBox loadToDeckCombo { "LOAD TO DECK" };

//delete all music from library
    TextButton deleteAllButton{ "CLEAR PLAYLIST" };

//shuffle random music from library into GUI
    TextButton shuffleDeck1Button{ "SHUFFLE TO DECK 1" };
    TextButton shuffleDeck2Button{ "SHUFFLE TO DECK 2" };

    ComboBox shuffleToDeckCombo{ "SHUFFLE TO DECK" };

//adding play button listener
    void buttonClicked(Button* button) override;

private:
//adding table list box
    TableListBox musicLibrary;

//vectors for titles, track files and length
    std::vector <juce::String> trackTitles;
    std::vector <juce::String> audioLength;
    std::vector <juce::File> tracks;
    std::vector <juce::URL> trackURL;

//add music to library and iterating through vector
    void addToMusicLibrary();
    bool identifier;

//parsing meta data
    DJAudioPlayer* parsingMetaData;

//string title
    juce::String titles;

//string length
    juce::String audLength;
    juce::String lengthOfAudio(juce::URL audioURL);
    juce::String convertSeconds(double lengthInSeconds);

//convertion of length
    double lengthInSeconds;
    int roundedSeconds;
    juce::String lengthInMinutes;
    juce::String mins;
    juce::String secs;
    juce::String timeLength;

//search music in library
    void searchMusic(juce::String textMusic);
    int rowval;

//load to DeckGUI
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    void loadInGUI(DeckGUI* deckGUI);
    void loadToDeckChanged();
    int loadselID;


//saving and loading of music to library when user restarts program
    juce::File path;
    std::string filePath;
    std::string audLength1;
    void loadFromLibrary();
    void saveToLibrary();    

//deleting track with button in column
    int btnID;

//shuffle to DeckGUI
    void shuffleToGUI(DeckGUI* deckGUI);
    void shuffleToDeckChanged();
    int prevRow = 0;
    int shuffleselID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
