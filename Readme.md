Building on Debian/Ubuntu Linux
--------------------------------
First get these packages:

        sudo apt-get install build-essential cmake libfreetype6-dev libglew1.5-dev libjpeg62-dev libpng12-dev libsndfile1-dev libxrandr-dev libopenal-dev

Then get SFML 2.0 RC from [this PPA](https://launchpad.net/~sonkun/+archive/sfml-stable), like so:

        sudo add-apt-repository ppa:sonkun/sfml-stable
        sudo apt-get update
        sudo apt-get install libsfml-dev

You also need to have g++ 4.6 or higher for full [C++11](http://en.wikipedia.org/wiki/C++11) support. On older Ubuntu-based distributons, the following PPA can be used to obtain [the latest](http://askubuntu.com/questions/61254/how-to-update-gcc-to-the-latest-versionin-this-case-4-7-in-ubuntu-10-04) versions of the GCC toolchain:

        sudo add-apt-repository ppa:ubuntu-toolchain-r/test
        sudo apt-get update
        sudo apt-get install g++-4.6

Additionally, if you have multiple versions of GCC installed, g++ 4.6 might not be your default version of g++. In these cases, it might be better to specify the version of g++ explicitly in your compiler command as `g++-4.6`.

### Eclipse Configuration

- **Enabling C++11 support:**

	- In *Project -> Properties -> C/C++ Build -> Settings (page) -> Tool Settings (tab) -> GCC C++ Compiler -> Command*, change `g++` to `g++-4.6`. Do the same for *GCC C++ Linker* as well.

	- In *Project -> Properties -> C/C++ Build -> Settings (page) -> Tool Settings (tab) -> GCC C++ Compiler -> Miscallaneous -> Other flags*, add the following flag: `-std=c++0x` (sets the standard to the then-yet-to-be-released ISO C++11).

	- In *Project -> Properties -> C/C++ Build -> Settings (page) -> Tool Settings (tab) -> GCC C++ Compiler -> Preprocessor -> Defined Symbols (-D)*, add the following symbol: `__GXX_EXPERIMENTAL_CXX0X__` (enabling this macro is necessary for Eclipse to be able to parse the C++11 extensions in STL headers).

- **Linker settings:** In *Project -> Properties -> C/C++ Build -> Settings (page) -> Tool Settings (tab) -> GCC C++ Linker -> Libraries -> Libraries (-l)*, add the following libraries: `sfml-graphics`, `sfml-window` and `sfml-system`.

- **Speeding up the build process:** Select one of the appropriate options from: *Project -> Properties -> C/C++ Build -> (main page) -> Behavior (tab) -> Build settings -> Enable parallel build*.

- **Dealing with an out-of-sync index:** Every now and then, the Eclipse C++ indexer will go haywire and start reporting various segments of your code as erroneous even though everything compiles fine. There is an easy [fix](http://stackoverflow.com/a/3040761) to this: Close Eclipse; go to your workspace directory; delete everything under `$workspace\.metadata\.plugins\org.eclipse.cdt.core`; restart Eclipse. The indexer will run again, and everything should be back to normal.