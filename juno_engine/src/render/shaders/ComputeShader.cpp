#include "render/shaders/ComputeShader.h"
#include "core/AssetManager.h"
using namespace juno;

ComputeShader::ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int wgSize)
{
    this->filepath = fp;
    this->workGroupSize = wgSize;
    this->numObjects = numObjects;
    positions.reserve(numObjects);
    velocities.reserve(numObjects);


    compileCShader();
}

void ComputeShader::compileCShader()
{
    GLuint vertShader = AssetManager::get().loadShaderComponentFile(filepath, GL_VERTEX_SHADER);
    GLuint fragShader = AssetManager::get().loadShaderComponentFile(filepath, GL_FRAGMENT_SHADER);
    //GLuint geomShader = AssetManager::get().loadShaderComponentFile(filepath, GL_GEOMETRY_SHADER);
    GLuint compShader = AssetManager::get().loadShaderComponentFile(filepath, GL_COMPUTE_SHADER);
    this->progID = glCreateProgram();

    glAttachShader(progID, vertShader);
    glAttachShader(progID, fragShader);
    //glAttachShader(progID, geomShader);
    glLinkProgram(progID);

    linkErrorCheck(vertShader, GL_VERTEX_SHADER);
    linkErrorCheck(fragShader, GL_FRAGMENT_SHADER);
    //linkErrorCheck(geomShader, GL_GEOMETRY_SHADER);

    this->csProgID = glCreateProgram();
    glAttachShader(csProgID, compShader);
    glLinkProgram(csProgID);

    linkErrorCheck(compShader, GL_COMPUTE_SHADER);
    //JN_INFO("Linking Compute Shader!");
}

void ComputeShader::init()
{
    loc_viewMatrix = glGetUniformLocation(progID, "view");

    glm::vec4 * csPositions = (glm::vec4 *) calloc(numObjects, sizeof(glm::vec4));



    for(unsigned int i = 0; i < numObjects; i++)
    {
        float x = (static_cast<float>((rand())) / static_cast<float>(RAND_MAX));
        float y = (static_cast<float>((rand())) / static_cast<float>(RAND_MAX));
        float z = (static_cast<float>((rand())) / static_cast<float>(RAND_MAX));
        //JN_INFO("{}, {}, {}", x, y, z);
        csPositions[i] = glm::vec4(x,y,z,1.0f);
    }

    /* generate a buffer for the positions */
    glGenBuffers(1, &dataBuffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec3), csPositions, GL_DYNAMIC_COPY);

}


void ComputeShader::bind()
{

    


    /* -------------- */
}

GLuint ComputeShader::getCSID()
{
    return csProgID;
}

GLuint ComputeShader::getPositionBufferID()
{
    return dataBuffers[0];
}

GLuint ComputeShader::getVelocityBufferID()
{
    return dataBuffers[1];
}

GLuint ComputeShader::getColorBufferID()
{
    return dataBuffers[2];
}

// GLuint ComputeShader::getForceBufferID()
// {
//     return dataBuffers[3];
// }

unsigned int ComputeShader::getNumObjects()
{
    return numObjects;
}

std::vector<glm::vec4>& ComputeShader::getPositions()
{
    return positions;
}

std::vector<glm::vec4>& ComputeShader::getVelocities()
{
    return velocities;
}

std::vector<glm::vec4>& ComputeShader::getColors()
{
    return colors;
}

// std::vector<glm::vec4>& ComputeShader::getForces()
// {
//     return forces;
// }
