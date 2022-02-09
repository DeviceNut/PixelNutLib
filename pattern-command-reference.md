Pattern Command Reference
===============================================================

The following is a reference for the various pattern commands, in alphabetical order.

See the file 'how-patterns-work.md' for a discussion of how all of this works together to create effects. You should also be familiar with the interface methods defined in the source file 'PixelNutEngine.h' and 'PixelNutPlugins.h'.


A[<byteval>]
---------------------------------------------------------------
Assigns the trigger source for an effect to a particular layer. This allows one layer to trigger another.

If <byteval> is missing it will default to 0. This specifies the layer that will trigger the current layer being defined.

For example, if on the first layer you use 'A1', then the second layer will trigger the first layer.


B[<percent>]
---------------------------------------------------------------
Sets the 'pcentBright' drawing property for the current effect to the value <percent>, which is a percentage from 0-100.

The brightness for all effects are relative to the overall brightness level, which can be set from an application by calling the 'setMaxBrightness' method of the PixelNutSupport class.

For example, if the brightness for a specific plugin is 90%, but the overall brightness has been set to 60%, then the result is an effective brightness of 0.90 * 0.60 = 54%.

If no value is specified the command is ignored. The initial value for this property is 100.


C[<percent>]
---------------------------------------------------------------
Sets the 'pixCount' drawing property for the current effect by mapping the value <percent>, which is a percentage from 0-100, to the total number of pixels in the output pixel array (1...max).

If no value is specified the command is ignored. The pixel count property itself is initially set to 1.

This property is used by many plugins to determine the length or duration of its animation, but can be used for other purposes as well.


D[<byteval>]
---------------------------------------------------------------
Sets the 'msecsDelay' drawing property for the current effect to <byteval>, which is a delay time in milliseconds.

Increasing the delay for an effect increases the amount of time between the calls to an effect plugin's 'nextstep()' method. This is not precise, but guarantees that at least that much time will have elapsed since the previous call.

If no value is specified the command is ignored. The initial value for this property is 0.


E<plugin>
---------------------------------------------------------------
Creates a new effect layer specified by the value <plugin> (see the file 'PluginFactory.cpp' for which numbers have been assigned to which plugin).

Unlike most other commands, leaving out the number causes an error to be returned from the 'execCmdStr()' library method.

Depending on the plugin type, this layer may or may not be an effect track, meaning it will be actually be drawing pixels, instead of just modifying drawing properties.

This plugin for this layer now becomes the current plugin, and all subsequent commands affect the properties of that plugin.


F[<force>]
---------------------------------------------------------------
Sets the force value used in subsequent triggers for an effect plugin, from 0 to 1000. If no value is specified, then each trigger uses a random value in that range.


G
---------------------------------------------------------------
This is the Go command, which activates all previously defined plugin effect layers that haven't been activated before. 

Until this command is used, the layers are not "active", meaning the 'nextstep()' method of the plugins are not called when the application calls 'updateEffects()'.

This is usually put at the end of a pattern string, so that nothing is displayed until the entire pattern has been setup.


H[<degrees>]
---------------------------------------------------------------
Sets the 'degreeHue' drawing property for the current effect with the value <degrees>, which is an angle from 0-359. This sets the hue value in the HSV color model (Hue, Saturation, Value), which is used to create the final 24-bit RGB pixel values.

If no value is specified the command is ignored. The initial value for this property is 0, which is Red.


I
---------------------------------------------------------------
Enables external triggering for a plugin layer. This means that calls to 'triggerForce()' from the application will cause triggering on all plugin layers that have been enabled. This does not affect the automatic triggering that happens with the use of the 'T' command.

The default value is disabled.

N[<byteval>]
---------------------------------------------------------------
Sets the repeat count used in automatic triggering (see the 'T' command) to the value <byteval>, from 0-255. If the value is missing 0 is used, which is the default setting.

This determines how many times the current plugin's 'trigger()' method is called (not including the initial one when 'T' is used). A value of 0 means that the triggering will continue indefinitely.


O[<wordval>]
---------------------------------------------------------------
Sets the default minimum auto triggering time in seconds for the specified plugin effect layer. If no value is specified the default of 1 second is used.


P
---------------------------------------------------------------
Pops all layers off the track/layer stacks.

This is used at the beginning of all preset patterns so that previously loaded effects are removed, allowing the creation of new patterns from scratch instead of having them append commands to the previously one.


Q[<byteval>]
---------------------------------------------------------------
Determines which effect properties can be changed.

It is used to direct external control of these properties to specific tracks, such that changing those values only affects tracks that have corresponding bits set.

Note that this command applies to the entire effect track, and thus to all the effect layers that are assigned to it.

The value in <byteval> is to be interpreted as a bit field, with the bit values defined in the 'ExtControlBit' enumeration in PixelNutEngine.h.

For example, using the 'Q3' command on an effect track indicates that calls to 'setColorProperty()' will set both the color hue and whiteness properties on that track.

In addition, if the application enables the external property mode by calling 'setPropertyMode(true)' (it is false by default), these Q bits inhibit the predraw type of effects assigned to a track from modifying the corresponding property.

Using the 'Q3' example above, when this mode is enabled, any predraw effect that normally would periodically change the color hue wouldn't work, allowing the application to directly set the color instead.


T[<byteval>]
---------------------------------------------------------------
Triggers the current effect layer (calls into the 'trigger()' method of that plugin), and optionally specifies a timer value with <byteval>.

The timer value specifies a time duration in seconds, which is used to trigger the plugin at a random time between the minimum (set with the 'O' command, with default of 1 second) and the additional number of seconds specified by the value here (if 0 then triggering always happens at that minimum).

If no value is specified there is only one trigger. If this command is omitted when creating an effect, then the method 'nextstep()' for that plugin is not called until triggered with a call to 'triggerForce()' by the application (which would need to be enabled by setting the 'I' command for the drawing track).


U[0,1]
---------------------------------------------------------------
Sets the 'goUpwards' drawing property for the current effect to either 0 or 1.

This determines the direction pixels are drawn in, where the UP direction is from the start of the pixel array to the end of the pixel array.

If no value is specified the current value is toggled. The default setting is 1, meaning the UP direction.

This is used internally by the PixelNut Library drawing code when forming the final output pixel values from each effect track.


V[0,1]
---------------------------------------------------------------
Sets the 'orPixelValues' drawing property for the current effect to either 0 or 1.

This determines how the pixel tracks are combined when creating the final output pixel array.

If no value is specified the current value is toggled. The default setting is 0, meaning that the pixels are OR'ed together, instead of being overridden.

This is used internally by the PixelNut Library drawing code when forming the final output pixel values from each effect track.


W[<percent>]
---------------------------------------------------------------
Sets the 'pcentWhite' drawing property for the current effect with the value <percent>, which a percentage from 0-100.

This sets the inverse of the saturation value in the HSV color model (Hue, Saturation, Value), which is used to create the final 24-bit RGB pixel values.

In other words, 0% whiteness is fully saturated, and at 100% whiteness there is no color hue at all. Practically, values above about 50% or so appear almost completely white.

If no value is specified the command is ignored. The initial value for this property is 0.


X[<pixel>]
---------------------------------------------------------------
Sets the starting pixel of a segment. Together with the Y command this defines the range of a segment, after which all commands apply to only the pixels within this range. Pixels are numbered from 0.

If no value is specified then 0 is used, which is also the initial value for this property.


Y[<pixel_count>]
---------------------------------------------------------------
Sets the number of pixels in a segment. Together with the X command this defines the range of a segment, after which all commands apply to only the pixels within this range.

If no value is specified then the total number of pixels in the entire strip is used, which is also the initial value for this property.
