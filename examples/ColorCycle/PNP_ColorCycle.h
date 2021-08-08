// What Effect Does:
//
//    Cycles through specific choices of colors (hue and whiteness).
//    The brightness is not affected.
//
// Calling trigger():
//
//    Not implemented.
//
// Calling nextstep():
//
//    Changes to next color after 'count' number of steps.
//
// Properties Used:
//
//    pixCount - determines number of steps before color change.
//
// Properties Affected:
//
//    degreeHue, pcentWhite - modified after so many calls to nextstep().
//

class PNP_ColorCycle : public PixelNutPlugin
{
public:
  byte gettype(void) const
  {
    return 0;
  };

  void begin(byte id, uint16_t pixlen)
  {
      max = 0;
      count = 1;
      index = 0;
  }

  void nextstep(PixelNutHandle handle, PixelNutSupport::DrawProps *pdraw)
  {
    if (max == 0) max = pdraw->pixCount * 3;

    if (count++ >= max)
    {
      pdraw->degreeHue = hues[index];
      pdraw->pcentWhite = whites[index];
      pixelNutSupport.makeColorVals(pdraw);

      count = 1;
      if (++index >= 3) index = 0;
    }
  }

private:
    uint16_t count, max;
    uint16_t index;
    // cycles through Red, White, Blue
    uint16_t hues[3]   = { 0,   0,   240 };
    uint16_t whites[3] = { 0,   100, 0   };
};
