/*
 *
 */

#ifndef HBSEAWOLF_H_
#define HBSEAWOLF_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hbapi.h"
#include "hbapierr.h"
#include "hbapiitm.h"
#include "hbapistr.h"
#include "hbstack.h"

#include "hbseawolf.ch"

#include <GLFW/glfw3.h>

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

typedef enum bool
{
   F = 0,
   T = ( ! 0 )
} bool;

typedef struct _SeaWolf
{
   GLFWwindow      *window;      // handle of the created window
   int              width;       // desired width, in screen coordinates, of the window
   int              height;      // desired height, in screen coordinates
   int              tmp_width;
   int              tmp_height;
   const char      *title;       // initial, UTF-8 encoded window title
   bool             closeFlag;   // returns the value of the close flag of the specified window
   // cursor
   double           cursorX;     // new cursor x-coordinate, relative to the left edge of the content area
   double           cursorY;     // new cursor y-coordinate, relative to the top edge of the content area
   // key
   int              keyKey;      // the keyboard key that was pressed or released
   int              keyScancode; // the system-specific scancode of the key
   int              keyAction;   // GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. Future releases may add more actions
   int              keyMods;     // bit field describing which modifier keys were held down
   // mouse button
   int              mouseButton; // mouse button that was pressed or released
   int              mouseAction; // one of GLFW_PRESS or GLFW_RELEASE, future releases may add more actions
   int              mouseMods;   // bit field describing which modifier keys were held down
   // window maximized
   int              winMaximized;// GLFW_TRUE if the window was maximized, or GLFW_FALSE if it was restored
   // color
   int              background;
   // OpenGL

} SeaWolf, *pSeaWolf;

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

SeaWolf *hb_SeaWolf_Param( int iParam );
SeaWolf *hb_SeaWolf_ParamGet( int iParam );
SeaWolf *hb_SeaWolf_ItemGet( PHB_ITEM pItem );
PHB_ITEM hb_SeaWolf_ItemPut( PHB_ITEM pItem, SeaWolf *pSeaWolf );
void     hb_SeaWolf_ItemClear( PHB_ITEM pItem );
void     hb_SeaWolf_Return( SeaWolf *pSeaWolf );
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// macros
#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UNUSED( n ) ( ( void )( n ) )
#define LEN( n ) ( sizeof( n ) / sizeof( n )[ 0 ] )

#define HB_ERR_ARGS() ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

#endif /* End HBSEAWOLF_H_ */
