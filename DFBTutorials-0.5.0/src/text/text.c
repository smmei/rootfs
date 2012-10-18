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
 * text.c
 *
 * Drawing text 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <directfb.h>

/*
 * (Globals)
 */
static IDirectFB *dfb = NULL;
static IDirectFBSurface *primary = NULL;
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
 * The font we will use to draw the text.
 */
static IDirectFBFont *font = NULL;

/* 
 * The string we will draw. Strings in DirectFB have to UTF-8 encoded.
 * For ASCII characters this does not make any difference.
 */
static char *text = "DirectFB rulez!";

int main (int argc, char **argv)
{
  int i, width;

  /*
   * A structure describing font properties.
   */
  DFBFontDescription font_dsc;

  /*
   * (Locals)
   */
  DFBSurfaceDescription dsc;

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
   * First we need to create a font interface by passing a filename
   * and a font description to specify the desired font size. DirectFB will 
   * find (or not) a suitable font loader.
   */
  font_dsc.flags = DFDESC_HEIGHT;
  font_dsc.height = 48;
  DFBCHECK (dfb->CreateFont (dfb, "/decker.ttf", &font_dsc, &font));
  
  /*
   * Set the font to the surface we want to draw to.
   */
  DFBCHECK (primary->SetFont (primary, font));
  
  /*
   * Determine the size of our string when drawn using the loaded font.
   * Since we are interested in the full string, we pass -1 as string length.
   */
  DFBCHECK (font->GetStringWidth (font, text, -1, &width));

  /*
   * We want to let the text slide in on the right and slide out on the left.
   */
  for (i = screen_width; i > -width; i--)
    {
      /*
       * Clear the screen.
       */
      DFBCHECK (primary->SetColor (primary, 0x0, 0x0, 0x0, 0xFF));
      DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

      /*
       * Set the color that will be used to draw the text.
       */
      DFBCHECK (primary->SetColor (primary, 0x80, 0x0, 0x20, 0xFF));

      /*
       * Draw the text left aligned with "i" as the X coordinate.
       */
      DFBCHECK (primary->DrawString (primary, text, -1, i, screen_height / 2, DSTF_LEFT));

      /*
       * Flip the front and back buffer, but wait for the vertical retrace to avoid tearing.
       */
      DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));
    }

  /*
   * Release the font.
   */
  font->Release (font);

  /*
   * (Release)
   */
  primary->Release (primary);
  dfb->Release (dfb);
  
  return 23;
}

