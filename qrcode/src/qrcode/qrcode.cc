#include "qrcode/qrcode.h"

#define STB_IMAGE_IMPLEMENTATION

#include "qrcode/stb_image.h"

namespace qrcode {

std::vector<std::vector<ImageInfo>> QRCode::ReadImage(const char* path) {
  int width, height, bpp;

  uint8_t* rgb_image = stbi_load(path, &width, &height, &bpp, 3);

  std::vector<std::vector<ImageInfo>> res(
      height, std::vector<ImageInfo>(width, {0, 0, 0}));

  uint8_t* temp = rgb_image;

  for (int index = 0; index < width * height; index++) {
    // r
    auto r = *(temp++);
    // g
    auto g = *(temp++);
    // b
    auto b = *(temp++);
    res[index / width][index % width] = {r, g, b};
  }

  stbi_image_free(rgb_image);
  return res;
}
}  // namespace qrcode
