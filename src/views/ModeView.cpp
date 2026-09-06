#include "HomeView.h"

// ==============================================================================
// Constructor: Initialize default values
// ==============================================================================
HomeView::HomeView() {
    m_tallyBlink = true;
    m_tapeCounter = 142.6f;
    setup();
}

// ==============================================================================
// Setup: Calculate initial layout positions for the monitor and cards
// ==============================================================================
void HomeView::setup() {
    windowResized(ofGetWidth(), ofGetHeight());
}

// ==============================================================================
// Update: Run per-frame timers (tally blink and tape counter)
// ==============================================================================
void HomeView::update() {
    // 1. Blink the tally LED lamp every 600 milliseconds
    // (ofGetElapsedTimeMillis() % 1200 < 600 gives a steady 50% duty-cycle pulse)
    m_tallyBlink = (ofGetElapsedTimeMillis() % 1200) < 600;

    // 2. Slowly increment the vintage mechanical tape counter
    m_tapeCounter += 0.005f;
    if (m_tapeCounter > 999.9f) {
        m_tapeCounter = 0.0f;
    }
}

// ==============================================================================
// Window Resized: Dynamically adjusts all elements to fit the window cleanly
// ==============================================================================
void HomeView::windowResized(int w, int h) {
    // 1. Calculate the center monitor dimensions (16:9 widescreen aspect ratio)
    float monitorW = ofClamp(w * 0.75f, 640.0f, 960.0f);
    float monitorH = monitorW * (9.0f / 16.0f);
    float monitorX = (w - monitorW) * 0.5f;
    float monitorY = 55.0f; // Sits below the header bar

    m_monitorRect.set(monitorX, monitorY, monitorW, monitorH);

    // 2. Set up the 4 bottom dashboard navigation cards
    m_cards.clear();
    float cardsStartY = monitorY + monitorH + 28.0f;
    float totalCardsW = monitorW;
    float cardGap = 16.0f;
    float singleCardW = (totalCardsW - (cardGap * 3.0f)) / 4.0f;
    float cardH = 100.0f;

    // Card 1: Play View (Quad Matrix)
    NavCard card1;
    card1.bounds.set(monitorX + (0 * (singleCardW + cardGap)), cardsStartY, singleCardW, cardH);
    card1.title = "1. Play View";
    card1.line1 = "2x2 Quad Matrix";
    card1.line2 = "Live 4-way stream";
    card1.actionText = "CLICK TO ENTER >";
    card1.borderColor = ofColor(245, 197, 24); // Retro Yellow
    card1.targetState = 1;
    m_cards.push_back(card1);

    // Card 2: Select Mode
    NavCard card2;
    card2.bounds.set(monitorX + (1 * (singleCardW + cardGap)), cardsStartY, singleCardW, cardH);
    card2.title = "2. Select Mode";
    card2.line1 = "Retro | Holiday | Party";
    card2.line2 = "Intensity slider studio";
    card2.actionText = "CLICK TO ENTER >";
    card2.borderColor = ofColor(239, 68, 68); // Red
    card2.targetState = 2;
    m_cards.push_back(card2);

    // Card 3: Upload / Stream
    NavCard card3;
    card3.bounds.set(monitorX + (2 * (singleCardW + cardGap)), cardsStartY, singleCardW, cardH);
    card3.title = "3. Upload/Stream";
    card3.line1 = "Image | Video | Cam";
    card3.line2 = "Switch active media feed";
    card3.actionText = "CLICK TO ENTER >";
    card3.borderColor = ofColor(6, 182, 212); // Cyan
    card3.targetState = 3;
    m_cards.push_back(card3);

    // Card 4: Manual Filter Sandbox
    NavCard card4;
    card4.bounds.set(monitorX + (3 * (singleCardW + cardGap)), cardsStartY, singleCardW, cardH);
    card4.title = "4. Manual Filter";
    card4.line1 = "Filter-All Sandbox";
    card4.line2 = "Raw DSP tweak sliders";
    card4.actionText = "CLICK TO ENTER >";
    card4.borderColor = ofColor(16, 185, 129); // Emerald Green
    card4.targetState = 4;
    m_cards.push_back(card4);
}

// ==============================================================================
// Draw (Default): Renders background, chassis, viewfinder, and cards
// ==============================================================================
void HomeView::draw() {
    update();

    // 1. Draw top header title bar
    drawHeader(m_monitorRect.x, 20.0f, m_monitorRect.width);

    // 2. Draw physical camera chassis with the blinking tally lamp in the bezel corner
    drawCameraChassis(m_monitorRect.x, m_monitorRect.y, m_monitorRect.width, m_monitorRect.height);

    // 3. Draw on-screen viewfinder overlay (crosshair and live real-time clock)
    drawViewfinderOSD(m_monitorRect.x, m_monitorRect.y, m_monitorRect.width, m_monitorRect.height);

    // 4. Draw the 4 clickable navigation cards
    drawDashboardCards();
}

// ==============================================================================
// Draw (With cv::Mat): Helper if your main loop renders the OpenCV frame directly
// ==============================================================================
void HomeView::draw(const cv::Mat& frame) {
    draw();
}

// ==============================================================================
// Draw Header: Title and format telemetry
// ==============================================================================
void HomeView::drawHeader(float x, float y, float w) {
    ofPushStyle();
    // Yellow status LED pip
    ofSetColor(245, 158, 11);
    ofDrawRectangle(x, y + 2, 8, 8);

    // Title
    ofSetColor(240, 240, 240);
    ofDrawBitmapString("RETRO VIDEOSHOP STUDIO - VINTAGE OPTICAL CONSOLE", x + 16, y + 10);

    // Telemetry specs on the right
    ofSetColor(140, 140, 145);
    std::string spec = "NTSC 525-LINE | 1280x720 30FPS";
    ofDrawBitmapString(spec, x + w - (spec.length() * 8.0f), y + 10);
    ofPopStyle();
}

// ==============================================================================
// Draw Camera Chassis: Outer metal bezel housing the [REC] tally outside the picture
// ==============================================================================
void HomeView::drawCameraChassis(float x, float y, float w, float h) {
    ofPushStyle();

    // Bezel border thickness
    float bezelTop = 24.0f;
    float bezelBottom = 20.0f;

    // Outer dark chassis casing
    ofSetColor(24, 24, 28);
    ofDrawRectRounded(x - 6, y - bezelTop, w + 12, h + bezelTop + bezelBottom, 8);

    // Metallic chassis border
    ofNoFill();
    ofSetColor(70, 70, 78);
    ofSetLineWidth(2.0f);
    ofDrawRectRounded(x - 6, y - bezelTop, w + 12, h + bezelTop + bezelBottom, 8);
    ofFill();

    // --------------------------------------------------------------------------
    // TOP BEZEL CORNER: Physical [REC] Tally Light Lamp (OUTSIDE THE PICTURE)
    // --------------------------------------------------------------------------
    float lampX = x + 12;
    float lampY = y - (bezelTop * 0.5f);

    // Lamp chrome bezel ring
    ofSetColor(60, 60, 65);
    ofDrawCircle(lampX, lampY, 6);

    // Red LED diode: glows red when tallyBlink is true, dark red when false
    if (m_tallyBlink) {
        ofSetColor(239, 68, 68); // Bright glowing red
    } else {
        ofSetColor(80, 20, 20);  // Dim standby red
    }
    ofDrawCircle(lampX, lampY, 4);

    // Label next to lamp
    ofSetColor(m_tallyBlink ? ofColor(248, 113, 113) : ofColor(130, 130, 135));
    ofDrawBitmapString("REC TALLY", lampX + 10, lampY + 4);

    // Tape Counter display
    ofSetColor(120, 120, 125);
    ofDrawBitmapString("TAPE", lampX + 110, lampY + 4);
    ofSetColor(245, 176, 0); // Amber phosphor
    ofDrawBitmapString(ofToString(m_tapeCounter, 1), lampX + 148, lampY + 4);

    // Top-right power battery indicator
    ofSetColor(16, 185, 129); // Green
    ofDrawBitmapString("PWR [|||]", x + w - 75, lampY + 4);

    // --------------------------------------------------------------------------
    // BOTTOM BEZEL BAR: Hardware locked status
    // --------------------------------------------------------------------------
    float botY = y + h + (bezelBottom * 0.5f) + 4;
    ofSetColor(16, 185, 129);
    ofDrawCircle(x + 12, botY - 4, 3);
    ofSetColor(140, 140, 145);
    ofDrawBitmapString("SYNC LOCKED  |  IRIS: f/2.8  |  SHUTTER: 1/60s", x + 22, botY);

    ofPopStyle();
}

// ==============================================================================
// Draw Viewfinder OSD: Crosshair & Real-Time Dynamic Camcorder Timestamp
// ==============================================================================
void HomeView::drawViewfinderOSD(float x, float y, float w, float h) {
    ofPushStyle();

    // 1. Center Optical Crosshair (clean and subtle)
    float midX = x + (w * 0.5f);
    float midY = y + (h * 0.5f);
    ofSetColor(255, 255, 255, 60);
    ofSetLineWidth(1.0f);
    ofDrawLine(midX - 12, midY, midX + 12, midY);
    ofDrawLine(midX, midY - 12, midX, midY + 12);
    ofNoFill();
    ofDrawCircle(midX, midY, 6);
    ofFill();

    // 2. Optical Viewfinder Corner Brackets
    ofSetColor(245, 176, 0, 160); // Amber corner brackets
    ofSetLineWidth(2.0f);
    float bLen = 14.0f;
    float pad = 16.0f;
    // Top-left bracket
    ofDrawLine(x + pad, y + pad, x + pad + bLen, y + pad);
    ofDrawLine(x + pad, y + pad, x + pad, y + pad + bLen);
    // Bottom-left bracket
    ofDrawLine(x + pad, y + h - pad, x + pad + bLen, y + h - pad);
    ofDrawLine(x + pad, y + h - pad, x + pad, y + h - pad - bLen);
    // Top-right bracket
    ofDrawLine(x + w - pad, y + pad, x + w - pad - bLen, y + pad);
    ofDrawLine(x + w - pad, y + pad, x + w - pad, y + pad + bLen);
    // Bottom-right bracket
    ofDrawLine(x + w - pad, y + h - pad, x + w - pad - bLen, y + h - pad);
    ofDrawLine(x + w - pad, y + h - pad, x + w - pad, y + h - pad - bLen);

    // --------------------------------------------------------------------------
    // BOTTOM-LEFT TIMESTAMP: Dynamically formatted using real system clock
    // (Replaces the static hardcoded 1952 date with live date & time!)
    // --------------------------------------------------------------------------
    const char* monthNames[] = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    };
    int currentMonth = ofClamp(ofGetMonth() - 1, 0, 11);
    int currentDay = ofGetDay();
    int currentYear = ofGetYear();

    // Convert to 12-hour format with AM/PM
    int rawHours = ofGetHours();
    std::string ampm = (rawHours >= 12) ? "PM" : "AM";
    int hours12 = rawHours % 12;
    if (hours12 == 0) hours12 = 12;
    int currentMinutes = ofGetMinutes();
    int currentSeconds = ofGetSeconds();

    // Format string: e.g. "SEP. 06 2026  12:35:10 PM"
    std::string liveTimestamp = ofVAArgsToString(
        "%s. %02d %04d  %02d:%02d:%02d %s",
        monthNames[currentMonth], currentDay, currentYear,
        hours12, currentMinutes, currentSeconds, ampm.c_str()
    );

    // Draw dark protective background pill so text is always readable over any video
    float stampX = x + pad + 10.0f;
    float stampY = y + h - pad - 6.0f;
    float stampW = (liveTimestamp.length() * 8.0f) + 16.0f;
    float stampH = 20.0f;

    ofSetColor(0, 0, 0, 190);
    ofDrawRectRounded(stampX - 4, stampY - 14, stampW, stampH, 4);

    // Draw amber retro phosphor text
    ofSetColor(245, 176, 0); // Amber color
    ofDrawBitmapString(liveTimestamp, stampX + 4, stampY);

    ofPopStyle();
}

// ==============================================================================
// Draw Dashboard Cards: Clean ASCII text without any broken '?' characters
// ==============================================================================
void HomeView::drawDashboardCards() {
    ofPushStyle();

    for (const auto& card : m_cards) {
        // 1. Card background fill
        ofSetColor(18, 18, 22);
        ofDrawRectRounded(card.bounds, 6);

        // 2. Colored accent border
        ofNoFill();
        ofSetColor(card.borderColor);
        ofSetLineWidth(1.8f);
        ofDrawRectRounded(card.bounds, 6);
        ofFill();

        // 3. Card Title (bold headline)
        ofSetColor(245, 245, 245);
        ofDrawBitmapString(card.title, card.bounds.x + 12, card.bounds.y + 22);

        // 4. Description lines (using clean ASCII '-' and '|' so no '?' appears!)
        ofSetColor(160, 160, 165);
        ofDrawBitmapString(card.line1, card.bounds.x + 12, card.bounds.y + 44);
        ofDrawBitmapString(card.line2, card.bounds.x + 12, card.bounds.y + 60);

        // 5. Action prompt at the bottom
        ofSetColor(card.borderColor);
        ofDrawBitmapString(card.actionText, card.bounds.x + 12, card.bounds.y + 84);
    }

    ofPopStyle();
}

// ==============================================================================
// Handle Mouse Click: Detects which card the user clicked and returns mode ID
// ==============================================================================
int HomeView::handleMouseClick(int x, int y) const {
    for (const auto& card : m_cards) {
        if (card.bounds.inside(x, y)) {
            return card.targetState; // 1 = Quad, 2 = Select Mode, 3 = Upload, 4 = Manual
        }
    }
    return 0; // Clicked outside cards
}
