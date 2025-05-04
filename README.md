# AutoScaler

## Aims

Automatically utilise between 82-92% of your GPU at all times in UEVR games.
This means you should not go below your target framerate, and should also utilise your GPU with extra resolution when you have headroom.

## Installation

To add to a UEVR game, download the latest from [Releases](https://github.com/hookmanuk/AutoScaler/releases) and unzip into the plugins folder of your UEVR global dir game folder.

Note that in order for this logic to work, you'll need to ensure you set a fixed target frame rate in SteamVR for your application, so that your GPU will always try and be at full utilisation for your target frame rate.

e.g. If you want to target 90fps then turn motion smoothing off and set the fixed rate to 90.

Or, if you want to target 45fps motion smoothing, then turn motion smoothing on and set the fixed rate to 45.

## Compatibility

This plugin is compatible with multiple UEVR games.
It relies on being able to set the ScreenPercentage in code and it not to be overriden by the game itself.

Primarily it has been tested as working well in Clair Obscur.

Note that this plugin is only concerned with GPU usage. If your framerates are erratic due to CPU usage, then this plugin will not work well.

## Implementation Method

Uses Nvidia NVML to monitor GPU usage and then sets Screen Percentage based on that usage.

This will aim to set your GPU usage between 82% & 92%, changing screen percentage to keep it within these bounds.

To build from source, install Nvidia CUDA, load in Visual Studio and ensure your project paths are pointing at the correct locations.

