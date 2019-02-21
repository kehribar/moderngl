// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#ifndef CUBE_H
#define CUBE_H

// ----------------------------------------------------------------------------
#include <stdint.h>
#include "shader.h"
#include "stb_image.h"
#include "opengl_util.h"

// ----------------------------------------------------------------------------
class Cube_wTexture{
  
  public:

    // ...
    static const int32_t cube_vertices_count = 36;
        
    // ...
    const float cube_vertices[36 * 5] = {

      // positions          // texture location
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // ...
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;

    // ...
    unsigned int tex;
    unsigned int tex_ind;

    // ...
    void init(
      const char* vertexPath, const char* fragmentPath, const char* texturePath
    )
    {
      // ...
      shader.init(vertexPath, fragmentPath);

      // ...
      tex_ind = opengl_getTextureID();

      // ...
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      // ...
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(
        GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW
      );

      // ...
      glVertexAttribPointer(0, 
        3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0
      );
      glEnableVertexAttribArray(0);

      // ...
      glVertexAttribPointer(1, 
        2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))
      );
      glEnableVertexAttribArray(1);

      // ...
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      
      // Set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      
      // Set texture filtering parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      // Load image, create texture and generate mipmaps
      int width;
      int height;
      int nrChannels;
      stbi_set_flip_vertically_on_load(true);
      unsigned char *data = stbi_load(
        texturePath, &width, &height, &nrChannels, 0
      );
      if(data)
      {
        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      else
      {
        std::cout << "Failed to load texture" << std::endl;
      }
      stbi_image_free(data);
    }

    // ...
    void bindVertexArray()
    {
      // ...
      glActiveTexture(GL_TEXTURE0 + tex_ind);

      // ...
      shader.setInt("tex", tex_ind);

      // ...
      glBindTexture(GL_TEXTURE_2D, tex);

      // ...
      glBindVertexArray(VAO);
    }

    // ...
    void render()
    {
      // ...
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  };

// ----------------------------------------------------------------------------
#endif