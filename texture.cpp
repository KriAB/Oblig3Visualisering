#include "innpch.h"
#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include "texture.h"

Texture::Texture(GLuint textureUnit) : QOpenGLFunctions_4_1_Core()
{
    initializeOpenGLFunctions();
    //small dummy texture
    for (int i=0; i<16; i++)
        pixels[i] = 0;
    pixels[0] = 255;
    pixels[4] = 255;
    pixels[8] = 255;
    pixels[9] = 255;
    pixels[10] = 255;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &mId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mId);
    qDebug() << "Texture::Texture() id = " << mId;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 reinterpret_cast<const GLvoid*>(pixels));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    setTexture(textureUnit);
}

/**
 \brief Texture::Texture() Read a bitmap file and create a texture with standard parameters
 \param filename The name of the bmp file containing a texture
 First one 2D texture is generated from
 - glGenTextures()
 Then the OpenGL functions
 - glBindTexture()
 - glTexParameteri()
 - glTexImage2D()
 are used. The texture can be retrieved later by using the function id()
 */
Texture::Texture(const std::string& filename, GLuint textureUnit): QOpenGLFunctions_4_1_Core()
{
    initializeOpenGLFunctions();
    readBitmap(filename);
    setTexture(textureUnit);
}

/**
    \brief Texture::id() Return the id of a previously generated texture object
    \return The id of a previously generated texture object
 */
GLuint Texture::id() const
{
    return mId;
}

void Texture::readBitmap(const std::string &filename)
{
    OBITMAPFILEHEADER bmFileHeader;
    OBITMAPINFOHEADER bmInfoHeader;

    std::string fileWithPath =  gsl::assetFilePath + "Textures/" + filename;

    std::ifstream file;
    file.open (fileWithPath.c_str(), std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char *) &bmFileHeader, 14);

        file.read((char *) &bmInfoHeader, sizeof(OBITMAPINFOHEADER));
        mColumns = bmInfoHeader.biWidth;
        mRows = bmInfoHeader.biHeight;
        mnByte = bmInfoHeader.biBitCount / 8;

        mBitmap = new unsigned char[mColumns * mRows * mnByte];
        file.read((char *) mBitmap, mColumns * mRows * mnByte);
        file.close();
    }
    else
    {
        qDebug() << "Can not read " << QString(fileWithPath.c_str());
    }
    unsigned char tmp;
    // switch red and blue
    for (int k = 0; k < mColumns * mRows * mnByte; k += mnByte) {
        tmp = mBitmap[k];
        mBitmap[k] = mBitmap[k + 2];
        mBitmap[k + 2] = tmp;
    }
    qDebug() << "Texture read: " << QString(fileWithPath.c_str());
}

void Texture::setTexture(GLuint textureUnit)
{
    glGenTextures(1, &mId);
    // activate the texture unit first before binding texture
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, mId);
    qDebug() << "Texture::Texture() id = " << mId;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                mColumns,
                mRows,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                mBitmap);
    glGenerateMipmap(GL_TEXTURE_2D);
}
