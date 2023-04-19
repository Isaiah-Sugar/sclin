



# sc-lin

scanline audio visualization in your DAW

an (incomplete) VST3/AU plugin to display audio inputs 

# use:
after sc-lin is installed, a host application will be able to run the .vst3 or .component files. It should appear as a normal audio plugin with midi input.


# install:
I should really put compiled versions up
but if you've got a compiled version, you just have to move the .vst3 and .au files into the correct directories:


### windows:
* `C:\Program Files\Common Files\VST3`

### macos:
* (for all users)
	* `/Library/Audio/Plug-Ins/VST3` (VST3 obviously)
	* `/Library/Audio/Plug-Ins/Components` (AU plugin)

* (for only 1 user)
	* `/Users/THE-NAME-OF-THE-USER/Library/Audio/Plug-Ins/VST3` (VST3)
	* `/Users/THE-NAME-OF-THE-USER/Library/Audio/Plug-Ins/Components` (AU)
### linux: 
* `figure it out yourself`


# compile:

#### if you're familiar with the JUCE library
* you'll need to change the `MainDir` object (in `MainComponent.h`) to the path of the `sc-lin` folder (parent directory of this file). references to svg files are made with these.


#### for everyone else
* You'll need to have the JUCE library installed to build this. It comes with the Projucer, which creates files for several IDE's. Then you can build from there. I use Xcode because I'm filthy and use macos. Projucer deals with the IDE so I don't have to, which I appreciate. So, long story short, you open the .jucer file with Projucer, then it exports to visual studio or xcode, and it should set everything up to build for you. You might have to figure out how to choose the 'release' configuration, I'm still figuring that part out. I think it's an IDE thing. 

* JUCE library: https://juce.com/get-juce ('personal plan' is free, you can build with that)
* go in MainComponent.h and change the line `const File MainDir = File("/Users/isaiahsugar/programming/audio/plugins/sc-lin/sc-lin");` to the `sc-lin` folder you downloaded. (parent directory of this file)
