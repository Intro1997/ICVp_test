#include "icv_texture.hpp"
#include "tools.hpp"
#include <iostream>
#include <stb/stb_image.h>

GLuint CreateTextureObj(const char *path) {
  int width, height, channel;

  unsigned char *data = stbi_load(path, &width, &height, &channel, 0);
  GLenum image_type[] = {0, GL_RED, 0, GL_RGB, GL_RGBA};
  if (data) {
    if (image_type[channel] == 0 ||
        channel >= sizeof(image_type) / sizeof(GLenum)) {
      std::cerr << "Unsupport channel number" << channel << "\n";
      return 0;
    }

    GLint origin_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &origin_texture);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, image_type[channel], width, height, 0,
                 image_type[channel], GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, origin_texture);

    CHECK_GL_ERROR_WITH_RET(0);

    return textureId;
  } else {
    std::cerr << "Load image [" << path << "] failed\n";
    return 0;
  }
}