#include "render/shaders/ComputeShader.h"
#include "core/AssetManager.h"
#include "utils/MathUtils.h"
using namespace juno;

ComputeShader::ComputeShader(const std::string& fp, unsigned int numObjects, unsigned int wgSize)
{
    this->filepath = fp;
    this->workGroupSize = wgSize;
    this->numObjects = numObjects;


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

    for(unsigned int i = 0; i < numObjects; i++)
    {
        Particle p;
        p.position = glm::vec3( sinf(i)/(min(i, 50000)) * 5000, cosf(i)/(min(i, 50000)) * 5000, (tanf(-i * M_PI + M_PI*2)/i) * 1000);
        p.velocity = glm::vec3(100, 100, 100);
        p.mass = 0.1f;
        p.scale = 1.0f;
        particles.emplace_back(p);
    }

    loc_viewMatrix = glGetUniformLocation(this->progID, "view");
    loc_projectionMatrix = glGetUniformLocation(this->progID, "projection");




    glGenVertexArrays(1, &computeVaoID);
    glBindVertexArray(computeVaoID);

    glGenBuffers(1, &computeSSBO);
    glBindBuffer(GL_ARRAY_BUFFER, computeSSBO);
    glBufferData(GL_ARRAY_BUFFER, numObjects * sizeof(Particle), &particles[0], GL_STREAM_DRAW);

    /* */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(glm::vec3)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(glm::vec3)*2));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 7));

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 3));
	// glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 6));
	// glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 7));
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    
    /* */
    glBindVertexArray(0);




}


void ComputeShader::bind()
{

    


    /* -------------- */
}

GLuint ComputeShader::getCSID()
{
    return csProgID;
}

GLuint ComputeShader::getParticleSSBO()
{
    return loc_particleBuffer;
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
