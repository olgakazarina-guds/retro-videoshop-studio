import React, { useEffect, useState } from 'react';

// Define the properties (props) that this overlay accepts from its parent view
interface ViewfinderOverlayProps {
  showRec?: boolean;           // Whether to show an in-frame recording badge
  showCrosshair?: boolean;     // Whether to display the center optical crosshair
  showTimestamp?: boolean;     // Whether to display the live date & time stamp
  isAudioReactive?: boolean;   // Whether the microphone is active
  audioLevel?: number;         // Current audio volume level (0.0 to 1.0)
  modeTitle?: string;          // Optional title of the active mode (e.g., "RETRO")
  isRecordingVideo?: boolean;  // Whether video export is actively recording
}

// Array of 3-letter month abbreviations used to format the camcorder date
const MONTH_NAMES = [
  'JAN.', 'FEB.', 'MAR.', 'APR.', 'MAY', 'JUN.',
  'JUL.', 'AUG.', 'SEP.', 'OCT.', 'NOV.', 'DEC.'
];

export const ViewfinderOverlay: React.FC<ViewfinderOverlayProps> = ({
  showRec = false,
  showCrosshair = true,
  showTimestamp = true,
  isAudioReactive = false,
  audioLevel = 0,
  modeTitle,
  isRecordingVideo = false,
}) => {
  // formatIndex lets the user click the timestamp to cycle through different display styles
  const [formatIndex, setFormatIndex] = useState<number>(0);

  // Helper function to extract and format the real, live system date and time
  const getFormattedTime = () => {
    const now = new Date(); // Gets the actual current date & time from the computer
    const month = MONTH_NAMES[now.getMonth()];
    const day = String(now.getDate()).padStart(2, '0');
    const year = now.getFullYear();

    // Convert 24-hour time to standard 12-hour AM/PM format
    let hours = now.getHours();
    const ampm = hours >= 12 ? 'PM' : 'AM';
    const hours12 = hours % 12 || 12; // Converts 0 to 12 for midnight
    const hoursStr = String(hours12).padStart(2, '0');
    const minutes = String(now.getMinutes()).padStart(2, '0');
    const seconds = String(now.getSeconds()).padStart(2, '0');

    return {
      dateStr: `${month} ${day}, ${year}`,
      timeStr: `${hoursStr}:${minutes}:${seconds} ${ampm}`,
      smpteStr: `00:${String(now.getMinutes()).padStart(2, '0')}:${seconds}:12`,
    };
  };

  // State variable holding the current live timestamp data
  const [timeData, setTimeData] = useState(getFormattedTime);

  // Set up a 1-second interval timer so the clock ticks in real time
  useEffect(() => {
    const timer = setInterval(() => {
      setTimeData(getFormattedTime());
    }, 1000);

    // Clean up the timer when the component unmounts to prevent memory leaks
    return () => clearInterval(timer);
  }, []);

  return (
    // "pointer-events-none" ensures the overlay doesn't block mouse clicks to the video
    <div className="pointer-events-none absolute inset-0 select-none overflow-hidden text-xs">
      
      {/* 1. Four Optical Corner Viewfinder Brackets */}
      <div className="absolute top-4 left-4 w-5 h-5 border-t-2 border-l-2 border-amber-400/70" />
      <div className="absolute top-4 right-4 w-5 h-5 border-t-2 border-r-2 border-amber-400/70" />
      <div className="absolute bottom-4 left-4 w-5 h-5 border-b-2 border-l-2 border-amber-400/70" />
      <div className="absolute bottom-4 right-4 w-5 h-5 border-b-2 border-r-2 border-amber-400/70" />

      {/* 2. Optional In-Frame Status Badge (only shown if showRec is true) */}
      {showRec && (
        <div className="absolute top-4 left-10 flex items-center gap-1.5 px-2 py-0.5 rounded bg-black/60 border border-red-500/40 text-[11px] font-mono-retro font-bold text-red-400">
          <span className="w-2 h-2 rounded-full bg-red-500 animate-pulse" />
          <span>LIVE</span>
        </div>
      )}

      {/* 3. Top Right: Mode Title and Real-Time Audio VU Meter */}
      <div className="absolute top-4 right-10 flex items-center gap-2 font-mono-retro">
        {modeTitle && (
          <span className="px-2.5 py-1 rounded bg-black/75 border border-amber-500/40 text-amber-300 text-[11px] font-semibold tracking-wide shadow-md">
            {modeTitle}
          </span>
        )}
        {isAudioReactive && (
          <div className="hidden sm:flex items-center gap-1.5 px-2 py-1 rounded bg-black/75 border border-pink-500/50 text-pink-400 text-[10px]">
            <span>AUDIO</span>
            <div className="w-10 h-1.5 bg-neutral-800 rounded-xs overflow-hidden flex items-center">
              <div
                className="h-full bg-pink-500 transition-all duration-75"
                style={{ width: `${Math.min(100, Math.round(audioLevel * 100))}%` }}
              />
            </div>
          </div>
        )}
      </div>

      {/* 4. Center: Optical Alignment Crosshair */}
      {showCrosshair && (
        <div className="absolute inset-0 flex items-center justify-center pointer-events-none opacity-30">
          <div className="relative w-7 h-7">
            <div className="absolute top-1/2 left-0 w-full h-[1px] bg-neutral-100 -translate-y-1/2" />
            <div className="absolute left-1/2 top-0 h-full w-[1px] bg-neutral-100 -translate-x-1/2" />
            <div className="absolute inset-1 rounded-full border border-neutral-100" />
          </div>
        </div>
      )}

      {/* 5. Bottom Left: Live Camcorder Timestamp (Clickable to switch formats) */}
      {showTimestamp && (
        <div
          id="viewfinder-osd-timestamp"
          onClick={() => setFormatIndex((prev) => (prev + 1) % 3)}
          className="pointer-events-auto cursor-pointer absolute bottom-4 left-10 font-mono-retro font-bold text-amber-400 bg-black/80 backdrop-blur-xs px-2.5 py-1.5 rounded-md border border-amber-500/30 shadow-md hover:border-amber-400/60 transition-colors select-none"
          title="Click to toggle timestamp format (Single-line / Stacked / Timecode)"
        >
          {/* Style 0: Clean single-line live date and time */}
          {formatIndex === 0 && (
            <div className="flex items-center gap-2 text-xs tracking-wider">
              <span>{timeData.dateStr}</span>
              <span className="text-amber-500/60">•</span>
              <span>{timeData.timeStr}</span>
            </div>
          )}
          {/* Style 1: Classic 2-line VHS stacked camcorder OSD */}
          {formatIndex === 1 && (
            <div className="flex flex-col text-[11px] leading-tight tracking-wider">
              <span>{timeData.timeStr}</span>
              <span className="text-amber-300/80">{timeData.dateStr}</span>
            </div>
          )}
          {/* Style 2: Studio broadcast SMPTE timecode */}
          {formatIndex === 2 && (
            <div className="flex items-center gap-2 text-xs tracking-wider text-emerald-400">
              <span className="text-emerald-500 font-bold">SMPTE TC</span>
              <span>{timeData.smpteStr}</span>
            </div>
          )}
        </div>
      )}

      {/* 6. Bottom Right: Broadcast Specs and Format Telemetry */}
      <div className="hidden sm:block absolute bottom-4 right-10 font-mono-retro text-neutral-300 text-[11px] bg-black/75 px-2.5 py-1 rounded border border-neutral-700/60 shadow-md">
        16:9 • 720p • 30FPS • NTSC
      </div>
    </div>
  );
};
