# makeSound

**makeSound** is a small C program that converts image pixel data into audio.

It loads an image using `stb_image`, processes the pixel values into audio samples, and saves the resulting audio as a WAV file. It can also play the generated audio through SDL2.

The project is mainly an experiment in **C, image processing, audio generation, and digital signal processing**.

## How it works

The current pipeline is:

```text
Image
  ↓
stb_image
  ↓
RGB pixel data
  ↓
Pixel values → audio samples
  ↓
WAV file
  ↓
Optional audio playback through SDL2
```

Currently, the red channel of each RGB pixel is used to influence the generated waveform.

The audio uses:

* Sample rate: **44,100 Hz**
* Channels: **Mono**
* Format: **32-bit floating-point PCM**
* Base frequency: **440 Hz**

## Requirements

* GCC
* GNU Make
* SDL2
* `stb_image`
* A C standard library
* `libm` (`math.h`)

### Linux

On Debian/Ubuntu:

```bash
sudo apt install gcc make libsdl2-dev
```

On Arch Linux:

```bash
sudo pacman -S gcc make sdl2
```

You will also need `stb_image.h` available in your include path.

## Building

Clone the repository:

```bash
git clone https://github.com/maazbilal49-maker/makeSound.git
cd makeSound
```

Build with:

```bash
make
```

This produces:

```text
makeSound
```

To remove the compiled executable:

```bash
make clean
```

## Usage

Basic usage:

```bash
./makeSound image.png
```

This will:

1. Load the image.
2. Convert its pixel data into audio samples.
3. Save the result as:

```text
output.wav
```

4. Play the generated sound.

### Disable playback

If you only want to generate the WAV file:

```bash
./makeSound image.png --noplay
```

This is useful when generating audio from many images or when you don't want the program to immediately play the result.

## Example

```bash
./makeSound picture.png
```

Output:

```text
output.wav
```

The resulting WAV file can be opened with audio software such as Audacity, VLC, or another WAV-compatible player.

## Current limitations

The current image-to-sound algorithm is intentionally simple.

Each pixel contributes one audio sample, so the duration of the generated audio depends directly on the number of pixels:

```text
duration = (width × height) / 44100
```

For example:

```text
100 × 100 image
= 10,000 samples
≈ 0.23 seconds
```

This means large images produce considerably longer audio.

The current algorithm also produces fairly experimental/noisy results because the image data is being mapped directly into a waveform.

## Future ideas

Possible improvements include:

* Better image → frequency mapping
* Use brightness instead of only one color channel
* Use RGB channels as separate audio parameters
* Map the X axis to time
* Map the Y axis to frequency
* Generate multiple simultaneous frequencies
* Add different waveforms
* Add volume/amplitude mapping
* Add stereo output
* Add WAV configuration options
* Add MP3 export
* Add command-line options for sample rate and frequency
* Create a spectrogram of the generated sound
* Experiment with Fourier transforms
* Create reversible image/audio encoding

## Technologies

* **C** — core implementation
* **stb_image** — image loading
* **SDL2** — audio playback
* **WAV/PCM** — audio output

## License

This project is currently an experimental personal project.

See the repository for licensing information.
