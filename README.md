#How to use project

1. Install raylib (https://www.raylib.com/) Arch users are welcome (https://archlinux.org/packages/extra/x86_64/raylib/)

2. Get Cmake to build project (https://cmake.org/)

3. Check out repo, go into repo folder and create a build folder

4. Go into folder and type cmake .. followed by make

5. Start executable with ./synthesizer_tryout

#Gnuplot

command to plot sine_wave_generator:

./sine_wave_generator 440 1.0|gnuplot -p -e "set xrange[1:1000]; plot '-' "
