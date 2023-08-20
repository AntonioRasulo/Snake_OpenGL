#pragma once

#include <map>
#include <string>

namespace Utility {
    /* A static singleton ResourceManager class that hosts several
       functions to load Textures and Shaders. Each loaded texture
       and/or shader is also stored for future reference by string
       handles. All functions and resources are static and no 
       public constructor is defined. */

    class Shader;
    class Texture2D;

    class ResourceManager
    {

        /* Default destructor */
        ~ResourceManager() {}

        /* private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static). */
        ResourceManager() { }

        /* loads and generates a shader from file */
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        /* loads a single texture from file */
        static Texture2D loadTextureFromFile(const char* file, bool alpha);

        /* resource storage */
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;

    public:
   
        /**
         * Deleting copy constructor.
         */
        ResourceManager(const ResourceManager& obj) = delete;

        /**
         * Deleting copy assignment operator.
         */
        ResourceManager& operator=(const ResourceManager& obj) = delete;

        /**
         * Deleting move constructor.
         */
        ResourceManager(ResourceManager&& obj) = delete;

        /**
         * Deleting move assignment operator.
         */
        ResourceManager& operator=(ResourceManager&& obj) = delete;

        /* loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code.
           If gShaderFile is not nullptr, it also loads a geometry shader */
        static Shader& LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name);
        /* retrieves a stored sader */
        static Shader& GetShader(const std::string& name);
        /* loads (and generates) a texture from file */
        static Texture2D& LoadTexture(const char* file, bool alpha, const std::string& name);
        /* retrieves a stored texture */
        static Texture2D& GetTexture(const std::string& name);

        /* properly de-allocates all loaded resources */
        static void Clear();

    };

}
