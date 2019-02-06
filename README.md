# Ambilight Screen Grabber

This project is a lightweight screengrabber that is required in order to make the Adalight Arduino project work. For each frame it captures, a color average is done for each pixel location and everything is parsed then sent to the Arduino board through serial port interface.

## Usage

First, the solution needs to be compiled. If Visual Studio is used as the compiler, for better performance, setting the solution configuration at `Release` will make a drastic change.

After this is done, simply launch the executable. It will be running in the taskbar, where you can exit it at any time you want by right clicking on the icon.

## Options

Options are now loaded from a file located at `%AppData%\Roaming\Ambilight\options.txt`. Three options are currently available: `smoothing` is controlling the linear interpolation between each frame captured, `portname` is representing the serial port used by the Arduino and `coordinates` represents the array for pixel coordinates where a LED should be located behing the screen. This options file has the following structure where `coordinates` can be "infinitely" expanded: 

```
smoothing=5
portname=\\.\COM10
coordinates=[
    {1151,1079},
    {1232,1079},
    {1294,1079}
]
```

## Additional info

This program is a Windows based application only since it is based on the Win32 API.

This project has been base of a working Adalight Arduino project from James Bruce where you can find the source code [here](https://gist.github.com/jamesabruce/09d79a56d270ed37870c).
