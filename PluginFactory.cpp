// PixelNutApp Plugin Factory
// Class to create instances of plugins.
// Uses the PixelNutSupport and PixelNutComets Interfaces.
/*
    Copyright (c) 2015-2017, Greg de Valois
    Software License Agreement (BSD License)
    See license.txt for the terms of this license.
*/

#include "PixelNutLib.h"

#include "plugins/PNP_DrawAll.h"
#include "plugins/PNP_DrawPush.h"
#include "plugins/PNP_DrawStep.h"
#include "plugins/PNP_LightWave.h"
#include "plugins/PNP_CometHeads.h"
#include "plugins/PNP_FerrisWheel.h"
#include "plugins/PNP_BlockScanner.h"
#include "plugins/PNP_Twinkle.h"
#include "plugins/PNP_Blinky.h"
#include "plugins/PNP_Noise.h"
#include "plugins/PNP_HueSet.h"
#include "plugins/PNP_HueRotate.h"
#include "plugins/PNP_ColorMeld.h"
#include "plugins/PNP_ColorModify.h"
#include "plugins/PNP_ColorRandom.h"
#include "plugins/PNP_CountSet.h"
#include "plugins/PNP_DelaySurge.h"
#include "plugins/PNP_BrightWave.h"
#include "plugins/PNP_WinExpander.h"
#include "plugins/PNP_FlipDirection.h"

extern PluginFactoryCore *pPluginFactory; // use externally declared pointer to instance

PixelNutPlugin *PluginFactoryCore::makePlugin(int plugin)
{
  switch (plugin)
  {
    // drawing effects:

    case 0:   return new PNP_DrawAll;                     // draws current color to all pixels
    case 1:   return new PNP_DrawPush;                    // draws current color one pixel at a time, inserting at the head
    case 2:   return new PNP_DrawStep;                    // draws current color one pixel at a time, advancing each time

    case 10:  return new PNP_LightWave;                   // light waves (brighness changes) that move; count property sets wave frequency
    case 20:  return new PNP_CometHeads;                  // creates "comets": moving head with tail that fades, trigger creates new head
    case 30:  return new PNP_FerrisWheel;                 // rotates "ferris wheel spokes" around; count property sets spaces betwen spokes
    case 40:  return new PNP_BlockScanner;                // moves color block back and forth; block size set from count property

    case 50:  return new PNP_Twinkle;                     // scales light levels individually up and down
    case 51:  return new PNP_Blinky;                      // blinks on and off random pixels at full brightness
    case 52:  return new PNP_Noise;                       // sets random pixels/brightness with current color

    default:  return NULL;
  }
}

PixelNutPlugin *PluginFactoryAdv::makePlugin(int plugin)
{
  switch (plugin)
  {
    // predraw effects:

    case 100: return new PNP_HueSet;                      // force directly sets the color hue property value once when triggered
    case 101: return new PNP_HueRotate;                   // rotates color hue on each step; amount of change set from trigger force

    case 110: return new PNP_ColorMeld;                   // smoothly melds between colors when they change
    case 111: return new PNP_ColorModify;                 // force modifies both the color hue/white properties once when triggered
    case 112: return new PNP_ColorRandom;                 // sets color hue/white to random values on each step (doesn't use force)

    case 120: return new PNP_CountSet;                    // force directly sets the count property value once when triggered

    case 131: return new PNP_DelaySurge;                  // force decreases delay then evenly reverts to original value
                                                          // (this must be triggered periodically for a continuous effect)

    case 142: return new PNP_BrightWave;                  // force determines the number of steps that modulates brightness

    case 150: return new PNP_WinExpander;                 // expands/contracts drawing window that stays centered on strip

    case 160: return new PNP_FlipDirection;               // toggles the drawing direction on each trigger

    default:  return PluginFactoryCore::makePlugin(plugin);
  }
}

// must provide destructor for plugin abstract (interface) base class
PixelNutPlugin::~PixelNutPlugin() {}
