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
 * sprite.c
 *
 * Moving a sprite with the cursor keys (without input buffer)
 */

#include <stdio.h>
#include <unistd.h>

#include <directfb.h>

/*
 * (Globals)
 */
static IDirectFB *dfb = NULL;
static IDirectFBSurface *primary = NULL;
static IDirectFBSurface *tux = NULL;
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
 * Our interface to the keyboard.
 */
static IDirectFBInputDevice *keyboard = NULL;


int main (int argc, char **argv)
{
  /*
   * (Locals)
   */
  DFBSurfaceDescription   dsc;
  IDirectFBImageProvider *provider;

  /*
   * To quit the application when escape is pressed we store the key state here.
   */
  DFBInputDeviceKeyState escape = DIKS_UP;

  /*
   * Here we store the current position of the sprite on the screen
   * and the maximum x/y coordinates the sprite can have without being clipped.
   */
  int sprite_x, sprite_y, max_x, max_y;

  /*
   * (Initialize)
   */
  DFBCHECK (DirectFBInit (&argc, &argv));
  DFBCHECK (DirectFBCreate (&dfb));
  DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
  dsc.flags = DSDESC_CAPS;
  dsc.caps  = DSCAPS_PRIMARY | DSCAPS_FLIPPING;
  DFBCHECK (dfb->CreateSurface( dfb, &dsc, &primary ));
  DFBCHECK (primary->GetSize (primary, &screen_width, &screen_height));

  /*
   * Retrieve an interface to the keyboard so we can query key states.
   * DIDID_KEYBOARD is the device id of the primary keyboard.
   */
  DFBCHECK (dfb->GetInputDevice (dfb, DIDID_KEYBOARD, &keyboard));

  /*
   * (Load the sprite)
   */
  DFBCHECK (dfb->CreateImageProvider (dfb, "/tux.png", &provider));
  DFBCHECK (provider->GetSurfaceDescription (provider, &dsc));
  DFBCHECK (dfb->CreateSurface (dfb, &dsc, &tux));
  DFBCHECK (provider->RenderTo (provider, tux, NULL));
  provider->Release (provider);

  /*
   * Calculate maximum x/y coordinates depending on screen resolution and sprite size.
   */
  max_x = screen_width - dsc.width;
  max_y = screen_height - dsc.height;

  /*
   * Initialize the sprite position by centering it on the screen.
   */
  sprite_x = (screen_width - dsc.width) / 2;
  sprite_y = (screen_height - dsc.height) / 2;

  /*
   * Loop through until the escape key is pressed.
   */
  while (escape == DIKS_UP)
    {
      /*
       * Local variable for key state queries.
       */
      DFBInputDeviceKeyState state;

      /*
       * Clear the screen.
       */
      DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

      /*
       * Blit the sprite at its current position.
       */
      DFBCHECK (primary->Blit (primary, tux, NULL, sprite_x, sprite_y));

      /*
       * Flip the front and back buffer, but wait for the vertical retrace to avoid tearing.
       */
      DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));

      /*
       * Query state of cursor keys and eventually move the sprite.
       */
      DFBCHECK (keyboard->GetKeyState (keyboard, DIKI_LEFT, &state));
      if (state == DIKS_DOWN)
	sprite_x--;

      DFBCHECK (keyboard->GetKeyState (keyboard, DIKI_RIGHT, &state));
      if (state == DIKS_DOWN)
	sprite_x++;

      DFBCHECK (keyboard->GetKeyState (keyboard, DIKI_UP, &state));
      if (state == DIKS_DOWN)
	sprite_y--;

      DFBCHECK (keyboard->GetKeyState (keyboard, DIKI_DOWN, &state));
      if (state == DIKS_DOWN)
	sprite_y++;

      /*
       * Now make sure we didn't move the sprite out of the screen.
       */
      if (sprite_x < 0)
	sprite_x = 0;
      else if (sprite_x > max_x)
	sprite_x = max_x;
      if (sprite_y < 0)
	sprite_y = 0;
      else if (sprite_y > max_y)
	sprite_y = max_y;

      /*
       * Query state of escape key.
       */
      DFBCHECK (keyboard->GetKeyState (keyboard, DIKI_ESCAPE, &escape));
    }

  /*
   * Release the keyboard.
   */
  keyboard->Release (keyboard);

  /*
   * (Release)
   */
  tux->Release (tux);
  primary->Release (primary);
  dfb->Release (dfb);
  
  return 23;
}

