// #include "Renderer/RenderObjects/Shaders/ComputeShader.h"
// #include "Core/AssetManager.h"
// #include "Utils/Math.h"
// using namespace Juno;

// ComputeShader::ComputeShader(std::vector<Particle>& particles, const std::string& fp, unsigned int numWG) : numWorkGroups(numWG), numObjects(particles.size())
// {
//     this->filepath = fp;
//     CompileCShader();
//     Setup(particles);
// }

// void ComputeShader::CompileCShader()
// {
//     GLuint vertShader = AssetManager::Get().ReadShaderComponentFile(filepath, GL_VERTEX_SHADER);
//     GLuint fragShader = AssetManager::Get().ReadShaderComponentFile(filepath, GL_FRAGMENT_SHADER);
//     //GLuint geomShader = AssetManager::Get().ReadShaderComponentFile(filepath, GL_GEOMETRY_SHADER);
//     GLuint compShader = AssetManager::Get().ReadShaderComponentFile(filepath, GL_COMPUTE_SHADER);
//     //this->progID = glCreateProgram();

//     // glAttachShader(progID, vertShader);
//     // glAttachShader(progID, fragShader);
//     // //glAttachShader(progID, geomShader);
//     // glLinkProgram(progID);

//     LinkErrorCheck(vertShader, GL_VERTEX_SHADER);
//     LinkErrorCheck(fragShader, GL_FRAGMENT_SHADER);
//     //linkErrorCheck(geomShader, GL_GEOMETRY_SHADER);

//     // this->csProgID = glCreateProgram();
//     // glAttachShader(csProgID, compShader);
//     // glLinkProgram(csProgID);

//     LinkErrorCheck(compShader, GL_COMPUTE_SHADER);
// }

// void ComputeShader::Setup(std::vector<Particle>& particles)
// {
//     //setActive();
//     // loc_viewMatrix = glGetUniformLocation(this->progID, "view");
//     //loc_projectionMatrix = glGetUniformLocation(this->progID, "projection");

//     // glUseProgram(csProgID);
//     // glGenVertexArrays(1, &computeVaoID);
//     // glBindVertexArray(computeVaoID);



//     // glGenBuffers(1, &computeSSBO);
//     // glBindBuffer(GL_ARRAY_BUFFER, computeSSBO);
//     // glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), &particles[0], GL_STREAM_DRAW);

//     // /* */
//     // glEnableVertexAttribArray(0);
//     // glEnableVertexAttribArray(1);
//     // glEnableVertexAttribArray(2);

//     // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
//     // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 3 ));
//     // glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 6 ));

  
//     // glVertexAttribDivisor(0, 1);
//     // glVertexAttribDivisor(1, 1);
//     // glVertexAttribDivisor(2, 1);
//     // // glVertexAttribDivisor(3, 1);
    
//     // /* */
//     // glBindVertexArray(0);
//     // glBindBuffer(GL_ARRAY_BUFFER, 0);


// }


// void ComputeShader::BindCS()
// {
//     //glUseProgram(csProgID);
    


//     /* -------------- */
// }

// void ComputeShader::Compute()
// {
// //     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeSSBO);
// //     glDispatchCompute( (numObjects/numWorkGroups)+1, 1, 1);
// //     glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

// //     glUseProgram(progID);
// //     glBindVertexArray(computeVaoID);
// }




// GLuint ComputeShader::GetCSID()
// {
//     return csProgID;
// }

// GLuint ComputeShader::GetParticleSSBO()
// {
//     return computeSSBO;
// }

// GLuint ComputeShader::GetParticleVaoID()
// {
//     return computeVaoID;
// }


// unsigned int ComputeShader::GetNumObjects()
// {
//     return numObjects;
// }

// // std::vector<glm::vec4>& ComputeShader::getForces()
// // {
// //     return forces;
// // }
