# Ambilight Screen Grabber

This project is a lightweight screengrabber that is required in order to make the Adalight Arduino project work. For each frame it captures, a color average is done for each pixel location and everything is parsed then sent to the Arduino board through serial port interface.

## Usage

First, the solution needs to be compiled. If Visual Studio is used as the compiler, for better performance, setting the solution configuration at `Release` will make a drastic change. After this is done, simply launch the executable. It will be running in the taskbar, where you can exit it at any time you want by right clicking on the icon.

## Options

Options are now loaded from a file located right beside the executable. The options file must be named `options.json`. An exemple options file is available in this repository at `ambilight/options.json`. The most important fields are `portname`, `baudRate`, `coordinates` and `radius`. The first two need to match the confirguration of your Arduino setup. 

### Portname

The value for `portname` can be found in the Arduino IDE, in Tools and Port. 

### Baud rate

The value for `baudRate` needs to match the value specified in the sketch uploaded to the Arduino. Usual values can be found on the [Arduino documentation](https://www.arduino.cc/en/Reference/SoftwareSerialBegin). Higher values result in higher refresh rate but can affect CPU usage.

### Coordinates

The `coordinates` field is an array of x and y pixel coordinates on your screen. This array must be ordered in a way that it matches the leds ordering starting from the Arduino pin. These coordinates will be the positions of where to sample the screen.

### Radius

The `radius` field represent the maximum distance from a coordinate that needs to be considered in the pixel average sent to the Arduino.

### Luts

The `luts` field is used to adjust the color accuracy of the pixels. The first one in this list is the default one, and the ones after are used if the physical screen monitor brightness is changed to a lower value than the threshold. 

### Smoothing

The `smoothing` field gives the user the ability to have the pixels slowly respond to changes on the screen. Its value should be between 0 and 1.

### Target frame rate

The `targetFrameRate` field is used when the high fidelity mode is not activated. It limits the program refresh rate, thus reducing CPU load. High fidelity mode can be enabled from the taskbar when a smoother result is needed but will result in higher CPU usage.

## Additional info

This program is a Windows based application only since it is based on the Win32 API. This project was started because of a working Adalight Arduino project from James Bruce where you can find the source code [here](https://gist.github.com/jamesabruce/09d79a56d270ed37870c). Doing this project I've learned a lot about the Win32 ecosystem and how Serial communication works. I've also learn to always keep optimization in mind while still having an understandable code base. I'm glad I can now more easily bring this project to another computer and customize it to the user's need.

## License 

The MIT License
