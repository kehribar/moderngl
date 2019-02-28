// ----------------------------------------------------------------------------
// 
// 
// ----------------------------------------------------------------------------
#ifndef SHADER_H
#define SHADER_H

// ----------------------------------------------------------------------------
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// ----------------------------------------------------------------------------
class Shader{
  
  public:

    // ...
    bool isValid;
    unsigned int ID;

    // ...
    unsigned int vertex;
    unsigned int fragment;

    // ...
    void init(const char* vertexPath, const char* fragmentPath)
    {
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;
     
      // Enable expections
      vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      // ...
      isValid = true;

      try 
      {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
       
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();   
       
        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();
       
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();     

        // ...
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        if(checkCompileErrors(vertex, "VERTEX") == false)
        {
          isValid = false;
        }

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if(checkCompileErrors(fragment, "FRAGMENT") == false)
        {
          isValid = false;
        }

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
      }
      catch(std::ifstream::failure e)
      {
        isValid = false;
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
      }
    }

    // ..
    void bindVertexInput(int32_t index, const char* name)
    {
      // ...
      glBindAttribLocation(ID, index, name);
    }

    // ...
    void link()
    {
      // ...
      glLinkProgram(ID);
      if(checkCompileErrors(ID, "PROGRAM") == false)
      {
        isValid = false;
      }

      // Delete the shaders as they're linked into our program now and no 
      // longer necessary
      glDeleteShader(vertex);
      glDeleteShader(fragment);
    }

    // ...
    void use() 
    { 
      glUseProgram(ID); 
    }

    // ...
    void setBool(const std::string &name, bool value) const
    {         
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }

    // ...
    void setInt(const std::string &name, int value) const
    { 
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }

    // ...
    void setFloat(const std::string &name, float value) const
    { 
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    // ...
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
      glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    
    // ...
    void setVec2(const std::string &name, float x, float y) const
    { 
      glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }

    // ...
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }

    // ...
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
      glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }

    // ...
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
      glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }

    // ...
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
      glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }

    // ...
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
      glUniformMatrix2fv(
        glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]
      );
    }

    // ...
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
      glUniformMatrix3fv(
        glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]
      );
    }

    // ...
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
      glUniformMatrix4fv(
        glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]
      );
    }

  private:

    // ...
    bool checkCompileErrors(GLuint shader, std::string type)
    {
      GLint success;
      GLchar infoLog[1024];
      if(type != "PROGRAM")
      {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
          glGetShaderInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: ";
          std::cout << type << std::endl << infoLog;
          std::cout << std::endl;
          return false;
        }
        return true;
      }
      else
      {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
          glGetProgramInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: ";
          std::cout << type << std::endl << infoLog;
          std::cout << std::endl;
          return false;
        }
        return true;
      }
    }
};

// ----------------------------------------------------------------------------
#endif
