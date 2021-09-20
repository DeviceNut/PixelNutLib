How PixelNut Effect Patterns Work
===============================================================

An effect pattern is simply a string of characters, which is parsed by the PixelNut Engine for commands to be executed with the 'execCmdStr()' method (defined in 'PixelNutEngine.h').

Review the file 'library-design.md' for more information on the overall architecture of the PixelNutLib library. For more details on each command, see the file 'pattern-command-reference.md'.

Each command is a letter ('A'-'Z'), a numeric value depending on the specific command, and separated from other commands by one or more spaces.

The notation used for the command definitions use the following conventions and symbolic names:

{}                  indicates a required selection
[]                  indicates an optional selection

<percent>           {0...100}
<degrees>           {0...359}
<byteval>           {0...255}
<wordval>           {0...65535}
<force>             {0...1000}
<plugin>            {0...MAX_PLUGIN_VALUE}
<layer>             {0...<number_of_layers>-1}
<layer_count>       {1...<number_of_layers>}
<pixel>             {0...<number_of_pixels-1>}
<pixel_count>       {1...<number_of_pixels>}


Command Definitions
---------------------------------------------------------------

Effect plugins are specified with the 'E' command and its identifier (see the source file 'PluginFactory.cpp' for the mapping of values to effect plugins).

E<plugin>           Creates new layer using specified plugin.

The following commands set the drawing properties, determining how, when, and where pixels are drawn:

                    Sets drawing property:
H<degrees>            color hue degrees
W<percent>            whiteness percent
B<percent>            brightness percent.
C<percent>            pixel count percent
D<byteval>            delay in milliseconds
U[0,1]                direction up/down
V[0,1]                layer pixel value OR'ed or overwritten
X<pixel>              defines starting pixel of a segment
Y<pixel_count>        sets number of pixels in the segment
Z<pixel>              sets first pixel to starting drawing


These commands control and affect how triggering works for one particular plugin layer:

A[<byteval>]        Assigns trigger source for effect.
F[<force>]          Sets force value used in triggering.
N[<wordval>]        Sets repeat count used in triggering.
O[<wordval>]        Sets min auto triggering time in seconds.
T[<wordval>]        Causes trigger, with optional timer value.

Note that pattern strings usually start with the 'P' command to clear all previous effect layers off of the stack.

The 'I' command enables external triggering for the specified plugin layer. Otherwise, calls to the 'triggerForce()' library method from applications will be ignored for that effect layer.

The 'Q<byteval>' command determines which drawing properties get changed when calling 'setColorProperty()', 'setCountProperty()' library methods from applications. If none of those calls are used, then this command can be ignored.

The 'M[<layer>]' command is only useful for applications that build patterns on the fly with multiple calls to 'execCmdStr()', or that edit patterns interactively with the user.

Finally, the last command in patterns strings is usually 'G', which activates all previously defined tracks.

Until this command is used, the effect layers are not 'active', meaning the 'nextstep()' method of the plugins are not called when the application calls 'updateEffects()'.


LightWave Example
---------------------------------------------------------------

Let's examine an effect pattern string in detail. This is the 'PATTERN_LIGHT_WAVE' pattern from the 'MyPatterns.h' file from any of the PixelNut product applications.

'P E10 D60 Q7 T E101 I T E120 F250 I T G'

1. 'P' clears the effect stack.

2. 'E10' creates the LightWave drawing effect plugin. There is now one layer on the stack. This layer is also a track, as it actually draws to its own pixel array. Note that this plugin uses the pixel count drawing property to determine how many pixels are to be drawn.

3. 'D60' sets the delay property for that track to 60 milliseconds.

4. 'Q7' allows the application to modify all of the drawing properties for that track.

5. 'T' triggers this first effect layer. Since the LightWave plugin doesn't actually have a 'trigger()' method implemented, the triggering doesn't actually do anything, but it does enable future calls to its 'nextstep()' method.

6. 'E101' creates the HueRotate effect plugin, which is a predraw effect type of plugin. What it does is to change the color hue drawing property each time its 'nextstep()' method is called. There are now 2 layers on the stack, but still only 1 track.

7. 'I' enables external triggering for this layer.

8. 'T' triggers this second effect layer. This immediately calls the 'trigger()' method of the HueRotate plugin, which sets the number of degrees the color hue will get changed each time. But because no 'F' command has been used, the force value is 0, and the color hue won't change until there is an external trigger.

9. 'E120' creates the CountSet predraw effect plugin, which sets the pixel count property from the force passed into every call to 'trigger()'. Note that this effect doesn't have a 'nextstep()' method at all, and only does work when triggered. This is now the 3rd layer on the stack, with still only 1 track.

10. 'F250' sets the force that will be passed into the calls to 'trigger()' for the CountSet plugin.

11. 'I' enables external triggering for this layer.

12. 'T' now triggers the CountSet plugin, using the force of 250. Since the maximum force is 1000, a force of 250 is 25% of the maximum, and so the pixel count property will be set to 25% of the number of pixels in the entire pixel strip.

13. 'G' activates all of the 3 layers on the stack.

Now, on each call to 'updateEffects()' by the application, the 'nextstep()' methods for each predraw layer for each track are called first, then for the track layer itself.

This creates an animation of red light waves moving down the strip. The initial color is red because no 'H' command was specified, and that is the default color.

When the application calls the 'triggerForce()' method, the 'trigger()' methods for both predraw effect plugins are called, which changes both the pixel count property (by the CountSet plugin), and the color hue property (by the RotateHue plugin), causing the LightWave to start drawing waves using the new color and length on subsequent calls to 'nextstep()'. The force that is passed by the application determines how much the color and count properties are changed with the trigger.


Execution Errors
---------------------------------------------------------------

Errors cause the remainder of the command string to be ignored, with the PixelNut Engine method 'execCmdStr()' returning something other than 'Status_Success' (see 'PixelNutEngine.h').

1. The command is not listed above. Returns 'Status_BadCmd'.

2. No layers and the command is not 'E': returns 'Status_BadCmd'.

3. No layers and the plugin specified in the 'E' command is not a 'ReDraw' type of plugin: returns 'Status_BadCmd'.

4. If the <plugin> value for the 'E' command is not associated with a plugin effect: returns 'Status_BadVal'.D