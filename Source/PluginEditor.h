/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Bbd_sound_processingAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Bbd_sound_processingAudioProcessorEditor (Bbd_sound_processingAudioProcessor&);
    ~Bbd_sound_processingAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Bbd_sound_processingAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bbd_sound_processingAudioProcessorEditor)
};
