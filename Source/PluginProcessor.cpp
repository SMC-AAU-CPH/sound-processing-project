/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>

//==============================================================================
Bbd_sound_processingAudioProcessor::Bbd_sound_processingAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    for (unsigned int bucket = 0; bucket < N; ++bucket)
    {
        mpBBDFilter[bucket] = new BDDFilter (ORDERIN, ORDEROUT);
        mpBBDFilter[bucket]->setInputZeros (RIN);
        mpBBDFilter[bucket]->setInputPoles (PIN);
        mpBBDFilter[bucket]->setOutputZeros (ROUT);
        mpBBDFilter[bucket]->setOutputPoles (POUT);
        mpBBDFilter[bucket]->setH0 ();
    }
}

Bbd_sound_processingAudioProcessor::~Bbd_sound_processingAudioProcessor()
{
    for (unsigned int m = 0; m < N; ++m)
    {
        delete mpBBDFilter[m];
    }
}

//==============================================================================
const String Bbd_sound_processingAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Bbd_sound_processingAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Bbd_sound_processingAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Bbd_sound_processingAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Bbd_sound_processingAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Bbd_sound_processingAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Bbd_sound_processingAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Bbd_sound_processingAudioProcessor::setCurrentProgram (int index)
{
}

const String Bbd_sound_processingAudioProcessor::getProgramName (int index)
{
    return {};
}

void Bbd_sound_processingAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Bbd_sound_processingAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (unsigned int b = 0; b < N; ++b)
    {
        mpBBDFilter[b]->setSampleRate (getSampleRate ());
        mpBBDFilter[b]->setClockRate (FCLK);
        mpBBDFilter[b]->setBlockSize (getBlockSize ());
    }
    bufferIn = new float[getBlockSize ()];
    bufferOut = new float[getBlockSize ()];
}

void Bbd_sound_processingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Bbd_sound_processingAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Bbd_sound_processingAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto * input  = buffer.getReadPointer (channel);
        auto * output  = buffer.getWritePointer (channel);
        // ..do something to the data...
        mpBBDFilter[0]->setInputPtr (const_cast<float *>(input));
        mpBBDFilter[0]->setOutputPtr (static_cast<float*>(bufferIn));
        mpBBDFilter[0]->process ();

        mpBBDFilter[1]->setInputPtr (const_cast<float *>(bufferIn));
        mpBBDFilter[1]->setOutputPtr (static_cast<float*>(bufferOut));
        mpBBDFilter[1]->process ();

        mpBBDFilter[2]->setInputPtr (const_cast<float *>(bufferOut));
        mpBBDFilter[2]->setOutputPtr (static_cast<float*>(bufferIn));
        mpBBDFilter[2]->process ();

        mpBBDFilter[3]->setInputPtr (const_cast<float *>(bufferIn));
        mpBBDFilter[3]->setOutputPtr (static_cast<float*>(bufferOut));
        mpBBDFilter[3]->process ();

        mpBBDFilter[4]->setInputPtr (const_cast<float *>(bufferOut));
        mpBBDFilter[4]->setOutputPtr (static_cast<float*>(bufferIn));
        mpBBDFilter[4]->process ();
        
        mpBBDFilter[N - 1]->setInputPtr (mpBBDFilter[1]->getOutputPtr ());
        mpBBDFilter[N - 1]->setOutputPtr (output);
        mpBBDFilter[N - 1]->process ();

    }
}

//==============================================================================
bool Bbd_sound_processingAudioProcessor::hasEditor () const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Bbd_sound_processingAudioProcessor::createEditor ()
{
    return new Bbd_sound_processingAudioProcessorEditor (*this);
}

//==============================================================================
void Bbd_sound_processingAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Bbd_sound_processingAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter ()
{
    return new Bbd_sound_processingAudioProcessor ();
}
