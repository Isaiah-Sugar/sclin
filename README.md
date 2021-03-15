# sc-lin

    scanline audio visualization in your DAW

    an (incomplete) VST3/AU plugin to display audio inputs 
    PLEASE REPORT ANY BUGS THROUGH GITHUB
    YOU CAN'T BE MORE CLUELESS THAN ME
    HOW DO I TURN OFF CAPS LOCK


# use:
    when installed, a host application will be able to run the .vst3 or .component files. It should appear as a normal audio plugin with midi input. 


# install:
    I should really put compiled versions up
    but if you've compiled it yourself, you just have to move the .vst3 and .au files into the correct directories:


    windows:


    macos:
        (for all users)
        /Library/Audio/Plug-Ins/VST3 (VST3 obviously)
        /Library/Audio/Plug-Ins/Components (AU plugin)

        (for only 1 user)
        /Users/THE-NAME-OF-THE-USER/Library/Audio/Plug-Ins/VST3 (VST3)
        /Users/THE-NAME-OF-THE-USER/Library/Audio/Plug-Ins/Components (AU)

    linux:



# compile:

    if you're familiar with the JUCE library, then just know that I'm not doing anything strange, it's literally just a Projucer project and some source files. WHY ARE YOU EVEN READING THIS. JUST BUILD IT ALREADY

    for everyone else
    you'll need to have the JUCE library installed. It comes with the Projucer, which creates files for several IDE's. Then you can build from there. I use Xcode because I'm filthy and don't use linux. Projucer deals with the IDE so I don't have to, which I appreciate. So, long story short, you open the .jucer file with Projucer, then it exports to an IDE of your choice, and it should set everything up to build for you. You might have to figure out how to choose the 'release' configuration, I'm still figuring that part out. I think it's an IDE thing. 
