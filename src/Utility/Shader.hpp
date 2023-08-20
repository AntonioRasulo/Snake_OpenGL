#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Utility {

    class Shader
    {
        /* Program ID */
        unsigned int m_ID;

        /* checks if compilation or linking failed and if so, print the error logs */
        void checkCompileErrors(unsigned int object, std::string type);

    public:

        /* sets the current shader as active */
        Shader& Use();

        /* Delete Shader */
        void deleteShader();

        /* compiles the shader from given source code */
        void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

        /* utility uniform functions */
        void setBool(const std::string& name, bool value, bool useShader = false);
        void setInt(const std::string& name, int value, bool useShader = false);
        void setFloat(const std::string& name, float value, bool useShader = false);
        void SetVector2f(const std::string& name, float x, float y, bool useShader = false);
        void SetVector2f(const std::string& name, const glm::vec2& value, bool useShader = false);
        void SetVector3f(const std::string& name, float x, float y, float z, bool useShader = false);
        void SetVector3f(const std::string& name, const glm::vec3& value, bool useShader = false);
        void SetVector4f(const std::string& name, float x, float y, float z, float w, bool useShader = false);
        void SetVector4f(const std::string& name, const glm::vec4& value, bool useShader = false);
        void SetMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader = false);

        /* setter and getter ID */
        void setID(unsigned int id) { m_ID = id; }
        unsigned int getID() const { return m_ID; }

    };

}
