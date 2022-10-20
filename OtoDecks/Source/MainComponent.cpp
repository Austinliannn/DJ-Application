#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
//make sure you set the size of the component after
//you add any child components.
    setSize (800, 600);

//some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
//specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

//add deckGUIs to gui to be displayed
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

//add playlist to gui to be displayed
    addAndMakeVisible(playlistComponent);

//getting format manager ready
    formatManager.registerBasicFormats();
}
//==============================================================================




//==============================================================================
MainComponent::~MainComponent()
{
//this shuts down the audio device and clears the audio source.
    shutdownAudio();
}
//==============================================================================




//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
//deligate to player 1 and prepare it to play
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);

//deligate to player 2 and prepare it to play
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

//set up for mixer source input
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}
//==============================================================================




//==============================================================================
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
//deligate next audio block to mixersource which has multiple inputs
    mixerSource.getNextAudioBlock(bufferToFill);
}
//==============================================================================




//==============================================================================
void MainComponent::releaseResources()
{
//deligate release resources to player 1
    player1.releaseResources();

//deligate release resources to player 2
    player2.releaseResources();

//deligate release resources to mixerSource
    mixerSource.releaseResources();

//removing all inputs in mixersource
    mixerSource.removeAllInputs();
}
//==============================================================================




//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
// (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

// You can add your drawing code here!
}
//==============================================================================




//==============================================================================
void MainComponent::resized()
{
//calculation for rows
    double rowH = getHeight() / 6;

//setting bounds for deckGUI
    deckGUI1.setBounds(0, 0, getWidth(), rowH * 2.25);
    deckGUI2.setBounds(0, rowH * 2.25, getWidth(), rowH * 2.25);

//setting bounds for playlist
    playlistComponent.setBounds(0, rowH * 4.5, getWidth(), rowH * 4.5);
}
//==============================================================================

