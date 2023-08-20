#pragma once

namespace Utility {
    /* Texture2D is able to store and configure a texture in OpenGL. */
    /* It also hosts utility functions for easy management. */
    class Texture2D
    {

        /* holds the ID of the texture object, used for all texture operations to reference to this particular texture */
        unsigned int m_ID;

        /* texture image dimensions */
        /* width and height of loaded image in pixels */
        unsigned int m_width;
        unsigned int m_height;

        /* texture Format */
        unsigned int m_internalFormat;  /* format of texture object */
        unsigned int m_imageFormat;     /* format of loaded image */

        /* texture configuration */
        unsigned int m_wrapS;       /* wrapping mode on S axis */
        unsigned int m_wrapT;       /* wrapping mode on T axis */
        unsigned int m_filterMin;   /* filtering mode if texture pixels < screen pixels */
        unsigned int m_filterMax;   /* filtering mode if texture pixels > screen pixels */

    public:

        /* constructor (sets default texture modes) */
        Texture2D();

        /* generates texture from image data */
        void Generate(unsigned int width, unsigned int height, unsigned char* data);

        /* binds the texture as the current active GL_TEXTURE_2D texture object */
        void Bind() const;

        /* Getters */
        unsigned int getID() const { return m_ID; }

        /* Setters */
        void setInternalFormat(unsigned int internalFormat) { m_internalFormat = internalFormat; }
        void setImageFormat(unsigned int imageFormat) { m_imageFormat = imageFormat; }
    };

}
