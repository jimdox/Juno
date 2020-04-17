
#include "render/shaders/ComputeShader.h"
#include "core/AssetManager.h"
#include "utils/MathUtils.h"
using namespace juno;

ComputeShader::ComputeShader(std::vector<Particle>& particles, const std::string& fp, unsigned int numWG) : numWorkGroups(numWG), numObjects(particles.size())
{
    this->filepath = fp;
    compileCShader();
    setup(particles);
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
}

void ComputeShader::setup(std::vector<Particle>& particles)
{
    //setActive();
    loc_viewMatrix = glGetUniformLocation(this->progID, "view");
    loc_projectionMatrix = glGetUniformLocation(this->progID, "projection");

    glUseProgram(csProgID);
    glGenVertexArrays(1, &computeVaoID);
    glBindVertexArray(computeVaoID);



    glGenBuffers(1, &computeSSBO);
    glBindBuffer(GL_ARRAY_BUFFER, computeSSBO);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), &particles[0], GL_STREAM_DRAW);

    /* */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 3 ));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 6 ));

  
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    // glVertexAttribDivisor(3, 1);
    
    /* */
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



}


void ComputeShader::bindCS()
{
    glUseProgram(csProgID);
    


    /* -------------- */
}

void ComputeShader::run()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeSSBO);
    glDispatchCompute( (numObjects/numWorkGroups)+1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glUseProgram(progID);
    glBindVertexArray(computeVaoID);
}




GLuint ComputeShader::getCSID()
{
    return csProgID;
}

GLuint ComputeShader::getParticleSSBO()
{
    return computeSSBO;
}

GLuint ComputeShader::getParticleVaoID()
{
    return computeVaoID;
}


unsigned int ComputeShader::getNumObjects()
{
    return numObjects;
}

// std::vector<glm::vec4>& ComputeShader::getForces()
// {
//     return forces;
// }
