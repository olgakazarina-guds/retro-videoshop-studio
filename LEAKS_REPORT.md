# Memory and Leak Analysis

## Purpose

This report documents the memory check required for the Retro Videoshop Studio
project. The application was profiled with Apple's Instruments **Leaks** template
on macOS.

## Trace information

| Item | Result |
| --- | --- |
| Trace file | `Leaks.trace` |
| Instrument template | Leaks |
| Recording duration | 231.4 seconds |
| Process | `retro-videoshop-studio-mainDebug` |
| Platform | macOS on Apple silicon |
| Recording end | Stopped by the user |

The trace's Leaks table was exported with `xctrace` and contained **327 leaked
records**, representing **20,256 bytes** in total.

## Results

| Responsible library | Records | Bytes |
| --- | ---: | ---: |
| Foundation | 258 | 16,320 |
| LinkServices | 63 | 3,360 |
| AppIntents | 6 | 576 |
| **Total** | **327** | **20,256** |

The responsible stack frames are Apple framework functions, including:

- `NSXPCConnection`
- `NSXPCInterface`
- `LNDaemonApplicationXPCInterface`
- `setProtocolMetadataWithSignature`
- `_BridgedURL.relativePath(percentEncoded:)`

No leak record is attributed to the application binary, `MediaSourceManager`,
`ofApp`, OpenCV, openFrameworks, AVFoundation, or any filter/view class.

## Interpretation

This trace does **not** demonstrate an application-owned leak. The reported
allocations are runtime allocations from macOS services used by the process or
its environment. They cannot be released safely from the Videoshop source
code.

## Heap and stack usage in this project

The application uses both kinds of memory for different responsibilities:

| Memory area | Project examples | Why it is appropriate |
| --- | --- | --- |
| Stack | Function parameters, local `cv::Mat` headers such as `temp`, `displayMat`, and `rotated`, local `ofImage` objects, local `std::vector<cv::Point>` values, and filter-local variables | These values have a limited scope and are automatically destroyed when the function returns. |
| Heap/owned buffers | Pixel storage behind `cv::Mat`, `ofImage`/`ofPixels` data, OpenCV filter outputs, resized/cropped matrices, the `displayImage` pixel/texture storage, and video/webcam buffers owned by `ofVideoPlayer` and `ofVideoGrabber` | Image and video data is too large and too long-lived for stack storage. These objects manage dynamically allocated buffers through RAII and reference counting. |
| GPU-managed memory | The OpenFrameworks texture updated by `displayImage.update()` | The rendered image must be stored in graphics memory so the GPU can display it efficiently. |

For example, `ofApp::draw()` creates short-lived conversion matrices on the
stack as C++ objects, while the pixel buffers owned by those matrices are
allocated dynamically. `MediaSourceManager` keeps the current frame and media
objects as members whose owned buffers live as long as needed. The source
manager calls `videoPlayer.close()` and `webcam.close()` before switching
sources, and C++ destructors release remaining resources when the owning
objects leave scope.

The filters also create multiple temporary matrices per frame for operations
such as color conversion, masking, edge detection, and resizing. These are
expected heap-buffer allocations with stack-based C++ handles; they are not
leaks when their references are released after the frame is processed.

The project uses RAII-managed objects:

- `cv::Mat` releases its reference-counted image buffers automatically.
- `ofImage` and `ofPixels` release their owned pixel storage when they leave
  scope.
- `ofVideoPlayer` and `ofVideoGrabber` are explicitly closed when switching
  media sources.
- Temporary matrices created by filters, rotation, cropping, resizing, and
  per-frame display conversion are expected to be released when their scope or
  reference count ends.

Adding arbitrary `delete`, `free`, or manual reference-count operations would be
incorrect and could introduce double-free errors or crashes.

## Scope and limitation

This result classifies the supplied trace. It does not replace a repeatable
functional test of every media path. For final presentation evidence, run a
fresh Instruments session after warming up the application and repeat:

1. Start with the bundled placeholder.
2. Switch between image and video sources several times.
3. Open Quad, Select Mode, and Manual Filter views.
4. Rotate media repeatedly.
5. Return to Home and repeat the cycle.
6. Test webcam setup and source switching if camera access is available.
7. Stop recording only after the final cycle and export the Leaks table.

Compare live bytes after each identical cycle. One-time framework startup
allocations should be distinguished from memory that grows continuously. Keep
the exported table and a screenshot of the Leaks instrument with the learning
log.

## Requirement assessment

| Course expectation | Evidence/status |
| --- | --- |
| Show that no memory loss was observed | **Met for the supplied trace:** no project-owned leak was reported. Do not claim that every possible execution path is mathematically leak-free. |
| Explain heap and stack memory | **Met in this report:** stack-based C++ handles/local values manage dynamically allocated image, video, and GPU buffers through RAII. |
| Verify realistic application behavior | **Repeat recommended:** perform the documented image/video/view/rotation/webcam cycle in a fresh Instruments run and retain the exported result. |

## Conclusion

For the supplied `Leaks.trace`, **no project-owned memory leak was found and no
source-code fix is warranted**. The result is the following:

> Instruments reported 20,256 bytes in 327 leak records, all originating in
> macOS Foundation, LinkServices, and AppIntents runtime code. No leak was
> attributed to the Videoshop application or its OpenCV/openFrameworks media
> pipeline. The application uses RAII and explicit media-source closing; a
> repeatable warm-up and media-switching run should be retained as the final
> application-specific verification. This supports the conclusion that no
> project-owned memory leak was observed; it does not prove that every possible
> execution path is leak-free.
