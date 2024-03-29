/* minwrpngconf.h: headers to make a minimal png-write-only library
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 2007 Glenn Randers-Pehrson
 * Derived from pngcrush.h, Copyright 1998-2007, Glenn Randers-Pehrson
 */

#ifndef MINWRPNGCONF_H
#define MINWRPNGCONF_H

#define PNG_NO_GLOBAL_ARRAYS

#define PNG_NO_READ_SUPPORTED

#define PNG_NO_WARNINGS
#define png_warning(s1,s2) ""
#define png_chunk_warning(s1,s2) ""

#define PNG_NO_WRITE_BACKGROUND
#define PNG_NO_WRITE_GAMMA
#define PNG_NO_WRITE_DITHER
#define PNG_NO_WRITE_INVERT
#define PNG_NO_WRITE_SHIFT
#define PNG_NO_WRITE_PACK
#define PNG_NO_WRITE_PACKSWAP
#define PNG_NO_WRITE_FILLER
#define PNG_NO_WRITE_SWAP_ALPHA
#define PNG_NO_WRITE_INVERT_ALPHA
#define PNG_NO_WRITE_RGB_TO_GRAY
#define PNG_NO_WRITE_USER_TRANSFORM
#define PNG_NO_WRITE_bKGD
#define PNG_NO_WRITE_cHRM
#define PNG_NO_WRITE_gAMA
#define PNG_NO_WRITE_sRGB
#define PNG_NO_WRITE_hIST
#define PNG_NO_WRITE_iCCP
#define PNG_NO_WRITE_oFFs
#define PNG_NO_WRITE_pCAL
#define PNG_NO_WRITE_pHYs
#define PNG_NO_WRITE_sBIT
#define PNG_NO_WRITE_sCAL
#define PNG_NO_WRITE_sPLT
#define PNG_NO_WRITE_TEXT
#define PNG_NO_WRITE_tIME
#define PNG_NO_WRITE_UNKNOWN_CHUNKS
#define PNG_NO_WRITE_USER_CHUNKS
#define PNG_NO_WRITE_EMPTY_PLTE
#define PNG_NO_WRITE_OPT_PLTE
#define PNG_NO_WRITE_FILTER
#define PNG_NO_WRITE_WEIGHTED_FILTER
#define PNG_NO_WRITE_INTERLACING_SUPPORTED  /* effective libpng-1.3.0 */

#define PNG_NO_INFO_IMAGE
#define PNG_NO_USER_MEM
#define PNG_NO_FIXED_POINT_SUPPORTED
#define PNG_NO_MNG_FEATURES
#define PNG_NO_USER_TRANSFORM_PTR
#define PNG_NO_HANDLE_AS_UNKNOWN
#define PNG_NO_CONSOLE_IO
#define PNG_NO_ZALLOC_ZERO
#define PNG_NO_ERROR_NUMBERS
#define PNG_NO_EASY_ACCESS

#endif /* MINWRPNGCONF_H */
