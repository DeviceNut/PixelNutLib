// PixelNut Engine Class and the
// PixelNut Plugin Factory Class
// Uses the PixelNut Support Interface.
/*
    Copyright (c) 2015-2024, Greg de Valois
    Software License Agreement (BSD License)
    See license.txt for the terms of this license.
*/

#pragma once

class PixelNutEngine
{
public:

  enum Status // Returned value from 'execCmdStr()' call below
  {
    Status_Success=0,
    Status_Error_BadVal,        // invalid value used in command
    Status_Error_BadCmd,        // unrecognized/invalid command
    Status_Error_Memory,        // not enough memory for effect
  };

  // These bits are combined to create the value for the 'Q' command for an individual track
  // (drawing effect). It is used to direct external control of the color/count properties
  // to particular tracks, such that changing those controls only affects tracks that have
  // the corresponding bit set.
  //
  // In addition, when the external property mode is enabled with 'setPropertyMode()',
  // a set property bit prevents any predraw effect from changing that property for that track.
  // Otherwise, a set bit allows modification by both external and internal (predraw) sources.
  //
  // By default (no bits set), only predraw effects can change the drawing properties.
  enum ExtControlBit
  {                                 // corresponds to the drawing property:
    ExtControlBit_DegreeHue  = 1,   //  degreeHue
    ExtControlBit_PcentWhite = 2,   //  pcentWhite
    ExtControlBit_PixCount   = 4,   //  pixCount
    ExtControlBit_All        = 7    // all bits ORed together
  };

  // Constructor: init location/length of the pixels to be drawn, 
  // the first pixel to start drawing and the direction of drawing,
  // and the maximum effect layers and tracks that can be supported.
  // num_layers/tracks *must not* be greater than MAX_TRACK_LAYER.
  PixelNutEngine(byte *ptr_pixels, uint16_t num_pixels, bool goupwards=true,
                 short num_layers=4, short num_tracks=3);

  void setMaxBrightness(byte percent) { pcentBright = percent; }
  byte getMaxBrightness() { return pcentBright; }

  void setDelayOffset(int8_t msecs) { delayOffset = msecs; }
  int8_t getDelayOffset() { return delayOffset; }

  // Sets the color properties for tracks that have set either the ExtControlBit_DegreeHue
  // or ExtControlBit_PcentWhite bits. These values can be individually controlled. The
  // 'hue_degree' is a value from 0...MAX_DEGREES_CIRCLE, and the 'white_percent' value
  // is a percentage from 0...MAX_PERCENTANGE.
  void setColorProperty(short hue_degree, byte white_percent);

  // Sets the pixel count property for tracks that have set the ExtControlBit_PixCount bit.
  // The 'pixcount_percent' value is a percentage from 0...MAX_PERCENTAGE.
  void setCountProperty(byte pixcount_percent);

  // When enabled, predraw effects are prevented from modifying the color/count properties
  // of a track with the corresponding ExtControlBit bit set, allowing only the external
  // control of that property with calls to set..Property().
  void setPropertyMode(bool enable);

  // Retrieves the external property mode settings
  bool  getPropertyMode()    { return externPropMode;   }
  short getPropertyHue()     { return externDegreeHue;  }
  byte  getPropertyWhite()   { return externPcentWhite; }
  byte  getPropertyCount()   { return externPcentCount; }

  // Triggers effect layers with a range value of -MAX_FORCE_VALUE..MAX_FORCE_VALUE.
  // (Negative values are not utilized by most plugins: they take the absolute value.)
  // Must be enabled with the "I" command for each effect layer to be effected.
  void triggerForce(short force);

  // Used by plugins to trigger based on the effect layer, enabled by the "A" command.
  void triggerForce(byte layer, short force);

  // Called by the above and DoTrigger(), CheckAutoTrigger(), allows override
  virtual void triggerLayer(byte layer, short force);

  // Parses and executes a command string, returning a status code.
  // An empty string (or one with only spaces), is ignored.
  virtual Status execCmdStr(char *cmdstr);

  // Pops off all layers from the stack
  virtual void clearStack(void);

  // Updates current effect: returns true if the pixels have changed and should be redisplayed.
  virtual bool updateEffects(void);

  // Private to the PixelNutSupport class and main application.
  byte *pDrawPixels; // current pixel buffer to draw into or display
  // Note: test this for NULL after constructor to check if successful!

protected:

  byte pcentBright = MAX_PERCENTAGE;            // max percent brightness to apply to each effect
  int8_t delayOffset = 0;                       // additional delay to add to each effect (msecs)
                                                // this is kept to be +/- 'DELAY_RANGE'

  typedef struct ATTR_PACKED // 18-20 bytes
  {
                                                // auto triggering information:
    uint32_t trigTimeMsecs;                     // time of next trigger in msecs (0 if not set yet)
    int16_t trigCount;                          // number of times to trigger (-1 to repeat forever)
    uint16_t trigDelayMin;                      // min amount of delay before next trigger in seconds
    uint16_t trigDelayRange;                    // range of delay values possible (min...min+range)

                                                // these apply to both auto and manual triggering:
    short trigForce;                            // amount of force to apply (-1 for random)
    bool trigActive;                            // true if this layer has been triggered at least once
    bool trigExtern;                            // true if external triggering is enabled for this layer
    byte trigSource;                            // what other layer can trigger this layer (255 for none)

    byte track;                                 // index into properties stack for plugin
    PixelNutPlugin *pPlugin;                    // pointer to the created plugin object
  }
  PluginLayer; // defines each layer of effect plugin

  typedef struct ATTR_PACKED // 30-32 bytes
  {
    uint32_t msTimeRedraw;                      // time of next redraw of plugin in msecs
    byte *pRedrawBuff;                          // allocated buffer or NULL for postdraw effects

    PixelNutSupport::DrawProps draw;            // redraw properties for this plugin

    byte layer;                                 // index into layer stack to redraw effect
    byte ctrlBits;                              // bits to control setting property values

    // used for logical segment control:
    byte segIndex;                              // assigned to this segment (from 0)
    byte disable;                               // non-zero to disable controls

                                                // for logical segments only:
    uint16_t segOffset;                         // output display buffer offset
    uint16_t segCount;                          // number of pixels to display
  }
  PluginTrack; // defines properties for each drawing plugin

  PluginLayer *pluginLayers;                    // plugin layers that creates effect
  short maxPluginLayers;                        // max number of layers possible
  short indexLayerStack  = -1;                  // index into the plugin layers stack
  short indexTrackEnable = -1;                  // higher indices are not yet activated

  PluginTrack *pluginTracks;                    // plugin tracks that have properties
  short maxPluginTracks;                        // max number of tracks possible
  short indexTrackStack = -1;                   // index into the plugin properties stack

  uint32_t timePrevUpdate = 0;                  // time of previous call to update

  bool goUpwards = true;                        // true to draw from start to end, else reverse
  short curForce = MAX_FORCE_VALUE/2;           // saves last settings to use on new patterns
  
  uint16_t numPixels;                           // total number of pixels in output display
  byte *pDisplayPixels;                         // pointer to actual output display pixels

  uint16_t segOffset;                           // offset in output buffer of current segment
  uint16_t segCount;                            // number of pixels to draw for current segment

  bool externPropMode = false;                  // true to allow external control of properties
  short externDegreeHue;                        // externally set values property values
  byte externPcentWhite;
  byte externPcentCount;

  void SetPropColor(void);
  void SetPropCount(void);
  void RestorePropVals(PluginTrack *pTrack, uint16_t pixCount, uint16_t degreeHue, byte pcentWhite);

  Status NewPluginLayer(int plugin, int segnum);

  void CheckAutoTrigger(bool rollover);
};

class PluginFactory
{
  public: virtual PixelNutPlugin *makePlugin(int plugin);
};
