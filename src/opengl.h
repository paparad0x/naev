/*
 * See Licensing and Copyright notice in naev.h
 */


#ifndef OPENGL_H
#  define OPENGL_H

#include <stdint.h>

#define GL_GLEXT_PROTOTYPES

#include "SDL.h"
#include "SDL_opengl.h"

#include "physics.h"
#include "colour.h"

#include "ncompat.h"

/*
 * We put all the other opengl stuff here to only have to include one header.
 */
#include "opengl_ext.h"
#include "opengl_tex.h"
#include "opengl_matrix.h"
#include "opengl_vbo.h"
#include "opengl_render.h"
#include "opengl_shader.h"


/* Recommended for compatibility and such */
#if HAS_BIGENDIAN
#  define RMASK   0xff000000 /**< Red bit mask. */
#  define GMASK   0x00ff0000 /**< Green bit mask. */
#  define BMASK   0x0000ff00 /**< Blue bit mask. */
#  define AMASK   0x000000ff /**< Alpha bit mask. */
#else
#  define RMASK   0x000000ff /**< Red bit mask. */
#  define GMASK   0x0000ff00 /**< Green bit mask. */
#  define BMASK   0x00ff0000 /**< Blue bit mask. */
#  define AMASK   0xff000000 /**< Alpha bit mask. */
#endif
#define RGBAMASK  RMASK,GMASK,BMASK,AMASK


/*
 * Contains info about the opengl screen
 */
#define OPENGL_FULLSCREEN  (1<<0) /**< Fullscreen. */
#define OPENGL_DOUBLEBUF   (1<<1) /**< Doublebuffer. */
#define OPENGL_VSYNC       (1<<2) /**< Sync to monitor vertical refresh rate. */
#define gl_has(f)    (gl_screen.flags & (f)) /**< Check for the flag */
/**
 * @brief Stores data about the current opengl environment.
 */
typedef struct glInfo_ {
   int desktop_w; /**< Desktop width. */
   int desktop_h; /**< Desktop height. */
   int x; /**< X offset of window viewport. */
   int y; /**< Y offset of window viewport. */
   /* Viewport considers x/y offset. */
   int w; /**< Window viewport width. */
   int h; /**< Window viewport height. */
   /* Scaled window is the effective window resolution without considering offsets. */
   int nw; /**< Scaled window width. */
   int nh; /**< Scaled window height. */
   /* Real window resolution is the real window resolution, unscaled and without offsets. */
   int rw; /**< Real window width. */
   int rh; /**< Real window height. */
   double scale; /**< Scale factor. */
   double wscale; /**< Width scale factor. */
   double hscale; /**< Height scale factor. */
   double mxscale; /**< Mouse X scale factor. */
   double myscale; /**< Mouse y scale factor. */
   int depth; /**< Depth in bpp */
   int r; /**< How many red bits we have. */
   int g; /**< How many green bits we have. */
   int b; /**< How many blue bits we have. */
   int a; /**< How many alpha bits we have. */
   unsigned int flags; /**< Stores different properties */
   int tex_max; /**< Maximum texture size */
   int multitex_max; /**< Maximum multitexture levels */
   int fsaa; /**< Full Scene Anti Aliasing level. */
   SDL_Window *window; /**< Window for SDL2. */
   SDL_GLContext context; /**< Context for OpenGL. */
} glInfo;
extern glInfo gl_screen; /* local structure set with gl_init and co */

extern gl_Matrix4 gl_view_matrix;

#define  SCREEN_X gl_screen.x /**< Screen X offset. */
#define  SCREEN_Y gl_screen.y /**< Screen Y offset. */
#define  SCREEN_W gl_screen.w /**< Screen width. */
#define  SCREEN_H gl_screen.h /**< Screen height. */


/*
 * used with colour.h
 */
#define COLOUR(x)    glColor4d((x).r,(x).g,(x).b,(x).a) /**< Change colour. */
#define ACOLOUR(x,a) glColor4d((x).r,(x).g,(x).b,a) /**< Change colour and override alpha. */


/*
 * initialization / cleanup
 */
int gl_init (void);
void gl_exit (void);
void gl_resize( int w, int h );


/*
 * Extensions and version.
 */
GLboolean gl_hasExt( char *name );
GLboolean gl_hasVersion( int major, int minor );
int gl_vendorIsIntel (void);


/*
 * Viewport.
 */
void gl_windowToScreenPos( int *sx, int *sy, int wx, int wy );
void gl_screenToWindowPos( int *wx, int *wy, int sx, int sy );
void gl_viewport( int x, int y, int w, int h );
void gl_defViewport (void);
void gl_setDefViewport( int x, int y, int w, int h );


/*
 * misc
 */
void gl_screenshot( const char *filename );
int SDL_SavePNG( SDL_Surface *surface, const char *file );
#ifdef DEBUGGING
#define gl_checkErr()   gl_checkHandleError( __func__, __LINE__ )
void gl_checkHandleError( const char *func, int line );
#else /* DEBUGGING */
#define gl_checkErr() /**< Hack to ignore errors when debugging. */
#endif /* DEBUGGING */


#endif /* OPENGL_H */

