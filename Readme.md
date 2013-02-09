Compilation on Debian/Ubuntu Linux
----------------------------------
First get these packages:

        sudo apt-get install build-essential cmake libfreetype6-dev libglew1.5-dev libjpeg62-dev libpng12-dev libsndfile1-dev libxrandr-dev libopenal-dev

Then get SFML 2.0 RC from [this PPA](https://launchpad.net/~sonkun/+archive/sfml-stable), like so:

        sudo add-apt-repository ppa:sonkun/sfml-stable
        sudo apt-get update
        sudo apt-get install libsfml-dev

You also need to have the g++ 4.7 or higher for full [C++11](http://en.wikipedia.org/wiki/C++11) support. On Ubuntu/Kubuntu 12.04 and higher, this can be [obtained](http://askubuntu.com/questions/61254/how-to-update-gcc-to-the-latest-versionin-this-case-4-7-in-ubuntu-10-04) by:

        sudo add-apt-repository ppa:ubuntu-toolchain-r/test
        sudo apt-get update
        sudo apt-get install g++-4.7 c++-4.7

On Eclipse, it will be necessary to enable the `__GXX_EXPERIMENTAL_CXX0X__` flag in order to activate all C++11 features. The flag isn't necessary for some reason, when running g++ from the command-line.

Additionally on Ubuntu and potentially other Linux system, g++ 4.7 will be installed as `g++-4.7` in order to not conflict with an existing older g++ installlation. You should configure your IDE or other tool such that it uses the right g++ version.

By now you should have all the dependencies and necessary tools needed for compilation.
