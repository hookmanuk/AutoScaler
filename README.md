Uses Nvidia NVML to monitor GPU usage and then sets Screen Percentage based on that usage.

To build from source, install Nvidia CUDA.

To add to a UEVR game, drop the DLL from Releases into the plugins folder.

This will aim to set your GPU usage between 82% & 92%, changing screen percentage to keep it within these bounds.
Note that in order for this logic to work, you'll need to ensure you set a fixed target frame rate in SteamVR for your application, so that your GPU will always try and be at full utilisation for your target frame rate.

e.g. If you want to target 90fps then turn motion smoothing off and set the fixed rate to 90.

Or, if you want to target 45fps motion smoothing, then turn motion smoothing on and set the fixed rate to 45.