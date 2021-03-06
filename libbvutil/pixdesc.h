/*
 * pixel format descriptor
 * Copyright (c) 2009 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of BVBase.
 *
 * BVBase is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * BVBase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BVBase; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef BVUTIL_PIXDESC_H
#define BVUTIL_PIXDESC_H

#include <inttypes.h>

#include "attributes.h"
#include "pixfmt.h"

typedef struct BVComponentDescriptor {
    /**
     * Which of the 4 planes contains the component.
     */
    uint16_t plane        : 2;

    /**
     * Number of elements between 2 horizontally consecutive pixels minus 1.
     * Elements are bits for bitstream formats, bytes otherwise.
     */
    uint16_t step_minus1  : 3;

    /**
     * Number of elements before the component of the first pixel plus 1.
     * Elements are bits for bitstream formats, bytes otherwise.
     */
    uint16_t offset_plus1 : 3;

    /**
     * Number of least significant bits that must be shifted away
     * to get the value.
     */
    uint16_t shift        : 3;

    /**
     * Number of bits in the component minus 1.
     */
    uint16_t depth_minus1 : 4;
} BVComponentDescriptor;

/**
 * Descriptor that unambiguously describes how the bits of a pixel are
 * stored in the up to 4 data planes of an image. It also stores the
 * subsampling factors and number of components.
 *
 * @note This is separate of the colorspace (RGB, YCbCr, YPbPr, JPEG-style YUV
 *       and all the YUV variants) BVPixFmtDescriptor just stores how values
 *       are stored not what these values represent.
 */
typedef struct BVPixFmtDescriptor {
    const char *name;
    uint8_t nb_components;  ///< The number of components each pixel has, (1-4)

    /**
     * Amount to shift the luma width right to find the chroma width.
     * For YV12 this is 1 for example.
     * chroma_width = -((-luma_width) >> log2_chroma_w)
     * The note above is needed to ensure rounding up.
     * This value only refers to the chroma components.
     */
    uint8_t log2_chroma_w;  ///< chroma_width = -((-luma_width )>>log2_chroma_w)

    /**
     * Amount to shift the luma height right to find the chroma height.
     * For YV12 this is 1 for example.
     * chroma_height= -((-luma_height) >> log2_chroma_h)
     * The note above is needed to ensure rounding up.
     * This value only refers to the chroma components.
     */
    uint8_t log2_chroma_h;
    uint8_t flags;

    /**
     * Parameters that describe how pixels are packed.
     * If the format has 2 or 4 components, then alpha is last.
     * If the format has 1 or 2 components, then luma is 0.
     * If the format has 3 or 4 components,
     * if the RGB flag is set then 0 is red, 1 is green and 2 is blue;
     * otherwise 0 is luma, 1 is chroma-U and 2 is chroma-V.
     */
    BVComponentDescriptor comp[4];

    /**
     * Alternative comma-separated names.
     */
    const char *alias;
} BVPixFmtDescriptor;

/**
 * Pixel format is big-endian.
 */
#define BV_PIX_FMT_FLAG_BE           (1 << 0)
/**
 * Pixel format has a palette in data[1], values are indexes in this palette.
 */
#define BV_PIX_FMT_FLAG_PAL          (1 << 1)
/**
 * All values of a component are bit-wise packed end to end.
 */
#define BV_PIX_FMT_FLAG_BITSTREAM    (1 << 2)
/**
 * Pixel format is an HW accelerated format.
 */
#define BV_PIX_FMT_FLAG_HWACCEL      (1 << 3)
/**
 * At least one pixel component is not in the first data plane.
 */
#define BV_PIX_FMT_FLAG_PLANAR       (1 << 4)
/**
 * The pixel format contains RGB-like data (as opposed to YUV/grayscale).
 */
#define BV_PIX_FMT_FLAG_RGB          (1 << 5)
/**
 * The pixel format is "pseudo-paletted". This means that BVBase treats it as
 * paletted internally, but the palette is generated by the decoder and is not
 * stored in the file.
 */
#define BV_PIX_FMT_FLAG_PSEUDOPAL    (1 << 6)
/**
 * The pixel format has an alpha channel.
 */
#define BV_PIX_FMT_FLAG_ALPHA        (1 << 7)

#if BV_API_PIX_FMT
/**
 * @deprecated use the BV_PIX_FMT_FLAG_* flags
 */
#define PIX_FMT_BE        BV_PIX_FMT_FLAG_BE
#define PIX_FMT_PAL       BV_PIX_FMT_FLAG_PAL
#define PIX_FMT_BITSTREAM BV_PIX_FMT_FLAG_BITSTREAM
#define PIX_FMT_HWACCEL   BV_PIX_FMT_FLAG_HWACCEL
#define PIX_FMT_PLANAR    BV_PIX_FMT_FLAG_PLANAR
#define PIX_FMT_RGB       BV_PIX_FMT_FLAG_RGB
#define PIX_FMT_PSEUDOPAL BV_PIX_FMT_FLAG_PSEUDOPAL
#define PIX_FMT_ALPHA     BV_PIX_FMT_FLAG_ALPHA
#endif

#if BV_API_PIX_FMT_DESC
/**
 * The array of all the pixel format descriptors.
 */
extern attribute_deprecated const BVPixFmtDescriptor bv_pix_fmt_descriptors[];
#endif

/**
 * Read a line from an image, and write the values of the
 * pixel format component c to dst.
 *
 * @param data the array containing the pointers to the planes of the image
 * @param linesize the array containing the linesizes of the image
 * @param desc the pixel format descriptor for the image
 * @param x the horizontal coordinate of the first pixel to read
 * @param y the vertical coordinate of the first pixel to read
 * @param w the width of the line to read, that is the number of
 * values to write to dst
 * @param read_pal_component if not zero and the format is a paletted
 * format writes the values corresponding to the palette
 * component c in data[1] to dst, rather than the palette indexes in
 * data[0]. The behavior is undefined if the format is not paletted.
 */
void bv_read_image_line(uint16_t *dst, const uint8_t *data[4],
                        const int linesize[4], const BVPixFmtDescriptor *desc,
                        int x, int y, int c, int w, int read_pal_component);

/**
 * Write the values from src to the pixel format component c of an
 * image line.
 *
 * @param src array containing the values to write
 * @param data the array containing the pointers to the planes of the
 * image to write into. It is supposed to be zeroed.
 * @param linesize the array containing the linesizes of the image
 * @param desc the pixel format descriptor for the image
 * @param x the horizontal coordinate of the first pixel to write
 * @param y the vertical coordinate of the first pixel to write
 * @param w the width of the line to write, that is the number of
 * values to write to the image line
 */
void bv_write_image_line(const uint16_t *src, uint8_t *data[4],
                         const int linesize[4], const BVPixFmtDescriptor *desc,
                         int x, int y, int c, int w);

/**
 * Return the pixel format corresponding to name.
 *
 * If there is no pixel format with name name, then looks for a
 * pixel format with the name corresponding to the native endian
 * format of name.
 * For example in a little-endian system, first looks for "gray16",
 * then for "gray16le".
 *
 * Finally if no pixel format has been found, returns BV_PIX_FMT_NONE.
 */
enum BVPixelFormat bv_get_pix_fmt(const char *name);

/**
 * Return the short name for a pixel format, NULL in case pix_fmt is
 * unknown.
 *
 * @see bv_get_pix_fmt(), bv_get_pix_fmt_string()
 */
const char *bv_get_pix_fmt_name(enum BVPixelFormat pix_fmt);

/**
 * Print in buf the string corresponding to the pixel format with
 * number pix_fmt, or a header if pix_fmt is negative.
 *
 * @param buf the buffer where to write the string
 * @param buf_size the size of buf
 * @param pix_fmt the number of the pixel format to print the
 * corresponding info string, or a negative value to print the
 * corresponding header.
 */
char *bv_get_pix_fmt_string(char *buf, int buf_size,
                            enum BVPixelFormat pix_fmt);

/**
 * Return the number of bits per pixel used by the pixel format
 * described by pixdesc. Note that this is not the same as the number
 * of bits per sample.
 *
 * The returned number of bits refers to the number of bits actually
 * used for storing the pixel information, that is padding bits are
 * not counted.
 */
int bv_get_bits_per_pixel(const BVPixFmtDescriptor *pixdesc);

/**
 * Return the number of bits per pixel for the pixel format
 * described by pixdesc, including any padding or unused bits.
 */
int bv_get_padded_bits_per_pixel(const BVPixFmtDescriptor *pixdesc);

/**
 * @return a pixel format descriptor for provided pixel format or NULL if
 * this pixel format is unknown.
 */
const BVPixFmtDescriptor *bv_pix_fmt_desc_get(enum BVPixelFormat pix_fmt);

/**
 * Iterate over all pixel format descriptors known to libbvutil.
 *
 * @param prev previous descriptor. NULL to get the first descriptor.
 *
 * @return next descriptor or NULL after the last descriptor
 */
const BVPixFmtDescriptor *bv_pix_fmt_desc_next(const BVPixFmtDescriptor *prev);

/**
 * @return an BVPixelFormat id described by desc, or BV_PIX_FMT_NONE if desc
 * is not a valid pointer to a pixel format descriptor.
 */
enum BVPixelFormat bv_pix_fmt_desc_get_id(const BVPixFmtDescriptor *desc);

/**
 * Utility function to access log2_chroma_w log2_chroma_h from
 * the pixel format BVPixFmtDescriptor.
 *
 * See bv_get_chroma_sub_sample() for a function that asserts a
 * valid pixel format instead of returning an error code.
 * Its recommended that you use avcodec_get_chroma_sub_sample unless
 * you do check the return code!
 *
 * @param[in]  pix_fmt the pixel format
 * @param[out] h_shift store log2_chroma_w
 * @param[out] v_shift store log2_chroma_h
 *
 * @return 0 on success, BVERROR(ENOSYS) on invalid or unknown pixel format
 */
int bv_pix_fmt_get_chroma_sub_sample(enum BVPixelFormat pix_fmt,
                                     int *h_shift, int *v_shift);

/**
 * @return number of planes in pix_fmt, a negative BVERROR if pix_fmt is not a
 * valid pixel format.
 */
int bv_pix_fmt_count_planes(enum BVPixelFormat pix_fmt);

void bb_check_pixfmt_descriptors(void);

/**
 * Utility function to swap the endianness of a pixel format.
 *
 * @param[in]  pix_fmt the pixel format
 *
 * @return pixel format with swapped endianness if it exists,
 * otherwise BV_PIX_FMT_NONE
 */
enum BVPixelFormat bv_pix_fmt_swap_endianness(enum BVPixelFormat pix_fmt);

#define BV_LOSS_RESOLUTION  0x0001 /**< loss due to resolution change */
#define BV_LOSS_DEPTH       0x0002 /**< loss due to color depth change */
#define BV_LOSS_COLORSPACE  0x0004 /**< loss due to color space conversion */
#define BV_LOSS_ALPHA       0x0008 /**< loss of alpha bits */
#define BV_LOSS_COLORQUANT  0x0010 /**< loss due to color quantization */
#define BV_LOSS_CHROMA      0x0020 /**< loss of chroma (e.g. RGB to gray conversion) */

/**
 * Compute what kind of losses will occur when converting from one specific
 * pixel format to another.
 * When converting from one pixel format to another, information loss may occur.
 * For example, when converting from RGB24 to GRAY, the color information will
 * be lost. Similarly, other losses occur when converting from some formats to
 * other formats. These losses can involve loss of chroma, but also loss of
 * resolution, loss of color depth, loss due to the color space conversion, loss
 * of the alpha bits or loss due to color quantization.
 * bv_get_fix_fmt_loss() informs you about the various types of losses
 * which will occur when converting from one pixel format to another.
 *
 * @param[in] dst_pix_fmt destination pixel format
 * @param[in] src_pix_fmt source pixel format
 * @param[in] has_alpha Whether the source pixel format alpha channel is used.
 * @return Combination of flags informing you what kind of losses will occur
 * (maximum loss for an invalid dst_pix_fmt).
 */
int bv_get_pix_fmt_loss(enum BVPixelFormat dst_pix_fmt,
                        enum BVPixelFormat src_pix_fmt,
                        int has_alpha);

/**
 * Compute what kind of losses will occur when converting from one specific
 * pixel format to another.
 * When converting from one pixel format to another, information loss may occur.
 * For example, when converting from RGB24 to GRAY, the color information will
 * be lost. Similarly, other losses occur when converting from some formats to
 * other formats. These losses can involve loss of chroma, but also loss of
 * resolution, loss of color depth, loss due to the color space conversion, loss
 * of the alpha bits or loss due to color quantization.
 * bv_get_fix_fmt_loss() informs you about the various types of losses
 * which will occur when converting from one pixel format to another.
 *
 * @param[in] dst_pix_fmt destination pixel format
 * @param[in] src_pix_fmt source pixel format
 * @param[in] has_alpha Whether the source pixel format alpha channel is used.
 * @return Combination of flags informing you what kind of losses will occur
 * (maximum loss for an invalid dst_pix_fmt).
 */
enum BVPixelFormat bv_find_best_pix_fmt_of_2(enum BVPixelFormat dst_pix_fmt1, enum BVPixelFormat dst_pix_fmt2,
                                             enum BVPixelFormat src_pix_fmt, int has_alpha, int *loss_ptr);

/**
 * @return the name for provided color range or NULL if unknown.
 */
const char *bv_color_range_name(enum BVColorRange range);

/**
 * @return the name for provided color primaries or NULL if unknown.
 */
const char *bv_color_primaries_name(enum BVColorPrimaries primaries);

/**
 * @return the name for provided color transfer or NULL if unknown.
 */
const char *bv_color_transfer_name(enum BVColorTransferCharacteristic transfer);

/**
 * @return the name for provided color space or NULL if unknown.
 */
const char *bv_color_space_name(enum BVColorSpace space);

/**
 * @return the name for provided chroma location or NULL if unknown.
 */
const char *bv_chroma_location_name(enum BVChromaLocation location);

#endif /* BVUTIL_PIXDESC_H */
