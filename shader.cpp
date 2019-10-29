#include "innpch.h"
#include "shader.h"

//#include "GL/glew.h" - using QOpenGLFunctions instead

#include "camera.h"
#include "matrix4x4.h"

Shader::Shader(const std::string shaderName, const GLchar *geometryPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    std::string vertexName = shaderName + ".vert";
    std::string fragmentName = shaderName + ".frag";

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    std::string vertexWithPath = gsl::shaderFilePath + vertexName;
    std::string fragmentWithPath = gsl::shaderFilePath + fragmentName;

    // Open files and check for errors
    vShaderFile.open( vertexWithPath );
    if(!vShaderFile)
        std::cout << "ERROR SHADER FILE " << vertexWithPath << " NOT SUCCESFULLY READ" << std::endl;
    fShaderFile.open( fragmentWithPath );
    if(!fShaderFile)
        std::cout << "ERROR SHADER FILE " << vertexWithPath << " NOT SUCCESFULLY READ" << std::endl;
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );

    //Do the same thing with the geometry shader if it exists:
    const GLchar *gShaderCode = nullptr;
    std::string geometryCode;
    if (geometryPath)
    {
        std::string geometryCode;
        std::ifstream gShaderFile;
        // Open files and check for errors
        gShaderFile.open( geometryPath );
        if(!gShaderFile)
            std::cout << "ERROR SHADER FILE " << geometryPath << " NOT SUCCESFULLY READ" << std::endl;

        std::stringstream gShaderStream;
        // Read file's buffer contents into streams
        gShaderStream << gShaderFile.rdbuf( );
        // close file handlers
        gShaderFile.close( );
        // Convert stream into string
        geometryCode = gShaderStream.str( );

        gShaderCode = geometryCode.c_str( );
    }

    // 2. Compile shaders
    GLuint vertex{0}, fragment{0}, geometry{0};
    GLint success{0};
    GLchar infoLog[512]{};

    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER VERTEX " << shaderName << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER FRAGMENT " << shaderName << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Geometry Shader
    if (gShaderCode)
    {
        geometry = glCreateShader( GL_GEOMETRY_SHADER );
        glShaderSource( geometry, 1, &gShaderCode, nullptr );
        glCompileShader( geometry );
        // Print compile errors if any
        glGetShaderiv( geometry, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( geometry, 512, nullptr, infoLog );
            std::cout << "ERROR SHADER GEOMETRY " << geometryPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    // Shader Program
    this->program = glCreateProgram( );
    glAttachShader( this->program, vertex );
    glAttachShader( this->program, fragment );
    if(geometryPath)
        glAttachShader( this->program, geometry );
    glLinkProgram( this->program );
    // Print linking errors if any
    glGetProgramiv( this->program, GL_LINK_STATUS, &success );
    if (!success)
    {
        glGetProgramInfoLog( this->program, 512, nullptr, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    glDeleteShader( vertex );
    glDeleteShader( fragment );
    if(geometryPath)
        glDeleteShader(geometry);

    std::cout << "Shader read: " << shaderName << std::endl;
}

Shader::~Shader()
{
    qDebug() << "shader program " << program;
    glDeleteProgram(program);
}

void Shader::use()
{
    glUseProgram( this->program );
}

GLuint Shader::getProgram() const
{
    return program;
}

void Shader::transmitUniformData(gsl::Matrix4x4 *modelMatrix, Material *material)
{
    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrix->constData());
}

void Shader::setCurrentCamera(Camera *currentCamera)
{
    mCurrentCamera = currentCamera;
}

Camera *Shader::getCurrentCamera() const
{
    return mCurrentCamera;
}
