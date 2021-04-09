/*
  ==============================================================================

    This file was auto-generated, and then edited into oblivion!
        
        update: why did I write that

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

/*
MyListener::MyListener(){
    
};

MyListener::~MyListener(){
    
};

 */
 


//==============================================================================
SclinAudioProcessor::SclinAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
//    myEditor = NULL;
    
    

    
    ringBuffer = new RingBuffer <float> (2, MainComponent::maxPixels * MainComponent::maxPixels * MainComponent::averageNumber * 2); //maxPixels is twice because x * y, averageNumber currently does nothing (it's 1), and 2 is so we can store 2 frames because edge cases. '_'
    
    masterWidthValue = MainComponent::defaultPixels;
    masterHeightValue = MainComponent::defaultPixels;
    
    masterColorModeState = false;
    
    masterChannelMode1Value = 1;
    masterChannelMode2Value = 1;
    masterChannelMode3Value = 1;
    
    masterChannelKnob1Value = 100.0;
    masterChannelKnob2Value = 100.0;
    masterChannelKnob3Value = 100.0;
    
    
}

SclinAudioProcessor::~SclinAudioProcessor()
{
}




//==============================================================================
const String SclinAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SclinAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SclinAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SclinAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SclinAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SclinAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SclinAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SclinAudioProcessor::setCurrentProgram (int index)
{
}

const String SclinAudioProcessor::getProgramName (int index)
{
    return {};
}

void SclinAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SclinAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}


void SclinAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SclinAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SclinAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
     if (!midiMessages.isEmpty()) {
         for (auto message: midiMessages) {
             
            juce::MidiMessage m = message.getMessage(); //the midi message
            
             
             //midi -> mono
             //code adapted from NotesAndVolts' teensy synthesizer, commented by me because I didn't want to listen to his explanation lol
             
            if (m.isNoteOn()) {
                isOn = true;
                if (lastFilled >= midiBufferSize) {
                    //replace the zeroeth note with the first, and so on, to make room for a new note.
                    for (int gap = 0; gap < (lastFilled - 1); gap++) { //
                        currentMidi[gap] = currentMidi[gap + 1];
                    }
                    lastFilled = midiBufferSize - 1; //make sure that, when we populate this array location, it's actually in bounds.
                }
                
                currentMidi[lastFilled] = m.getNoteNumber(); //put the note where it belongs.
                lastFilled++; //we've just added a note.
                
                currentMidiNote = m.getNoteNumber(); //update the main note value.
            }
            else if (m.isNoteOff()) {
                for (int found = 0; found < lastFilled; found++) { //looping up to the last note
                    if (currentMidi[found] == m.getNoteNumber()) { //if we found the note to remove
                        for (int gap = found; gap < (lastFilled - 1); gap++) { //loop through notes, starting with the one to be removed
                            currentMidi[gap] = currentMidi[gap + 1]; //push data back, so the note being removed is overwritten.
                        }
                        lastFilled--; //there's one less note. it's always just one note being removed, since this is run per midi message.
                        currentMidi[lastFilled] = representsNoData; //lastFilled should not have data in it, though all this copying has left it populated.
                        currentMidiNote = currentMidi[lastFilled - 1];
                    }
                }
            if (currentMidi[0] == representsNoData)
                isOn = false;
            }
        }
                
                //the sample position is message.samplePosition;
       
    }
    
    
    
    
    
    
    
    
    
    
    
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    ringBuffer->writeSamples(buffer, 0, buffer.getNumSamples());
    numBuffers++;
    
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        const float* samples = buffer.getReadPointer (channel);
        int numSamples = buffer.getNumSamples();

        
        // ..do something to the data...
    }
    */
    /*
    MidiBuffer processedMidi;
    int time;
    MidiMessage m;
 
    for (MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.isNoteOn())
        {
            m = MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), m.getVelocity());
            processedMidi.addEvent (m, time);
        }
        else if (m.isNoteOff())
        {
            m = MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), m.getVelocity());
            processedMidi.
        }

 
    }
 
    midiMessages.swapWith (processedMidi);
    */
    
    
    
    
    
    
}

//==============================================================================


// terrible terrible terrible
// i was trying to rig up some kind of way for the stuff to not change every time the editor is redrawn
// hopefully i'll get around to rewriting this when everything becomes daw parameters

void SclinAudioProcessor::sliderValueChanged (Slider* s)
{
    if (s == widthKnobPointer)
        masterWidthValue = s->getValue();
    else if (s == heightKnobPointer)
        masterHeightValue = s->getValue();
    else if (s == channelKnob1Pointer)
        masterChannelKnob1Value = s->getValue();
    else if (s == channelKnob2Pointer)
        masterChannelKnob2Value = s->getValue();
    else if (s == channelKnob3Pointer)
        masterChannelKnob3Value = s->getValue();
};


void SclinAudioProcessor::comboBoxChanged (ComboBox *c) {
    if (c == channelMode1Pointer) {
        masterChannelMode1Value = c->getSelectedId();
        
    }
    
    else if (c == channelMode2Pointer){
        masterChannelMode2Value = c->getSelectedId();
        
    }
    
    else if (c == channelMode3Pointer){
        masterChannelMode3Value = c->getSelectedId();
        
    }
    
};

void SclinAudioProcessor::buttonClicked (Button* b) {
    
    
};

void SclinAudioProcessor::buttonStateChanged (Button* b) {
    
    if (b == colorModePointer) {
        masterColorModeState = b->getToggleState();
        
    }
    
};





bool SclinAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SclinAudioProcessor::createEditor()
{


    SclinAudioProcessorEditor *myEditor = new SclinAudioProcessorEditor (*this, ringBuffer, &currentMidiNote, &isOn);
    

    //these are all the editor values that need to persist between the editor being closed and reopened.
    
    
    widthKnobPointer = &myEditor->mainComponent.widthKnob;
    widthKnobPointer->addListener(this);
    widthKnobPointer->setValue(masterWidthValue);
    
    heightKnobPointer = &myEditor->mainComponent.heightKnob;
    heightKnobPointer->addListener(this);
    heightKnobPointer->setValue(masterHeightValue);
    
    
     
    
    channelKnob1Pointer = &myEditor->mainComponent.channelKnob1;
    channelKnob1Pointer->addListener(this);
    channelKnob1Pointer->setValue(masterChannelKnob1Value);
    
    channelKnob2Pointer = &myEditor->mainComponent.channelKnob2;
    channelKnob2Pointer->addListener(this);
    channelKnob2Pointer->setValue(masterChannelKnob2Value);
    
    channelKnob3Pointer = &myEditor->mainComponent.channelKnob3;
    channelKnob3Pointer->addListener(this);
    channelKnob3Pointer->setValue(masterChannelKnob3Value);
    
    
    colorModePointer = &myEditor->mainComponent.colorMode;
    colorModePointer->addListener(this);
    colorModePointer->setToggleState(masterColorModeState, dontSendNotification);
    colorModePointer->setState(Button::buttonOver);


    channelMode1Pointer = &myEditor->mainComponent.channelMode1;
    channelMode1Pointer->addListener(this);
    channelMode1Pointer->setSelectedId(masterChannelMode1Value);
    
    channelMode2Pointer = &myEditor->mainComponent.channelMode2;
    channelMode2Pointer->addListener(this);
    channelMode2Pointer->setSelectedId(masterChannelMode2Value);
    
    channelMode3Pointer = &myEditor->mainComponent.channelMode3;
    channelMode3Pointer->addListener(this);
    channelMode3Pointer->setSelectedId(masterChannelMode3Value);
    
    
    
    return myEditor;
    
}






//==============================================================================
void SclinAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SclinAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SclinAudioProcessor();
}
