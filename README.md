# hb-seawolf

Harbour module implementing bindings, playground with graphics, with the use of [OpenGL](https://www.khronos.org/opengl) and [GLFW](https://www.glfw.org/)

### Getting Started

For more info see [getting started](examples/README.md).

## Building

- Build static library:

   ```
   hbmk2 hbseawolf.hbp
   ```

- Build and run sample and test code:

   ```
   hbmk2 main.prg
   ./main
   ```

### Examples

``` harbour
#include "hbseawolf.ch"

PROCEDURE Main()

   LOCAL w
   LOCAL text := "Hello Window"
   LOCAL x1, y1, x2, y2
   LOCAL i
   LOCAL n := 46, radius := 100

   w := sw_CreateWindow( 830, 450, text )

   WHILE( ! sw_MainLoop( w ) .AND. ! sw_GetKey( w, KEY_ESCAPE ) )

      sw_Begin( w )

         x2 := sw_WinWidth( w ) / 2
         y2 := sw_WinHeight( w ) / 2

         sw_Background( w, 0x323232 )

         FOR i := 1 TO 2 * n
            x1 := sin(        i * M_PI / n ) * radius * 2 + x2
            y1 := cos( M_PI + i * M_PI / n ) * radius * 2 + y2
            sw_Lines( w, x1, y1, x2, y2, 0xfecb0b )
         NEXT

      sw_End( w )

      sw_WaitEvents( w )

   ENDDO

   sw_CloseWindow( w )

RETURN
```

![This is an image](examples/main/main.png)

