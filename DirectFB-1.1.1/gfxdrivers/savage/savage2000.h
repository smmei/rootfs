/*
   (c) Copyright 2001-2007  The DirectFB Organization (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __SAVAGE2000_H__
#define __SAVAGE2000_H__

#include <core/gfxcard.h>

#include "savage.h"
#include "mmio.h"

typedef struct {
     SavageDeviceData s;
} Savage2000DeviceData;

typedef struct {
     SavageDriverData s;
} Savage2000DriverData;

void
savage2000_get_info( CoreGraphicsDevice *device,
                     GraphicsDriverInfo *info );

DFBResult
savage2000_init_driver( CoreGraphicsDevice  *device,
                        GraphicsDeviceFuncs *funcs,
                        void                *driver_data );

DFBResult
savage2000_init_device( CoreGraphicsDevice *device,
                        GraphicsDeviceInfo *device_info,
                        void               *driver_data,
                        void               *device_data );

void
savage2000_close_device( CoreGraphicsDevice *device,
                         void               *driver_data,
                         void               *device_data );

void
savage2000_close_driver( CoreGraphicsDevice *device,
                         void               *driver_data );


#undef FIFOSTATUS
#define FIFOSTATUS      0x48C60

/* Wait for fifo space */
static inline void
savage2000_waitfifo(Savage2000DriverData *sdrv,
                    Savage2000DeviceData *sdev, int space)
{
     uint32         slots = MAXFIFO - space;
     volatile u8   *mmio  = sdrv->s.mmio_base;

     sdev->s.waitfifo_sum += space;
     sdev->s.waitfifo_calls++;
     
     if ((savage_in32(mmio, FIFOSTATUS) & 0x000fffff) > slots) {
          do {
               sdev->s.fifo_waitcycles++;
          } while (savage_in32(mmio, FIFOSTATUS) > slots);
     }
     else {
          sdev->s.fifo_cache_hits++;
     }
}

/* Wait for idle accelerator */
static inline void
savage2000_waitidle(Savage2000DriverData *sdrv, Savage2000DeviceData *sdev)
{
     sdev->s.waitidle_calls++;

     while (savage_in32(sdrv->s.mmio_base, FIFOSTATUS) & 0x009fffff) {
          sdev->s.idle_waitcycles++;
     }
}

#endif
