# Retro Videoshop Studio

An interactive C++ and OpenCV-powered creative studio application designed to function as a digital time machine. The software transforms ordinary video clips, photos, and live camera streams into emotionally loaded memories through the lens of a classic, retro-styled studio interface.

## High-Level Project Goals & Expected Behaviour
* The application features an intuitive Home Screen Dashboard modeled after a vintage camera aesthetic, providing four primary interactive choices:
* **Play View:** Opens the signature 2x2 Quad-View matrix displaying the active media feed across four distinct cinematic styles simultaneously (Original, 1950s Retro, Holiday Warmth, and Party Neon). Clicking any quadrant instantly routes the user to that mode's dedicated fine-tuning view.
* **Select Mode:** Opens a direct pathway to jump straight into a specific mode's fine-tuning view (Retro, Holiday, or Party) equipped with an intuitive intensity scale.
* **Upload/Stream:** Manages media source configuration, allowing on-the-fly switching between static images (cv::imread), pre-recorded video files (cv::VideoCapture), and live webcam feeds (cv::VideoCapture(0)).
* **Manual Filter:** Opens the Filter-All Studio sandbox, enabling independent raw parameter adjustments (brightness, contrast, inversion, and sharpness).

## Special Features & Add-Ons
* **Film Strip Borders & Overlays:** Thematic visual framing mimicking classic film strips for the mode views.
* **Vintage Camera Aesthetics & Overlays:** Real-time 2D drawing primitives (cv::putText, cv::rectangle, cv::line) simulating a viewfinder feel with a pulsing red [REC] indicator, retro digital timestamps, and alignment grid lines.
* **Audio-Reactive Filters:** Real-time microphone input integration dynamically modulating filter parameters (such as neon contrast pulsing or film grain flickering) based on audio volume.
* **Snapshot & Video Export:** Dedicated UI buttons allowing users to capture frozen image snapshots (cv::imwrite) or record short clips directly from active view frames.

## Naming & Code Conventions

* **Classes & Structs:** PascalCase (`MediaSourceManager`, `RetroFilter`, `HomeView`)
* **Functions & Methods:** camelCase starting with action verbs (`getCurrentFrame()`, `processFrame()`, `handleMouseClick()`)
* **Member Variables:** m_camelCase (`m_intensity`, `m_capture`, `m_activeFilter`)
* **Constants & Enums:** ALL_CAPS (`FILTER_RETRO`, `MODE_QUAD`)
* **OpenCV Data:** `cv::Mat` type for all image and video frame representations
* **Header Files:** Every `.h` file must begin with `#pragma once`

## Quick Class & Architecture Overview

* **MediaSourceManager:** Abstracts input handling, managing static images (`cv::imread`), video files, and live webcam feeds (`cv::VideoCapture`) to deliver a standardized `cv::Mat` frame.
* **BaseFilter (Abstract Base Class):** Defines the pure virtual `process(const cv::Mat& input, float intensity)` contract for polymorphic execution.
* **RetroFilter, HolidayFilter, PartyFilter, ManualFilter:** Concrete derived classes executing specific pixel transformations (1950s sepia, color channel shifting, neon contrast, and raw slider adjustments).
* **HomeView:** Manages the vintage camera dashboard interface and hosts four primary clickable interaction options.
* **QuadView:** Manages the 2x2 live matrix rendering loop using region-of-interest (ROI) slicing and click-to-mode routing.
* **ModeView & FilterStudioView:** Dedicated UI environments handling fine-tuning intensity scales and raw parameter sliders framed inside film strip borders.

## Shared Method & Interface Contract

To maintain clean separation of concerns and prevent Git merge clashes between teammates, these exact function signatures form the core architectural contract:

### Filter Core & Pipeline
* **BaseFilter:** `virtual cv::Mat process(const cv::Mat& input, float intensity) = 0;`
* **MediaSourceManager:** 
  * `bool loadMedia(const std::string& path);`
  * `void update();`
  * `cv::Mat getCurrentFrame() const;`

### UI View Stack
* **HomeView:** 
  * `void draw();`
  * `int handleMouseClick(int x, int y) const;`
* **QuadView:** 
  * `void draw(const cv::Mat& frame);`
  * `int handleMouseClick(int x, int y) const;`
* **ModeView:** 
  * `void draw(const cv::Mat& frame, float intensity);`
  * `void setIntensity(float intensity);`

## Core Features & Add-Ons

* **Polymorphic DSP Processing:** Derived filter classes eliminate conditional branching (`if/else` or `switch` statements) inside high-frequency video rendering loops.
* **2x2 Live Quad-View Matrix:** Displays the active media feed across four distinct cinematic styles simultaneously with interactive click-to-mode routing.
* **Vintage Camera Viewfinder Aesthetics:** Real-time 2D drawing primitives (`cv::putText`, `cv::rectangle`, `cv::line`) simulating an authentic feel with a pulsing red `[REC]` indicator, orange retro timestamps, and alignment grid lines.
* **Audio-Reactive Filters:** Microphone input integration that dynamically modulates filter parameters (such as neon contrast pulsing or film grain flickering) based on real-time audio volume and beats.
* **Snapshot & Video Export:** Dedicated UI buttons allowing users to capture frozen image snapshots (`cv::imwrite`) or short video clips directly from active view frames.

## Team Collaboration Rules (GitHub Workflow)
Designed for a 2-person development team (40-hour split allocation) to ensure clean version control and zero code clashes:

**Branching Strategy:**
* Never commit directly to main.
* Use feature branches for all development (e.g., feature/media-manager, feature/retro-filter, feature/ui-quadview).
* Pull Requests & Code Reviews:
* Open a Pull Request (PR) when a feature is complete.
* Both teammates must briefly review the PR before merging into main to maintain architectural consistency.

**Strict Separation of Concerns:**
* Teammate A will focus on Core Input & Base Filters while Teammate B will implement UI Views & Preset Filters. The View Orchestration will be done in a joint, sequential manner.

**Baseline Maintenance:**
* Keep main stable. Always pull the latest changes from main into your feature branch before starting a new coding session.
