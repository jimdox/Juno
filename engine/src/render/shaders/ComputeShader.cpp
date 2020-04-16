#include "render/shaders/ComputeShader.h"
#include "core/AssetManager.h"
#include "utils/MathUtils.h"
using namespace juno;

ComputeShader::ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int numWG) : numWorkGroups(numWG), numObjects(numObjects)
{
    this->filepath = fp;
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

void ComputeShader::setup()
{
    loc_viewMatrix = glGetUniformLocation(this->progID, "view");
    loc_projectionMatrix = glGetUniformLocation(this->progID, "projection");

    glGenVertexArrays(1, &computeVaoID);
    glBindVertexArray(computeVaoID);

    for(unsigned int i = 0; i < numObjects; i++)
    {
        Particle p;
        p.position.x = 100 * cosf(i);
        p.position.y = 100 * sin(i) * cosf(i);
        p.position.z = 100 * sinf(i * M_PI/300);
        p.velocity = glm::vec3(sinf(2 * M_PI * i) + cosf(-i), cosf(i/(2 * M_PI)), sinf(2 * M_PI / i));   
        p.mass = 0.1f;
        p.scale = 1.0f;
        particles.emplace_back(p);
    }



    glGenBuffers(1, &computeSSBO);
    glBindBuffer(GL_ARRAY_BUFFER, computeSSBO);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), &particles[0], GL_DYNAMIC_COPY);

    /* */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 3 ));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 6 ));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 7 ));

  
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    
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

std::vector<Particle>& ComputeShader::getParticles()
{
    return particles;
}



// std::vector<glm::vec4>& ComputeShader::getForces()
// {
//     return forces;
// }
