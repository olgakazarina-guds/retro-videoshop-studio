import React, { useState, useEffect } from 'react';

// Properties accepted by the CameraChassis component
interface CameraChassisProps {
  children: React.ReactNode;   // The video canvas or image placed inside the frame
  title?: string;              // Model title displayed on the chassis nameplate
  subTitle?: string;           // Subtitle for hardware/format specs
  isRecording?: boolean;       // Whether export recording is active
  isAudioReactive?: boolean;   // Whether the mic VU meter should be shown
  audioLevel?: number;         // Volume level for audio reactivity
  className?: string;          // Extra Tailwind CSS classes
  showSprockets?: boolean;     // Whether to display 35mm film sprocket holes
}

export const CameraChassis: React.FC<CameraChassisProps> = ({
  children,
  title = 'LIVE MONITOR',
  subTitle = 'NTSC 525-LINE • 60Hz',
  isRecording = false,
  isAudioReactive = false,
  audioLevel = 0,
  className = '',
  showSprockets = false,
}) => {
  // State to toggle the blinking tally light on and off
  const [tallyBlink, setTallyBlink] = useState<boolean>(true);
  // Simulated tape position counter
  const [tapeCounter, setTapeCounter] = useState<number>(142.6);

  // Set up timer loops for the flashing tally light and mechanical tape counter
  useEffect(() => {
    // Flash tally LED every 600 milliseconds
    const tallyInterval = setInterval(() => {
      setTallyBlink((prev) => !prev);
    }, 600);

    // Slowly increment tape counter every 2 seconds
    const counterInterval = setInterval(() => {
      setTapeCounter((prev) => +(prev + 0.1).toFixed(1));
    }, 2000);

    return () => {
      clearInterval(tallyInterval);
      clearInterval(counterInterval);
    };
  }, []);

  return (
    <div
      className={`relative flex flex-col bg-[#18181c] border-2 border-neutral-700/80 rounded-xl shadow-[0_20px_50px_rgba(0,0,0,0.85)] overflow-hidden ${className}`}
    >
      {/* Metallic Corner Screws for industrial camera aesthetic */}
      <div className="absolute top-2 left-2 w-2 h-2 rounded-full bg-neutral-600 border border-neutral-400/40 shadow-sm flex items-center justify-center pointer-events-none z-20">
        <div className="w-1 h-[1px] bg-neutral-800 rotate-45" />
      </div>
      <div className="absolute top-2 right-2 w-2 h-2 rounded-full bg-neutral-600 border border-neutral-400/40 shadow-sm flex items-center justify-center pointer-events-none z-20">
        <div className="w-1 h-[1px] bg-neutral-800 -rotate-45" />
      </div>
      <div className="absolute bottom-2 left-2 w-2 h-2 rounded-full bg-neutral-600 border border-neutral-400/40 shadow-sm flex items-center justify-center pointer-events-none z-20">
        <div className="w-1 h-[1px] bg-neutral-800 -rotate-45" />
      </div>
      <div className="absolute bottom-2 right-2 w-2 h-2 rounded-full bg-neutral-600 border border-neutral-400/40 shadow-sm flex items-center justify-center pointer-events-none z-20">
        <div className="w-1 h-[1px] bg-neutral-800 rotate-45" />
      </div>

      {/* TOP CHASSIS BAR: Contains hardware status outside the picture */}
      <div className="relative px-6 py-2.5 bg-gradient-to-b from-[#24242a] via-[#1c1c22] to-[#16161a] border-b border-neutral-800 flex items-center justify-between select-none z-10">
        
        {/* Left: Hardware Tally Light [REC] Housing (OUTSIDE picture area) */}
        <div className="flex items-center gap-3">
          <div className="flex items-center gap-2 bg-neutral-900/90 px-2.5 py-1 rounded-md border border-neutral-700/70 shadow-inner">
            <div className="relative flex items-center justify-center">
              <div className="w-4 h-4 rounded-full bg-neutral-800 border border-neutral-500/60 shadow-sm flex items-center justify-center">
                {/* Physical Red LED lamp that glows and blinks */}
                <div
                  className={`w-2.5 h-2.5 rounded-full transition-all duration-200 ${
                    isRecording
                      ? 'bg-rose-500 shadow-[0_0_12px_#f43f5e] ring-2 ring-rose-400'
                      : tallyBlink
                      ? 'bg-red-500 shadow-[0_0_8px_#ef4444]'
                      : 'bg-red-950 opacity-40'
                  }`}
                />
              </div>
            </div>

            {/* Label next to the lamp */}
            <div className="flex flex-col leading-none">
              <span
                className={`font-mono-retro font-bold text-xs tracking-wider ${
                  isRecording
                    ? 'text-rose-400 animate-pulse'
                    : tallyBlink
                    ? 'text-red-400'
                    : 'text-neutral-500'
                }`}
              >
                {isRecording ? 'EXPORT REC' : 'REC TALLY'}
              </span>
              <span className="text-[9px] font-mono-retro text-neutral-400 tracking-tight mt-0.5">
                CAM-1 OPTICAL
              </span>
            </div>
          </div>

          {/* Mechanical Tape Counter display */}
          <div className="hidden sm:flex items-center gap-1.5 bg-black/60 px-2 py-1 rounded border border-neutral-800 text-[11px] font-mono-retro text-neutral-400">
            <span className="text-neutral-500 text-[9px] uppercase">TAPE</span>
            <span className="text-amber-400 font-bold tracking-widest font-mono">
              {tapeCounter.toFixed(1)}
            </span>
          </div>
        </div>

        {/* Center: Camera Model Nameplate */}
        <div className="flex flex-col items-center justify-center text-center">
          <span className="text-[11px] font-display-retro font-bold tracking-widest text-neutral-200 uppercase drop-shadow">
            {title}
          </span>
          <span className="text-[9px] font-mono-retro text-neutral-400 tracking-wider">
            {subTitle}
          </span>
        </div>

        {/* Right: Power Battery & Audio Indicators */}
        <div className="flex items-center gap-2.5">
          {isAudioReactive && (
            <div className="hidden md:flex items-center gap-1.5 bg-pink-950/40 border border-pink-500/40 px-2 py-0.5 rounded text-[10px] font-mono-retro text-pink-300">
              <span>MIC VU</span>
              <div className="w-8 h-1.5 bg-neutral-900 rounded-sm overflow-hidden flex">
                <div
                  className="h-full bg-pink-400 transition-all duration-75"
                  style={{ width: `${Math.min(100, Math.round(audioLevel * 100))}%` }}
                />
              </div>
            </div>
          )}

          {/* 3-bar green battery icon */}
          <div className="flex items-center gap-1.5 bg-neutral-900/80 px-2 py-1 rounded border border-neutral-800 text-[10px] font-mono-retro text-neutral-300">
            <span className="text-[9px] text-neutral-400">PWR</span>
            <div className="w-5 h-2.5 border border-emerald-500/70 p-0.5 flex gap-0.5 rounded-sm">
              <div className="bg-emerald-400 w-1/3 h-full rounded-2xs" />
              <div className="bg-emerald-400 w-1/3 h-full rounded-2xs" />
              <div className="bg-emerald-400 w-1/3 h-full rounded-2xs" />
            </div>
          </div>
        </div>
      </div>

      {/* Optional Top 35mm Sprocket Perforations */}
      {showSprockets && (
        <div className="w-full h-6 bg-[#0f0f12] flex items-center justify-between px-6 overflow-hidden border-b border-neutral-800/80">
          {Array.from({ length: 20 }).map((_, i) => (
            <div
              key={`sprocket-top-${i}`}
              className="w-4 h-3 rounded-xs bg-[#040405] border border-neutral-700/50 shadow-inner shrink-0 mx-0.5"
            />
          ))}
        </div>
      )}

      {/* INNER VIEWPORT: Displays the video/canvas children */}
      <div className="relative w-full aspect-video bg-black overflow-hidden flex items-center justify-center">
        {children}
      </div>

      {/* Optional Bottom 35mm Sprocket Perforations */}
      {showSprockets && (
        <div className="w-full h-6 bg-[#0f0f12] flex items-center justify-between px-6 overflow-hidden border-t border-neutral-800/80">
          {Array.from({ length: 20 }).map((_, i) => (
            <div
              key={`sprocket-bot-${i}`}
              className="w-4 h-3 rounded-xs bg-[#040405] border border-neutral-700/50 shadow-inner shrink-0 mx-0.5"
            />
          ))}
        </div>
      )}

      {/* BOTTOM CHASSIS BAR: Displays broadcast sync & resolution specs */}
      <div className="px-6 py-1.5 bg-[#17171b] border-t border-neutral-800 flex items-center justify-between text-[10px] font-mono-retro text-neutral-400 select-none z-10">
        <div className="flex items-center gap-3">
          <span className="flex items-center gap-1">
            <span className="w-1.5 h-1.5 rounded-full bg-emerald-400 inline-block" />
            SYNC LOCKED
          </span>
          <span className="hidden sm:inline-block">IRIS: f/2.8</span>
          <span className="hidden md:inline-block">SHUTTER: 1/60s</span>
        </div>

        <div className="flex items-center gap-3 text-neutral-400">
          <span className="hidden sm:inline-block">COLOR SYSTEM: NTSC</span>
          <span className="text-amber-400/90 font-semibold">1280 × 720</span>
        </div>
      </div>
    </div>
  );
};
