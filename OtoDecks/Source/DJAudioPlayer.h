/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 7 Feb 2022 7:52:48pm
    Author:  Austin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//implementing DJAudioPlayer class inheritance from audiosource
class DJAudioPlayer : public AudioSource,
                        public AudioAppComponent
{
public:

//generate audio
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

//top level functions needed
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

//functions needed for DJAudioPlayer
    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

//start and stop function for DJAudioPlayer
    void start();
    void stop();

//set looping, set loop bool variable
    void setLooping(bool text);
    bool loop = false;

//get relative position of playhead
    double getPositionRelative();

//getting audio length in seconds
    double audioLengthInSeconds();

//declaration for IIR filter
    void prepare(const dsp::ProcessSpec& spec);
    void process(const dsp::ProcessContextReplacing<float>& context);
    void reset();
    void updateParameters(double cutoffParam, double qParam, int typeParam);

//private class
private: 

//format audio
    AudioFormatManager& formatManager;
       
//unique pointer
    std::unique_ptr<AudioFormatReaderSource> readerSource;

//transport source to stop or start audio
    AudioTransportSource transportSource;

//resample source for speed of audio
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

//declaration for IIR filter
    double sampleRate = 0.0;
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> iir;
};