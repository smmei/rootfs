/*
   DirectFB Tutorials

   (c) Copyright 2000-2002  convergence integrated media GmbH.
   (c) Copyright 2002       convergence GmbH.
   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de> and
              Sven Neumann <neo@directfb.org>.
              
   This file is subject to the terms and conditions of the MIT License:

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * keybuffer.c
 *
 * Keybuffer and simple alphachannel example
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

/*
 * (Globals)
 */
static IDirectFB *dfb = NULL;
static IDirectFBSurface *primary = NULL;
static IDirectFBSurface *foot = NULL;
static IDirectFBInputDevice *keyboard = NULL;
static int screen_width  = 0;
static int screen_height = 0;
#define DFBCHECK(x...)                                         \
  {                                                            \
    DFBResult err = x;                                         \
                                                               \
    if (err != DFB_OK)                                         \
      {                                                        \
        fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
        DirectFBErrorFatal( #x, err );                         \
      }                                                        \
  }

/*
 * A buffer for input events.
 */
static IDirectFBEventBuffer *buffer = NULL;


int main (int argc, char **argv)
{
  /*
   * (Locals)
   */
  DFBSurfaceDescription   dsc;
  IDirectFBImageProvider *provider;

  /*
   * Set this to non-null to quit.
   */
  int quit = 0;

  /*
   * (Initialize)
   */
  DFBCHECK (DirectFBInit (&argc, &argv));
  DFBCHECK (DirectFBCreate (&dfb));
  DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
  dsc.flags = DSDESC_CAPS;
  dsc.caps  = DSCAPS_PRIMARY;
  DFBCHECK (dfb->CreateSurface( dfb, &dsc, &primary ));
  DFBCHECK (primary->GetSize (primary, &screen_width, &screen_height));

  /*
   * (Clear)
   */
  DFBCHECK (primary->FillRectangle (primary, 
                                    0, 0, screen_width, screen_height));

  /*
   * (Get keyboard)
   */
  DFBCHECK (dfb->GetInputDevice (dfb, DIDID_KEYBOARD, &keyboard));

  /*
   * Create an event buffer for the keyboard.
   */
  DFBCHECK (keyboard->CreateEventBuffer (keyboard, &buffer));

  /*
   * (Load the foot)
   */
  DFBCHECK (dfb->CreateImageProvider (dfb, "/foot.png", &provider));
  DFBCHECK (provider->GetSurfaceDescription (provider, &dsc));
  DFBCHECK (dfb->CreateSurface (dfb, &dsc, &foot));
  DFBCHECK (provider->RenderTo (provider, foot, NULL));
  provider->Release (provider);

  /*
   * Set blitting flags to DSBLIT_BLEND_ALPHACHANNEL that enables alpha
   * blending using the alpha channel of the source.
   */
  DFBCHECK (primary->SetBlittingFlags (primary, DSBLIT_BLEND_ALPHACHANNEL));

  /*
   * Loop as long as the escape key has not been pressed.
   */
  while (!quit)
    {
      /*
       * Structure which stores a DirectFB input event from an input buffer.
       */
      DFBInputEvent event;

      /*
       * This makes the current thread wait idle for the next event.
       */
      DFBCHECK (buffer->WaitForEvent (buffer));

      /*
       * Fetch all events from buffer one by one and process them.
       */
      while (buffer->GetEvent (buffer, DFB_EVENT(&event)) == DFB_OK)
        {
          /*
           * If any key went up, we clear the screen.
           */
          if (event.type == DIET_KEYRELEASE)
            DFBCHECK (primary->FillRectangle (primary,
                                              0, 0, 
                                              screen_width, screen_height));

          /*
           * If a key has been pressed and it's the escape key, we quit.
           * Otherwise we put a foot print somewhere.
           */
          if (event.type == DIET_KEYPRESS)
            {
              if (event.key_id == DIKI_ESCAPE)
                quit = 1;
              else
                DFBCHECK (primary->Blit (primary, foot, NULL,
                                         rand() % (screen_width - dsc.width),
                                         rand() % (screen_height - dsc.height)));
            }
        }

      /*
       * We do no flipping here because we created a non flipping primary
       * surface.
       */
    }

  /*
   * Release the input buffer.
   */
  buffer->Release (buffer);

  /*
   * (Release)
   */
  keyboard->Release (keyboard);
  foot->Release (foot);
  primary->Release (primary);
  dfb->Release (dfb);

  return 23;
}

