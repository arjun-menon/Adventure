Compilation on Debian/Ubuntu Linux
----------------------------------
First get these packages:

        sudo apt-get install build-essential cmake libfreetype6-dev libglew1.5-dev libjpeg62-dev libpng12-dev libsndfile1-dev libxrandr-dev libopenal-dev

Then get SFML 2.0 RC from [this PPA](https://launchpad.net/~sonkun/+archive/sfml-stable), like so:

        sudo add-apt-repository 
        sudo apt-get update
        sudo apt-get install libsfml-dev

This should setup all the dependencies needed for compilation.
