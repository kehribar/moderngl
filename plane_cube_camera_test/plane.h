// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#ifndef PLANE_H
#define PLANE_H

// ----------------------------------------------------------------------------
#include <stdint.h>
#include "shader.h"
#include "stb_image.h"
#include "opengl_util.h"

// ----------------------------------------------------------------------------
class Plane_NoTexture{
  
  // ...
  public:

    // ...
    float* planeVertices;
    uint32_t vertex_count;

    // ...
    Shader shader;
    unsigned int VAO;
    unsigned int VBO;

    // ...
    int32_t init(
      const char* vertexPath, const char* fragmentPath, uint32_t gridCount
    )
    {
      if(gridCount < 2)
      {
        return -1;
      }

      // ...
      vertex_count = (gridCount + 1) * 2 * 2;
      planeVertices = new float[vertex_count * 3];

      // Horizontal (?) lines
      for(int i=0; i<(gridCount + 1); i++)
      {
        float gridPoint = -1.0f;
        gridPoint += (float)(2 * i) / (float)((gridCount + 0));

        // Start point
        planeVertices[(i * 6) + 0] = -1.0f;
        planeVertices[(i * 6) + 1] = gridPoint;
        planeVertices[(i * 6) + 2] = 0.0f;

        // End point
        planeVertices[(i * 6) + 3] = 1.0f;
        planeVertices[(i * 6) + 4] = gridPoint;
        planeVertices[(i * 6) + 5] = 0.0f;
      }

      // Vertical (?) lines
      int offset = (vertex_count * 3) / 2;
      for(int i=0; i<(gridCount + 1); i++)
      {
        float gridPoint = -1.0f;
        gridPoint += (float)(2 * i) / (float)((gridCount + 0));

        // Start point
        planeVertices[offset + (i * 6) + 0] = gridPoint;
        planeVertices[offset + (i * 6) + 1] = -1.0f;
        planeVertices[offset + (i * 6) + 2] = 0.0f;

        // End point
        planeVertices[offset + (i * 6) + 3] = gridPoint;
        planeVertices[offset + (i * 6) + 4] = 1.0f;
        planeVertices[offset + (i * 6) + 5] = 0.0f;
      }

      // ...
      shader.init(vertexPath, fragmentPath);

      // ...
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      // ...
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(
        GL_ARRAY_BUFFER, vertex_count * 3 * sizeof(float), 
        planeVertices, GL_STATIC_DRAW
      );

      // ...
      glVertexAttribPointer(0, 
        3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0
      );
      glEnableVertexAttribArray(0);

      // ...
      return 0;
    }

    // ...
    void bindVertexArray()
    {
      // ...
      glBindVertexArray(VAO);
    }

    // ...
    void render()
    {
      // ...
      glDrawArrays(GL_LINES, 0, vertex_count);
    }

  // ...
  private:

 };

// ----------------------------------------------------------------------------
#endif