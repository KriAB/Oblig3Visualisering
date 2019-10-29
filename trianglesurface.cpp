#include "innpch.h"
#include "trianglesurface.h"

TriangleSurface::TriangleSurface() : VisualObject() {
    Vertex v{};
    v.set_xyz(0,0,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0,0); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(0,0,1);
    mVertices.push_back(v);
    v.set_xyz(0,0,0); v.set_rgb(0,1,0);
    mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(1,0,0);
    mVertices.push_back(v);
    v.set_xyz(0,0.5,0); v.set_rgb(0,0,1);
    mVertices.push_back(v);

    mMatrix.setToIdentity();
}

TriangleSurface::TriangleSurface(std::string filename) : VisualObject()
{
    readFile(filename);
    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface()
{
}

void TriangleSurface::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    glUseProgram(mMaterial.mShader->getProgram());
    glBindVertexArray( mVAO );
    mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}

void TriangleSurface::readFile(std::string filename) {
    std::ifstream inn;
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    inn.open(fileWithPath);

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        inn.close();
        qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }
}

void TriangleSurface::writeFile(std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {
        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            vertex = *it;
            ut << vertex << std::endl;
        }
        ut.close();
    }
}


void TriangleSurface::construct()
{
    float xmin=0.0f, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
    for (auto x=xmin; x<xmax; x+=h)
        for (auto y=ymin; y<ymax; y+=h)
        {
            float z = sin(gsl::PI*x)*sin(gsl::PI*y);
            mVertices.push_back(Vertex{x,y,z,x,y,z});
            z = sin(gsl::PI*(x+h))*sin(gsl::PI*y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = sin(gsl::PI*x)*sin(gsl::PI*(y+h));
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            z = sin(gsl::PI*(x+h))*sin(gsl::PI*y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = sin(gsl::PI*(x+h))*sin(gsl::PI*(y+h));
            mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
        }
}
