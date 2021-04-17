/*

 MIT License

 Copyright © 2018-2019 Robert B. Colton

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#ifdef _WIN32
#include <cwchar>
#include <cerrno>

#include <png.h>

unsigned libpng_encode32_file(const unsigned char* image, const unsigned w, const unsigned h, const wchar_t* filename) {
  FILE *fp; errno_t err = _wfopen_s(&fp, filename, L"wb");
  if (err) return err;

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -1;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, NULL);
    return -2;
  }

  png_init_io(png, fp);

  // Write header (8 bit colour depth)
  png_set_IHDR(png, info, w, h,
               8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png, info);

  for (unsigned i = 0; i < h; ++i) {
    png_write_row(png, (png_bytep)&image[sizeof(png_byte) * 4 * w * i]);
  }
  png_write_end(png, NULL);

  fclose(fp);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);

  return 0;
}

unsigned libpng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const wchar_t* filename) {
  (*w) = 0; (*h) = 0;
  FILE *fp; errno_t err = _wfopen_s(&fp, filename, L"rb");
  if (err) return err;

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -1;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, NULL);
    return -2;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  png_byte color_type, bit_depth;
  (*w)       = png_get_image_width(png, info);
  (*h)       = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  png_bytep image;
  size_t pitch = sizeof(png_byte) * 4 * (*w); // number of bytes in a row
  image = new png_byte[pitch * (*h)];

  for (size_t y = 0; y < (*h); y++) {
    png_read_row(png, (png_bytep)&image[pitch * y], NULL);
  }

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);

  (*out) = image;

  return 0;
}
#endif
