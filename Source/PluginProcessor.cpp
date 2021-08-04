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
const int SclinAudioProcessor::defaultPixels = 32;//default value for pixel knobs (x pixels / y pixels)
const int SclinAudioProcessor::defaultCycles = 2;//default value for pixel knobs (x pixels / y pixels)
const bool SclinAudioProcessor::defaultColorMode = false;
const int SclinAudioProcessor::defaultMult = 100;
const bool SclinAudioProcessor::defaultMidiState = false;
 
const int SclinAudioProcessor::maxPixels = 256; //maximum of each pixel knob (x pixels / y pixels)
const int SclinAudioProcessor::minPixels = 1; //minimum of each pixel knob (x pixels / y pixels)
const int SclinAudioProcessor::maxCycles = 10; //maximum of each pixel knob (x pixels / y pixels)
const int SclinAudioProcessor::minCycles = 1; //minimum of each pixel knob (x pixels / y pixels)
 
const int SclinAudioProcessor::maxMult = 200; //maximum value for the channel multipliers, minimum is just negative.


const juce::StringArray SclinAudioProcessor::colorSourceOptionStrings = juce::StringArray{"AVG", "LEFT", "RIGHT", "KNOB"};


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
                       ),
                       myTreeState(*this, nullptr, "Parameters", createParameters()) //createParameters() is my function. based on a tutorial from The Audio Programmer youtube channel
#endif
{
//    myEditor = NULL;
    
    

    
    ringBuffer = new RingBuffer <float> (2, maxPixels * maxPixels * MainComponent::averageNumber * 2); //maxPixels is twice because x * y, averageNumber currently does nothing (it's 1), and 2 is so we can store 2 frames because edge cases. '_'
    
    //AudioProcessor::addListener(this);
    /*
    
    //parameters for x and y resolutions
    xPixelsParam ("xpixels", "X Pixels", minPixels, maxPixels, defaultPixels) ;
    myTreeState.createAndAddParameter(std::unique_ptr<RangedAudioParameter>());
    addParameter(yPixelsParam = new AudioParameterInt("ypixels", "Y Pixels", minPixels, maxPixels, defaultPixels));
    addParameter(xCyclesParam = new AudioParameterInt("xcycles", "X Cycles", minCycles, maxCycles, defaultCycles));
    
    addParameter(colorModeParam = new AudioParameterBool("rgbhsv", "RGB/HSV", defaultColorMode));
    


    //these are the parameters for all the per-channel options. Sources and multipliers for every color channel. In HSV mode, they shouldn't actually be red green and blue, but hue saturation and value. This behavior should be possible to implement - basically identical to switching between midi and no midi modes, and keeping values properly.
    //---------- NOTE ----------
    //      the multiplier's ranges are defined in a rather janky way, but i was having trouble getting a NormalisableRange object defined in the MainComponent class and still build so i basically just copy pasted that part from the juce audio parameter tutorial.
    addParameter(redSourceParam = new AudioParameterChoice("redSourceParam", "Red Source", colorSourceOptionStrings, 1));
    addParameter(redMultParam = new AudioParameterFloat("redMultParam", "Red Multiplier", juce::NormalisableRange<float> (-1 * maxMult, maxMult), defaultMult));
    
    addParameter(greenSourceParam = new AudioParameterChoice("greenSourceParam", "Green Source", colorSourceOptionStrings, 1));
    addParameter(greenMultParam = new AudioParameterFloat("greenMultParam", "Green Multiplier", juce::NormalisableRange<float> (-1 * maxMult, maxMult), defaultMult));
    
    addParameter(blueSourceParam = new AudioParameterChoice("blueSourceParam", "Blue Source", colorSourceOptionStrings, 1));
    addParameter(blueMultParam = new AudioParameterFloat("blueMultParam", "Blue Multiplier", juce::NormalisableRange<float> (-1 * maxMult, maxMult), defaultMult));


    addParameter(roundnessParam = new AudioParameterFloat("roundnessParam", "Roundness", 0.0, 1.0, 0.0));
    addParameter(midiOnParam = new AudioParameterBool("midiOnParam", "Midi Toggle", defaultMidiState));
     addParameter(freezeParam = new AudioParameterBool("freezeParam", "Freeze", false));
    */
    /*
    masterWidthValue = MainComponent::defaultPixels;
    masterHeightValue = MainComponent::defaultPixels;
    
    masterColorModeState = false;
    
    masterChannelMode1Value = 1;
    masterChannelMode2Value = 1;
    masterChannelMode3Value = 1;
    
    masterChannelKnob1Value = 100.0;
    masterChannelKnob2Value = 100.0;
    masterChannelKnob3Value = 100.0;
    */
    
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
                currentNoteCycleLength = this->getSampleRate()/MidiMessage::getMidiNoteInHertz(currentMidiNote);
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
                else
                    currentNoteCycleLength = this->getSampleRate()/MidiMessage::getMidiNoteInHertz(currentMidiNote);
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



bool SclinAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SclinAudioProcessor::createEditor()
{


    SclinAudioProcessorEditor *myEditor = new SclinAudioProcessorEditor (this, ringBuffer, &currentNoteCycleLength, &isOn);
    
    
    
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


juce::AudioProcessorValueTreeState::ParameterLayout SclinAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    
    params.push_back(std::make_unique<AudioParameterBool>("midienable", "Midi On", false));
    params.push_back(std::make_unique<AudioParameterFloat>("roundness", "Roundness", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterBool>("freeze", "Freeze", false));
    
    
    params.push_back(std::make_unique<AudioParameterInt>("xpixels", "X Pixels", minPixels, maxPixels, defaultPixels));
    params.push_back(std::make_unique<AudioParameterInt>("xcycles", "X Cycles", minCycles, maxCycles, defaultCycles));
    params.push_back(std::make_unique<AudioParameterInt>("ypixels", "Y Pixels", minPixels, maxPixels, defaultPixels));

    
    params.push_back(std::make_unique<AudioParameterBool>("colormode", "RGB / HSV", false));

    params.push_back(std::make_unique<AudioParameterChoice>("channelmode1", "Channel 1 Mode", colorSourceOptionStrings, 0));
    params.push_back(std::make_unique<AudioParameterChoice>("channelmode2", "Channel 2 Mode", colorSourceOptionStrings, 0));
    params.push_back(std::make_unique<AudioParameterChoice>("channelmode3", "Channel 3 Mode", colorSourceOptionStrings, 0));


    
    params.push_back(std::make_unique<AudioParameterFloat>("channelknob1", "Channel 1 Multiplier", NormalisableRange<float>(-200.0f, 200.0f), 100.0f,
    String(), AudioProcessorParameter::genericParameter,
    [this](float value, int maximumStringLength) -> String { return String(channelKnobsStringFromValue(value, 5)); }));
    params.push_back(std::make_unique<AudioParameterFloat>("channelknob2", "Channel 2 Multiplier", NormalisableRange<float>(-200.0f, 200.0f), 100.0f,
    String(), AudioProcessorParameter::genericParameter,
    [this](float value, int maximumStringLength) -> String { return String(channelKnobsStringFromValue(value, 5)); }));
    params.push_back(std::make_unique<AudioParameterFloat>("channelknob3", "Channel 3 Multiplier", NormalisableRange<float>(-200.0f, 200.0f), 100.0f,
    String(), AudioProcessorParameter::genericParameter,
    [this](float value, int maximumStringLength) -> String { return String(channelKnobsStringFromValue(value, 5)); }));
 

    return {params.begin(), params.end()};
}

juce::String SclinAudioProcessor::channelKnobsStringFromValue(float value, int maxLength) {
    //a lambda function for the channelKnobs (the multipliers), giving the % suffix, and fixing the number of decimals
    String valueAsString = String(value);
    valueAsString = valueAsString.dropLastCharacters(valueAsString.length() - maxLength); //remove characters from the end, above the maxLength.
    valueAsString = valueAsString.trimCharactersAtEnd(StringRef(".")); //if the last character is a period, remove it. Matters for negative numbers with 3 digits.
    valueAsString += "%";
    return valueAsString;
    
}
