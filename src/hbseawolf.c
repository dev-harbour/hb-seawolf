/*
 *
 */

#include "hbseawolf.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* SeaWolf */
static HB_GARBAGE_FUNC( hb_seawolf_Destructor )
{
   SeaWolf **ppSeaWolf = ( SeaWolf ** ) Cargo;

   if( *ppSeaWolf )
   {
      *ppSeaWolf = NULL;
   }
}

static const HB_GC_FUNCS s_gcSeaWolfFuncs =
{
   hb_seawolf_Destructor,
   hb_gcDummyMark
};

SeaWolf *hb_SeaWolf_Param( int iParam )
{
   SeaWolf **ppSeaWolf = ( SeaWolf ** ) hb_parptrGC( &s_gcSeaWolfFuncs, iParam );

   if( ppSeaWolf && *ppSeaWolf )
   {
      return *ppSeaWolf;
   }
   else
   {
      HB_ERR_ARGS();
      return NULL;
   }
}

SeaWolf *hb_SeaWolf_ParamGet( int iParam )
{
   SeaWolf **ppSeaWolf = ( SeaWolf ** ) hb_parptrGC( &s_gcSeaWolfFuncs, iParam );

   return ppSeaWolf ? *ppSeaWolf : NULL;
}

SeaWolf *hb_SeaWolf_ItemGet( PHB_ITEM pItem )
{
   SeaWolf **ppSeaWolf = ( SeaWolf ** ) hb_itemGetPtrGC( pItem, &s_gcSeaWolfFuncs );

   return ppSeaWolf ? *ppSeaWolf : NULL;
}

PHB_ITEM hb_SeaWolf_ItemPut( PHB_ITEM pItem, SeaWolf *pSeaWolf )
{
   SeaWolf **ppSeaWolf = ( SeaWolf ** ) hb_gcAllocate( sizeof( SeaWolf *), &s_gcSeaWolfFuncs );
   *ppSeaWolf = pSeaWolf;
   return hb_itemPutPtrGC( pItem, ppSeaWolf );
}

void hb_SeaWolf_ItemClear( PHB_ITEM pItem )
{
   SeaWolf **ppSeaWolf = ( SeaWolf ** ) hb_itemGetPtrGC( pItem, &s_gcSeaWolfFuncs );

   if( ppSeaWolf )
   {
      *ppSeaWolf = NULL;
   }
}

void hb_SeaWolf_Return( SeaWolf *pSeaWolf )
{
   if( pSeaWolf )
   {
      hb_SeaWolf_ItemPut( hb_param( -1, HB_IT_ANY ), pSeaWolf );
   }
   else
   {
      hb_ret();
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static functions
static void cursor_position_callback( GLFWwindow *window, double x, double y )
{
   pSeaWolf s_w = glfwGetWindowUserPointer( window );

   s_w->cursorX = x;
   s_w->cursorY = y;
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   pSeaWolf s_w = glfwGetWindowUserPointer( window );

   s_w->keyKey      = key;
   s_w->keyScancode = scancode;
   s_w->keyAction   = action;
   s_w->keyMods     = mods;

   if( action != GLFW_PRESS )
      return;

   switch( key )
   {
      case KEY_PRINT_SCREEN:
         break;
   }
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
   pSeaWolf s_w = glfwGetWindowUserPointer( window );

   s_w->mouseButton = button;
   s_w->mouseAction = action;
   s_w->mouseMods   = mods;
}

static void window_maximize_callback( GLFWwindow *window, int maximized )
{
   pSeaWolf s_w = glfwGetWindowUserPointer( window );

   s_w->winMaximized = maximized;
}

static void hex_to_ClearColor( uint32_t hexColor )
{
   double r, g, b;
   r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
   g = ( ( hexColor >> 8  ) & 0xFF ) / 255.0;
   b = (   hexColor         & 0xFF ) / 255.0;

   glClearColor( r, g, b, 1.0 );
}

static void hex_to_Color3f( uint32_t hexColor )
{
   float r, g, b;
   r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
   g = ( ( hexColor >> 8  ) & 0xFF ) / 255.0;
   b = (   hexColor         & 0xFF ) / 255.0;

   glColor3f( r, g, b );
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
//pSeaWolf sw_CreateWindow( int width, int height, const char *title )
HB_FUNC( SW_CREATEWINDOW )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL &&  hb_param( 3, HB_IT_STRING ) != NULL )
   {
      pSeaWolf w;

      if( ! glfwInit() )
      {
         exit( EXIT_FAILURE );
      }

      w = calloc( 1, sizeof( SeaWolf ) );

      w->width  = hb_parni( 1 );
      w->height = hb_parni( 2 );
      w->title  = hb_parc( 3 );

      w->window = glfwCreateWindow( w->width, w->height, w->title, NULL, NULL );
      if( ! w->window )
      {
         free( w );
         glfwTerminate();
         exit( EXIT_FAILURE );
      }
      glfwMakeContextCurrent( w->window );
      glfwSetWindowUserPointer( w->window, w );

      glfwSetCursorPosCallback( w->window, cursor_position_callback );
      glfwSetKeyCallback( w->window, key_callback );
      glfwSetMouseButtonCallback( w->window, mouse_button_callback );
      glfwSetWindowMaximizeCallback( w->window, window_maximize_callback );

      glfwSetWindowSizeLimits( w->window, w->width, w->height, GLFW_DONT_CARE , GLFW_DONT_CARE );

      hb_SeaWolf_Return( w );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, NULL, "sw_CreateWindow", 3, hb_paramError( 1 ), hb_paramError( 2 ), hb_paramError( 3 ) );
   }
}

//bool sw_MainLoop( pSeaWolf w )
HB_FUNC( SW_MAINLOOP )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );

   if( w )
   {
      w->closeFlag = glfwWindowShouldClose( w->window );
      glfwSetWindowShouldClose( w->window, GLFW_FALSE );

      hb_retl( w->closeFlag );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, NULL, "sw_MainLoop", 1, hb_paramError( 1 ) );
   }
}

// bool sw_CloseWindow( pSeaWolf w )
HB_FUNC( SW_CLOSEWINDOW )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );

   if( w )
   {
      glfwDestroyWindow( w->window );
      free( w );
      glfwTerminate();

      exit( EXIT_SUCCESS );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, NULL, "sw_CloseWindow", 1, hb_paramError( 1 ) );
   }
}

// void begin_drawing( pSeaWolf w )
HB_FUNC( BEGIN_DRAWING )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );

   if( w )
   {
      glfwGetFramebufferSize( w->window, &w->width, &w->height );

      glViewport( 0, 0, w->width, w->height );
      hex_to_ClearColor( w->background );
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glDisable( GL_CULL_FACE );
      glDisable( GL_DEPTH_TEST );
      glDisable( GL_BLEND );

      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      glOrtho( 0, w->width, w->height, 0, -1, 1 );
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();

      glEnable( GL_TEXTURE_2D );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, NULL, "sw_Begin", 1, hb_paramError( 1 ) );
   }
}

// void end_drawing( pSeaWolf w )
HB_FUNC( END_DRAWING )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );

   if( w )
   {
      glfwSwapBuffers( w->window );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, NULL, "sw_End", 1, hb_paramError( 1 ) );
   }
}

// int opengl_functions( pSeaWolf w, iOpenGl type, int par1, int par2, int par3, int par4, int par5 )
HB_FUNC( OPENGL_FUNCTIONS )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );
   int par1 = hb_parni( 3 );
   int par2 = hb_parni( 4 );
   int par3 = hb_parni( 5 );
   int par4 = hb_parni( 6 );
   int par5 = hb_parni( 7 );

   int ret = 1;

   switch( hb_parni( 2 ) )
   {
   case OPENGL_BACKGROUND:

      w->background = par1;
      break;

   case OPENGL_POINT:

      hex_to_Color3f( par3 );
      glBegin( GL_POINTS );
      glVertex2f( par1, par2 +1 );
      glEnd();
      break;

   case OPENGL_POINT_SIZE:

      hex_to_Color3f( par4 );
      glPointSize( par3 );
      glBegin( GL_POINTS );
      glVertex2f( par1, par2 +1 );
      glEnd();
      break;

   case OPENGL_LINES:

      hex_to_Color3f( par5 );
      glBegin( GL_LINES );
      glVertex2f( par1, par2 );
      glVertex2f( par3 +1, par4 +1 );
      glEnd();
      break;

   case OPENGL_FILLRECT:

      hex_to_Color3f( par5 );
      glBegin( GL_POLYGON );
      glVertex2f( par1, par2 );
      glVertex2f( par3, par2 );
      glVertex2f( par3, par4 );
      glVertex2f( par1, par4 );
      glEnd();
      break;

   default:
      hb_retni( 0 );
   }
   hb_retni( ret );
}

//int text_functions( pSeaWolf w, iText type, const char *par1, int par2, int par3, int par4 )
HB_FUNC( TEXT_FUNCTIONS )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );
   int par1 = hb_parni( 3 );
   int par2 = hb_parni( 4 );
   int par3 = hb_parni( 5 );
   int par4 = hb_parni( 6 );

   int ret = 1;
   UNUSED( w );
   UNUSED( par1 );
   UNUSED( par2 );
   UNUSED( par3 );
   UNUSED( par4 );

   switch( hb_parni( 2 ) )
   {
   case TEXT_FONT:
      break;

   case TEXT_DISPOUTAT:
      break;

   default:
      hb_retni( 0 );
   }
   hb_retni( ret );
}

// int glfw_functions( pSeaWolf w, iGlfw type, int par1 )
HB_FUNC( GLFW_FUNCTIONS )
{
   SeaWolf *w = hb_SeaWolf_Param( 1 );

   int type = hb_parni( 2 );
   int par1 = hb_parni( 3 );
   int ret = 1;

   switch( type )
   {

   case GLFW_KEYBOARD:

      if( w->keyAction != GLFW_PRESS )
      break;;

      ret = w->keyKey;
      break;

   case GLFW_GET_KEY:

      ret = ( w->keyKey == par1 ) == GLFW_PRESS ? T : F;
      break;

   case GLFW_GET_MOUSEBUTTON:

      ret = ( w->mouseButton == par1 ) == GLFW_PRESS ? T : F;
      break;

   case GLFW_WIN_WIDTH:

      ret = w->width;
      break;

   case GLFW_WIN_HEIGHT:

      ret = w->height;
      break;

   case GLFW_WIN_MAXCOL:

      ret = w->width / 9;
      break;

   case GLFW_WIN_MAXROW:

      ret = w->height / 18;
      break;

   case GLFW_WIN_MAXIMIZED:

      glfwMaximizeWindow( w->window );
      ret = w->winMaximized;
      break;

   case GLFW_POLLEVENTS:

      glfwPollEvents();
      break;

   case GLFW_WAITEVENTS:

      glfwWaitEvents();
      break;

   case GLFW_WAITEVENTSTIMEOUT:

      glfwWaitEventsTimeout( ( double ) par1 );
      break;

   default:

      hb_retni( 0 );
   }

   if( GLFW_GET_KEY == type || GLFW_GET_MOUSEBUTTON == type || GLFW_WIN_MAXIMIZED == type )
   {
      hb_retl( ret );
   }
   else
   {
      hb_retni( ret );
   }
}
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* Trigonometric functions */
// ACOS()
// ASIN()
// ATAN()
// ATN2()
// CEILING()

// COS()
HB_FUNC( COS )
{
   PHB_ITEM pItem;

   if( ( pItem = hb_param( 1, HB_IT_NUMERIC ) ) != NULL )
   {
      hb_retnd( cos( hb_itemGetND( pItem ) ) );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, "Expected a numeric argument", "Cos()", 1, hb_paramError( 1 ) );
   }
}

// COT()
// DTOR()
// FACT()
// FLOOR()
// FV()
// GETPREC()
// LOG10()
// PAYMENT()
// PERIODS()
// PI()
// PV()
// RATE()
// RTOD()
// SETPREC()
// SIGN()

// SIN()
HB_FUNC( SIN )
{
   PHB_ITEM pItem;

   if( ( pItem = hb_param( 1, HB_IT_NUMERIC ) ) != NULL )
   {
      hb_retnd( sin( hb_itemGetND( pItem ) ) );
   }
   else
   {
      hb_errRT_BASE( EG_ARG, 2023, "Expected a numeric argument", "Sin()", 1, hb_paramError( 1 ) );
   }
}

// TAN()