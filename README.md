SharkSound
==========
v0.1 - Copyright Sharkable Inc.

SharkSound is for loading and playing sounds in native code. It has a focus on mobile. It is cross
platform with a C++ API. It supports iOS, OS X, and Android. It wraps OpenAL on Apple platforms and
OpenSL ES on Android.

It supports WAV and MP3 on all platforms, as well as whatever sound formats those platforms can
load.

Send questions and comments to jgs@sharkable.com
Thanks!

Building
--------

Include the files in the `sharksound` directory in your project, as well as the appropriate platform
subdirectory. Put the `src` directory into your header search path.

For the Android NDK, make sure to support STL and OpenSL ES. Here's a hint at what you might want in
your `.mk` build files:
```
APP_STL := gnustl_static
LOCAL_LDLIBS := -lOpenSLES -landroid
```

Using
-----

It's simple! You construct a `SharkSound::SoundController` appropriate to your platform. If you
don't hate preprocessor macros, you might do it like this:

```cpp
SharkSound::SoundController *sound_controller = NULL;
#if __APPLE__
  sound_controller = new AppleSoundController();
#elif __ANDROID__
  // Pass in the Activity's AssetManager (activity.getAssets()) to the native code as
  // asset_manager_java.
  AAssetManager *asset_manager = AAssetManager_fromJava(env, asset_manager_java);  
  sound_controller = new AndroidSoundController(asset_manager);
#else
  #define NO_SOUND_SUPPORT
#endif
```

Now your cross-platform code can load and play sounds!

```cpp
Sound *my_sound = sound_controller->GetSound("sounds/foo.wav");
my_sound->Play();
```

See `sound.h` and `sound_controller.h` for the full interface.

Note on iOS
-----------

Xcode by default flattens the directory structure of your assets. Because of this, if you try to
load `"sounds/foo.wav"` on iOS, SharkSound will first try to load the full path, then try to just
load `"foo.wav"`. This way you can have a directory structure in Android and not worry about that
breaking iOS.

Memory Management
-----------------

It's recommended you use one `SoundController` for the whole run of your app.
After you are done with a sound in a certain context, call `sound->Free()`.
Calling `GetSound` on the same file before freeing an instance will return the SAME instance,
but with a bumped retain count. Every time you call `Free` on that sound, the retain count goes
down, and when it reaches zero, the sound is freed from memory. Calling `GetSound` again will reload
that sound.

TODO
----
- Support limiting the number of times that each sound can overlap.
- Support freeing up memory after overlapping the sound sound too many times.
- Support for streaming long sounds (like MP3s) for music playing.
- More platforms?
