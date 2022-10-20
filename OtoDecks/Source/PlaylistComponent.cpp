/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 8 Feb 2022 7:26:18pm
    Author:  Austin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

#include <iostream>
#include <fstream>
#include <string> 

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _parsingMetaData,
                                    DeckGUI* _deckGUI1,
                                    DeckGUI* _deckGUI2)
                                    :parsingMetaData(_parsingMetaData),
                                    deckGUI1 (_deckGUI1),
                                    deckGUI2 (_deckGUI2)
                                   
{
// In your constructor, you should add any child components, and
// initialise any special settings that your component needs.

//adding columns to table
    musicLibrary.getHeader().addColumn("ID", 1, 1);
    musicLibrary.getHeader().addColumn("Songs", 2, 1);
    musicLibrary.getHeader().addColumn("Length", 3, 1);
    musicLibrary.getHeader().addColumn("", 4, 1);

//add track list to table
    musicLibrary.setModel(this);

//add to gui to be displayed
    addAndMakeVisible(musicLibrary);
    addAndMakeVisible(addMusicButton);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(submitButton);
    addAndMakeVisible(deleteAllButton);

//load to deck combo box
    addAndMakeVisible(loadToDeckCombo);
    loadToDeckCombo.addItem("LOAD TO DECK", 1);
    loadToDeckCombo.addItem("LOAD DECK 1", 2);
    loadToDeckCombo.addItem("LOAD DECK 2", 3);
    loadToDeckCombo.onChange = [this] { loadToDeckChanged(); };
    loadToDeckCombo.setSelectedId(1);

//shuffle to deck combo box
    addAndMakeVisible(shuffleToDeckCombo);
    shuffleToDeckCombo.addItem("SHUFFLE TO DECK", 1);
    shuffleToDeckCombo.addItem("SHUFFLE DECK 1", 2);
    shuffleToDeckCombo.addItem("SHUFFLE DECK 2", 3);
    shuffleToDeckCombo.onChange = [this] { shuffleToDeckChanged(); };
    shuffleToDeckCombo.setSelectedId(1);

//adding event listener
    addMusicButton.addListener(this);
    searchBox.addListener(this);
    submitButton.addListener(this);
    deleteAllButton.addListener(this);

//loading of existing library
    loadFromLibrary();
}
//==============================================================================




//==============================================================================
PlaylistComponent::~PlaylistComponent()
{
//save to library by default
    saveToLibrary();
}
//==============================================================================




//==============================================================================
void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

//clear the background
    g.fillAll(Colours::black);

//draw an outline around the component
    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   

//draw some placeholder text
    g.setColour (juce::Colours::orange);
    g.setFont (14.0f);

//set background of buttons and combo box
    addMusicButton.setColour(TextButton::ColourIds::buttonColourId, juce::Colours::black);
    searchBox.setColour(TextEditor::backgroundColourId, juce::Colours::black);
    submitButton.setColour(TextButton::ColourIds::buttonColourId, juce::Colours::black);
    deleteAllButton.setColour(TextButton::ColourIds::buttonColourId, juce::Colours::black);
    loadToDeckCombo.setColour(ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
    shuffleToDeckCombo.setColour(ComboBox::ColourIds::backgroundColourId, juce::Colours::black);

//color text and write text into textbox when empty
    searchBox.setTextToShowWhenEmpty("Search Tracks", juce::Colours::orange);   
    searchBox.setColour(TextEditor::textColourId, Colours::orange);
}
//==============================================================================




//==============================================================================
void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 11.25;
    double rowW = getWidth() / 8;

//setting bounds for table
    musicLibrary.setBounds(getWidth()/ 4, 0, (getWidth() / 4 * 3), getHeight()/3);

//setting column with based on getwidth
    musicLibrary.getHeader().setColumnWidth(1, getWidth() / 20);
    musicLibrary.getHeader().setColumnWidth(2, getWidth() / 4 * 1.8);
    musicLibrary.getHeader().setColumnWidth(3, getWidth() / 5);
    musicLibrary.getHeader().setColumnWidth(4, getWidth() / 20);

//setting bounds for buttons
    addMusicButton.setBounds(0, 0, rowW * 2, rowH);
    deleteAllButton.setBounds(0, rowH, rowW * 2, rowH);

//setting bounds for combobox
    loadToDeckCombo.setBounds(0, rowH * 2, rowW * 2, rowH / 2);
    shuffleToDeckCombo.setBounds(0, rowH * 2.5, rowW * 2, rowH / 2);

//setting bounds for search
    searchBox.setBounds(0, rowH * 3, getWidth() / 6, rowH / 1.25);
    submitButton.setBounds(getWidth() / 6, rowH * 3, getWidth() / 12, rowH / 1.25);
}
//==============================================================================




//==============================================================================
//returning number of rows
int PlaylistComponent::getNumRows()
{
//return number of things in the vector
        return trackTitles.size();
}
//==============================================================================




//==============================================================================
//row background
void PlaylistComponent::paintRowBackground(Graphics & g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    g.setColour(Colours::orange);

//condition to fill the row with particular colour
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
        g.setColour(Colours::black);
    }
    else
    {
        g.fillAll(Colours::black);
    }
}
//==============================================================================




//==============================================================================
//paint cell
void PlaylistComponent::paintCell(Graphics & g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
//checking condition that getNumRows must be larger than rowNumber or vector will be out of range
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
//drawing row number into cell
            g.drawText(std::to_string(rowNumber + 1) + ")",
                2, 0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 2)
        {
//drawing track names into cell
            g.drawText(trackTitles[rowNumber],
                2, 0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 3)
        {
//drawing track length into cell
            g.drawText(audioLength[rowNumber],
                2, 0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }
    }
}
//==============================================================================




//==============================================================================
//specification of each cell
Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                    int columnId,
                                                    bool isRowSelected,
                                                    Component* existingComponentToUpdate)
{
//checking condition to add play button
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "X" };
            btn->setColour(TextButton::ColourIds::buttonColourId, Colours::black);
            btn->setColour(TextButton::ColourIds::textColourOffId, Colours::orange);

//converting rowNumber into a standard string and setting id to button
            String id{ std::to_string(rowNumber) }; 
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }   
    }
    return existingComponentToUpdate;
}
//==============================================================================




//==============================================================================
//play button on track list
void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &addMusicButton)
    {

//calling add music function
        addToMusicLibrary();

//updating to music Library
        musicLibrary.updateContent();
    }

    else if (button == &deleteAllButton)
    {
//delete all tracks from playlist
        trackTitles.clear();
        tracks.clear();
        audioLength.clear();
        trackURL.clear();
        musicLibrary.updateContent();
    }

    else if (button == &submitButton)
    {
//calling search music function and get text from box
        searchMusic(searchBox.getText());
    }

    else
    {
//get id of button clicked
        btnID = button->getComponentID().getIntValue();
        DBG(btnID << " row is deleted");

//delete row based on id
        trackTitles.erase(trackTitles.begin() + btnID);
        tracks.erase(tracks.begin() + btnID);
        audioLength.erase(audioLength.begin() + btnID);
        trackURL.erase(trackURL.begin() + btnID);

//update music library
        musicLibrary.updateContent();
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::loadToDeckChanged()
{
    loadselID = loadToDeckCombo.getSelectedId();

    //DBG(loadselID);
    if (loadselID == 2)
    {
        loadInGUI(deckGUI1);
        loadToDeckCombo.setSelectedId(1);
        //DBG(selectID);
    }

    else if (loadselID == 3)
    {
        loadInGUI(deckGUI2);
        loadToDeckCombo.setSelectedId(1);
        //DBG(selectID);
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::shuffleToDeckChanged()
{
    shuffleselID = shuffleToDeckCombo.getSelectedId();

    //DBG(shuffleselID);
    if (shuffleselID == 2)
    {
        shuffleToGUI(deckGUI1);
        shuffleToDeckCombo.setSelectedId(1);
        //DBG(selectID);
    }

    else if (shuffleselID == 3)
    {
        shuffleToGUI(deckGUI2);
        shuffleToDeckCombo.setSelectedId(1);
        //DBG(selectID);
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::addToMusicLibrary()
{
//prompt user to select files
    juce::FileChooser chooser{ "Select a file" };

//allow user to select multiple files
    if (chooser.browseForMultipleFilesToOpen())
    {

//for loop to constantly get results if user selectes muliple files
        for (File& myFile : chooser.getResults())
        {

//load audioURL of the file
            juce::URL audioURL{ myFile };

//string the title of the name without the format
            titles = myFile.getFileNameWithoutExtension();

//getting length of audio
            audLength = lengthOfAudio(audioURL);

//iterating from vector trackTitles from beginning to end and compare with title of file
            identifier = (std::find(trackTitles.begin(), trackTitles.end(), titles)!= trackTitles.end());

//checking condition if title is not found than add it, else not loaded
            if (identifier != true)
            {
//push string titles, file and audiolength selected by user into individual vector
                trackTitles.push_back(titles);
                tracks.push_back(myFile);
                audioLength.push_back(audLength);
                trackURL.push_back(audioURL);

                DBG(titles << " is loaded.");
            }
            else
            {
                DBG(titles << " is not loaded.");
            }
        }
    }
}
//==============================================================================




//==============================================================================
juce::String PlaylistComponent::lengthOfAudio(juce::URL audioURL)
{
//load audio URL to parsingMetaData (same method from loadbutton in DeckGUI)
    parsingMetaData->loadURL(audioURL);

//storing length in seconds from DJAudioplayer.cpp into lengthInSeconds
    lengthInSeconds = parsingMetaData->audioLengthInSeconds();

//converting seconds with convertseconds function and store into lengthInMinutes
    lengthInMinutes = convertSeconds(lengthInSeconds);

//returning string of lengthInMinutes
    return lengthInMinutes;
}
//==============================================================================




//==============================================================================
juce::String PlaylistComponent::convertSeconds(double lengthInSeconds)
{
//rounding to nearest integer of lengthInSeconds from DJAudioplayer.cpp
    roundedSeconds = std::round(lengthInSeconds);

//calculation from seconds to minutes and string value
    mins = std::to_string((roundedSeconds / 60) % 60);

//calculation for seconds to be in 2 decimal places and string value
    secs = std::to_string(roundedSeconds % 60);

//string to display minutes and seconds calculated
    timeLength = mins + "min" + " " + secs + "sec";

//return duration of audio file
    return timeLength;
}
//==============================================================================




//==============================================================================
void PlaylistComponent::searchMusic(juce::String textMusic)
{
//iterating through trackTitles vector to find title same as input text
    auto compNum = std::find(trackTitles.begin(), trackTitles.end(), textMusic);

    if (compNum != trackTitles.end())
    {
            //DBG("Successfully found " << titles);
            //DBG("Row Number = " << compNum - trackTitles.begin());

//storing calculated row number into int rowNum
            rowval = compNum - trackTitles.begin();

//selecting row from music library based on row number calculated 
            musicLibrary.selectRow(rowval);
    }

    else
    {
        DBG(titles << " not found.");
//deselect all rows if not found
        musicLibrary.deselectAllRows();
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::loadInGUI(DeckGUI* deckGUI)
{
//get row that was selected
    int getRow = musicLibrary.getSelectedRow();

//checking condition that a row is selected and not negative
    if (getRow >= 0)
    {
//load both player and waveform from DeckGUI.cpp
        deckGUI->loadAudio(trackURL[getRow]);
    }

    else
    {
//error message to show if no rows are selected
        DBG("No rows are selected. Thus, no music was loaded");
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::shuffleToGUI(DeckGUI* deckGUI)
{
//checking condition to ensure there are songs in the library
    if (tracks.size() > 0)
    {
//randomly get a number from 0 to tracks size
        int randomRow = rand() % tracks.size() + 0;

//checking condition to ensure the song is always different
        if (randomRow != prevRow)
        {
//selecting row from music library
            musicLibrary.selectRow(randomRow);

 //get row that was selected
            int getRow = musicLibrary.getSelectedRow();

//checking condition that a row is selected and not negative
            if (getRow >= 0)
            {
//load both player and waveform from DeckGUI.cpp
                deckGUI->loadAudio(trackURL[getRow]);
                prevRow = randomRow;
            }
        }

//checking condition to ensure the song is always different
        if (randomRow = prevRow)
        {
//randomly get a number from 0 to tracks size
            int randomRow = rand() % tracks.size() + 0;

//selecting row from music library
            musicLibrary.selectRow(randomRow);

//get row that was selected
            int getRow = musicLibrary.getSelectedRow();

//checking condition that a row is selected and not negative
            if (getRow >= 0)
            {
//load both player and waveform from DeckGUI.cpp
                deckGUI->loadAudio(trackURL[getRow]);

//store randomRow value as previous row number
                prevRow = randomRow;
            }
        }
    }

    else if (tracks.size() <= 0)
    {
        DBG("Add songs into library before shuffling");
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::saveToLibrary()
{
//create stream to write to txt file that was created
    std::ofstream path("track.txt");

//checking condition if file is open
    if (path.is_open())
    {

//checking condition for every file that is added to the library will be written into txt
        for (File& musLib : tracks)
        {

//load audioURL of the file
            juce::URL audioURL{ musLib };

//getting length of audio
            audLength = lengthOfAudio(audioURL);
            
//write file path name and length of audio with a , separting the line
            path << musLib.getFullPathName() << "," << audLength << "\n";
        }
    }

    else
    {
//debugger if file is not open
        DBG("File doesn't exist ...");
    }
}
//==============================================================================




//==============================================================================
void PlaylistComponent::loadFromLibrary()
{
//create stream to read from txt file
    std::ifstream path("track.txt");

//checking condiiton if txt file is open
    if (path.is_open())
    {
        //DBG("File exist ...");

//get each line from txt file and separate line when , is detected
        while (getline(path, filePath, ','))
        {
            /*DBG(filePath);
            DBG("This is length: " << audLength1);*/

//open file from string read from txt file
            juce::File myFile(filePath);

//string the title of the name without the format
            titles = myFile.getFileNameWithoutExtension();

//get each line from txt file when length is detected
            getline(path, audLength1);

//load audioURL of the file
            juce::URL audioURL{ myFile };

//push string titles, file and audiolength selected by user into individual vector
            tracks.push_back(myFile);
            trackTitles.push_back(titles);
            audioLength.push_back(audLength1);
            trackURL.push_back(audioURL);
        }
    }
    else
    {
//debugger if file is not open
        DBG("File doesn't exist ...");
    }
//close file
    path.close();
}
//==============================================================================
