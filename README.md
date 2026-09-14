# Retro Videoshop Studio

Retro Videoshop Studio is a C++ and openFrameworks application that uses OpenCV to display images, video files, and webcam input through a vintage videoshop-style interface. The application uses a fixed 1280x720 canvas and starts with a branded placeholder image until the user selects another media source.

Memory and leak profiling notes are available in [LEAKS_REPORT.md](LEAKS_REPORT.md).

## How the Application Is Organized

`ofApp` is the main application coordinator. It owns the media manager, view objects, filter objects, and current screen state.

During each openFrameworks update cycle, the application:

1. Updates the active media source.
2. Gets the current frame from `MediaSourceManager`.
3. Draws the selected view into an OpenCV `cv::Mat` canvas.
4. Converts the canvas from BGR to RGB.
5. Uploads the result to an openFrameworks `ofImage` for display.

The main application states are:

- **Home:** Dashboard with the live preview and navigation cards.
- **Quad View:** Four previews of the same source: original, retro, holiday, and party.
- **Mode View:** One selected preset filter with an intensity control.
- **Filter Studio:** Manual controls for brightness, contrast, sharpness, and color inversion.

## Media Input

`MediaSourceManager` provides the current media frame as an OpenCV `cv::Mat`.

The application supports:

- **Images:** Loaded with openFrameworks `ofImage::load()`.
- **Video files:** Played with openFrameworks `ofVideoPlayer`.
- **Webcam on macOS:** Captured with openFrameworks `ofVideoGrabber`.
- **Webcam on Windows:** Captured with OpenCV `cv::VideoCapture`, using DirectShow through `cv::CAP_DSHOW` while testing available device IDs.

Input pixels are converted from openFrameworks RGB format to OpenCV BGR format for processing.

The media manager also supports rotating the current frame. If a media source cannot be opened, the application displays the bundled placeholder image or generates a simple fallback test pattern.

The Home screen's **Upload/Stream** card provides two actions:

- **Load File:** Opens a file dialog for an image or video.
- **Open Webcam:** Attempts to open an available webcam.

## Filters

`BaseFilter` defines the shared filter interface:

```cpp
virtual cv::Mat process(const cv::Mat& input, float intensity) = 0;
```

The project includes four concrete filters.

### RetroFilter

`RetroFilter`:

- Converts the source image to grayscale.
- Applies a cool blue and denim tint.
- Adds random horizontal VHS-style lines.
- Blends the filtered result according to the selected intensity.

### HolidayFilter

`HolidayFilter`:

- Creates a grayscale base.
- Builds a cool blue version.
- Builds a warm gold and red version.
- Blends between the cool and warm versions based on intensity.

### PartyFilter

`PartyFilter`:

- Increases contrast and brightness.
- Applies blue and magenta split-toning.
- Strengthens the blue and red channels.
- Adds highlighted edges for a neon-style effect.

### ManualFilter

`ManualFilter` provides user-controlled adjustments for:

- Brightness
- Contrast
- Sharpness
- Color inversion

The filters use OpenCV operations including `cvtColor`, channel splitting and merging, weighted blending, `filter2D`, thresholding, edge detection, and color inversion.

## Views and Interaction

- **HomeView** draws the camera-style dashboard, live preview, navigation cards, and blinking recording indicator. It converts mouse clicks into `HomeAction` values.
- **QuadView** draws the four-way comparison view and routes quadrant clicks to the selected preset mode.
- **ModeView** displays one selected preset filter with its name, intensity control, and 35mm-style rectangular frame.
- **FilterStudioView** displays the manual filter workspace.
- **CameraChrome** contains shared camera-style visual elements used by multiple views.

### Keyboard Controls

- Press **Esc** to return to the Home screen.
- Press **+** or **-** in Mode View to change filter intensity.
- Press **]** to rotate the current media frame.
- The on-screen rotation control also rotates the current media frame.

## Repository Structure

```text
src/
  ofApp.*                   Application lifecycle and screen routing
  MediaSourceManager.*      Image, video, webcam, and frame management
  views/                    Home, quad, preset, and manual studio views
  filters/                  Base and concrete image filters

bin/data/                   Runtime assets, including the placeholder image

config.make                 openFrameworks project configuration
addons.make                 Required openFrameworks addons
```

## Building

This project uses openFrameworks and the `ofxOpenCv` addon.

The exact build command depends on the openFrameworks installation and operating system. The project configuration is defined in `config.make`, and the required addons are listed in `addons.make`.

## Collaboration Notes

Keep changes focused by area:

- Media input and frame management
- Image filters
- User interface views
- Application routing

Use feature branches and pull requests so both contributors can review changes before they are merged into `main`.

When documenting ownership, performance results, or testing, include only work and measurements confirmed by the team.
