#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Utility {

    Shader& Shader::Use()
    {
        glUseProgram(m_ID);
        return *this;
    }

    void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
    {
        unsigned int sVertex, sFragment, gShader;
        // vertex Shader
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, nullptr);
        glCompileShader(sVertex);
        checkCompileErrors(sVertex, "VERTEX");
        // fragment Shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, nullptr);
        glCompileShader(sFragment);
        checkCompileErrors(sFragment, "FRAGMENT");
        // if geometry shader source code is given, also compile geometry shader
        if (geometrySource != nullptr)
        {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &geometrySource, nullptr);
            glCompileShader(gShader);
            checkCompileErrors(gShader, "GEOMETRY");
        }
        // shader program
        m_ID = glCreateProgram();
        glAttachShader(m_ID, sVertex);
        glAttachShader(m_ID, sFragment);
        if (geometrySource != nullptr)
            glAttachShader(m_ID, gShader);
        glLinkProgram(m_ID);
        checkCompileErrors(m_ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
        if (geometrySource != nullptr)
            glDeleteShader(gShader);
    }

    void Shader::deleteShader()
    {
        glDeleteProgram(m_ID);
    }

    void Shader::setBool(const std::string& name, bool value, bool useShader)
    {
        if (useShader)
            Use();
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value, bool useShader)
    {
        if (useShader)
            Use();
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value, bool useShader)
    {
        if (useShader)
            Use();
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::SetVector2f(const std::string& name, float x, float y, bool useShader)
    {
        if (useShader)
            Use();
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }
    void Shader::SetVector2f(const std::string& name, const glm::vec2& value, bool useShader)
    {
        if (useShader)
            Use();
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
    }
    void Shader::SetVector3f(const std::string& name, float x, float y, float z, bool useShader)
    {
        if (useShader)
            Use();
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }
    void Shader::SetVector3f(const std::string& name, const glm::vec3& value, bool useShader)
    {
        if (useShader)
            Use();
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
    }
    void Shader::SetVector4f(const std::string& name, float x, float y, float z, float w, bool useShader)
    {
        if (useShader)
            Use();
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
    }
    void Shader::SetVector4f(const std::string& name, const glm::vec4& value, bool useShader)
    {
        if (useShader)
            Use();
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
    }
    void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader)
    {
        if (useShader)
            Use();
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, false, glm::value_ptr(matrix));
    }


    void Shader::checkCompileErrors(unsigned int object, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;
            }
        }
        else
        {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;
            }
        }
    }

}
