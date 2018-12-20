# Ambilight Screen Grabber

This project is a lightweight screengrabber that is required in order to make the Adalight Arduino project work. For each frame it captures, a color average is done for each pixel location and everything is parsed then sent to the Arduino board through serial port interface.

## Usage

First, the solution needs to be compiled. If Visual Studio is used as the compiler, for better performance, setting the solution configuration at `Release` will make a drastic change.

After this is done, simply launch the executable. It will be running in the taskbar, where you can exit it at any time you want.

## Options

All options are currently specified in `options.h`. Any changes to this file will not affect directly the executable until the solution is compiled again.

## Additional info

This program is a Windows based application only since it is based on the Win32 API.

This project has been base of a working Adalight Arduino project from James Bruce where you can find the source code [here](https://gist.github.com/jamesabruce/09d79a56d270ed37870c).
