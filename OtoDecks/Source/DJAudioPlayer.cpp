/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 7 Feb 2022 7:52:48pm
    Author:  Austin

  ==============================================================================
*/

#include "DJAudioPlayer.h"
//==============================================================================
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            : formatManager(_formatManager)
{
}
//==============================================================================




//==============================================================================
DJAudioPlayer::~DJAudioPlayer() 
{
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
//getting transportsource to be ready
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

//getting resamplesource to be ready
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

//IIR filter prepare function call
    this->prepare({ sampleRate, (uint32)samplesPerBlockExpected, 2 });
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
//calling on resampleSource
    resampleSource.getNextAudioBlock(bufferToFill);

    dsp::AudioBlock<float> block(*bufferToFill.buffer, (size_t)bufferToFill.startSample);

//IIR filter process function call
    this->process(dsp::ProcessContextReplacing<float>(block));
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::releaseResources()
{
//calling releaseResources on the transportsource
    transportSource.releaseResources();

//calling releaseResources on the resamplesource
    resampleSource.releaseResources();
}
//==============================================================================




//==============================================================================
//loading audio file
void DJAudioPlayer::loadURL(URL audioURL)
{
//convert audioURL into an inputstream and passed to formatManager
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

//check if reader successfully created
    if (reader != nullptr) //good file!
    {
//create an audio format reader source
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));

//looping current track
        newSource->setLooping(loop);

//pass audio format reader source into the transport source
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

//pass ownership of the pointer to the new source
        readerSource.reset(newSource.release());
    }
}
//==============================================================================




//==============================================================================
//function to set looping for audio
void DJAudioPlayer::setLooping(bool text)
{
    if (text == 0)
    {
        loop = false;
        //DBG("FALSE");
    }
    else
    {
        loop = true;
        //DBG("TRUE");
    }
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::setGain(double gain)
{
//setting gain with appropriate range
    if (gain < 0 || gain > 1.0)
    {
//error message that gain is out of range
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
//setting gain
        transportSource.setGain(gain);
    }
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::setSpeed(double ratio)
{
//setting speed with appropriate range
    if (ratio < 0 || ratio > 100)
    {
//error message that speed is out of range
        std::cout << "DJAudioPlayer::setSpeed speed should be between 0 and 100" << std::endl;
    }
    else
    {
//setting speed
        resampleSource.setResamplingRatio(ratio);
    }
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::setPosition(double posInSecs)
{
//setting position in seconds
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
//setting position with appropriate range
    if (pos < 0 || pos > 1.0)
    {
//error message that position is out of range
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
//calculation for length of audio file
        double posInSecs = transportSource.getLengthInSeconds() * pos;

//setting position
        setPosition(posInSecs);
    }
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::start()
{
//calling start on the source
        transportSource.start();
}
//==============================================================================




//==============================================================================
void DJAudioPlayer::stop()
{
//calling stop on the source
    transportSource.stop();
}
//==============================================================================




//==============================================================================
double DJAudioPlayer::getPositionRelative()
{
//getting current position calculation
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
//==============================================================================




//==============================================================================
double DJAudioPlayer::audioLengthInSeconds()
{
//return transportSource length in seconds
    return transportSource.getLengthInSeconds();
}
//==============================================================================




//==============================================================================
//preparing IIR filter
void DJAudioPlayer::prepare(const dsp::ProcessSpec& spec)
{
    //DBG("Prepare is entered");
    sampleRate = spec.sampleRate;

    iir.state = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 440.0);
    iir.prepare(spec);
}
//==============================================================================




//==============================================================================
//processing IIR filter
void DJAudioPlayer::process(const dsp::ProcessContextReplacing<float>& context)
{
    iir.process(context);
}
//==============================================================================




//==============================================================================
//reset IIR filter
void DJAudioPlayer::reset()
{
    iir.reset();
}
//==============================================================================




//==============================================================================
//update parameters of IIR filter
void DJAudioPlayer::updateParameters(double cutoffParam, double qParam, int typeParam)
{
//checking condition of sampleRate not equals to 0
    if (sampleRate != 0.0)
    {
        auto cutoff = static_cast<float> (cutoffParam);
        auto qVal = static_cast<float> (qParam);

//switching between the 3 passes
        switch (typeParam)
        {
        case 1:     *iir.state = dsp::IIR::ArrayCoefficients<float>::makeLowPass(sampleRate, cutoff, qVal); break;
        case 2:     *iir.state = dsp::IIR::ArrayCoefficients<float>::makeHighPass(sampleRate, cutoff, qVal); break;
        case 3:     *iir.state = dsp::IIR::ArrayCoefficients<float>::makeBandPass(sampleRate, cutoff, qVal); break;
        default:    break;
        }

        /*DBG(cutoff);
        DBG(qVal);
        DBG(typeParam);*/
    }
}
//==============================================================================
