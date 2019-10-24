// #include "entity/terrain/Terrain.h"
// #include "core/bxAssetLoader.h"

// using namespace bx;

// Terrain::Terrain(float x_coord, float z_coord, Texture& texture)
// {

//     this->x = x_coord * BLOCK_SIZE;
//     this->z = z_coord * BLOCK_SIZE;

//     unsigned int area = NUM_VERTICES * NUM_VERTICES;

//     std::vector<float> vertices;
//     std::vector<float> normals;
//     std::vector<float> texCoordinates;
//     std::vector<unsigned int> indices;

//     vertices.resize(area * 3);
//     normals.resize(area * 3);
//     texCoordinates.resize(area * 2);
//     indices.resize(6 * (NUM_VERTICES-1) * (NUM_VERTICES-1));

//     unsigned int vPtr = 0;

//     for(unsigned int i=0; i < NUM_VERTICES; i++)
//     {
//         for(unsigned int j=0; j < NUM_VERTICES; j++)
//         {
//             vertices[vPtr * 3] = ((float)j)/((float)NUM_VERTICES - 1) * BLOCK_SIZE;
//             vertices[vPtr * 3 +1] = 0;
//             vertices[vPtr * 3 +2] = ((float)i)/((float)NUM_VERTICES - 1) * BLOCK_SIZE;

//             normals[vPtr * 3] = 0;
//             normals[vPtr * 3 +1] = 1;
//             normals[vPtr * 3 +2] = 0;

//             texCoordinates[vPtr * 2] = ((float)j)/((float)NUM_VERTICES - 1);
//             texCoordinates[vPtr * 2 +1] = ((float)i)/((float)NUM_VERTICES - 1);
//             vPtr++;
//         }
//     }

//     int index = 0;

//     for(int grid_z = 0; grid_z < NUM_VERTICES - 1; grid_z++)
//     {
//         for(int grid_x; grid_x < NUM_VERTICES - 1; grid_x++)
//         {
//             unsigned int upperLeft = (grid_z*NUM_VERTICES) + grid_x;
//             unsigned int upperRight = upperLeft + 1;
//             unsigned int lowerLeft = ((grid_z+1) * NUM_VERTICES) + grid_x;
//             unsigned int lowerRight = lowerLeft + 1; 

//             indices[index++] = upperLeft;
//             indices[index++] = lowerLeft;
//             indices[index++] = upperRight;
//             indices[index++] = upperRight;
//             indices[index++] = lowerLeft;
//             indices[index++] = lowerRight;

//         }
//     }

//         this->mesh = Mesh(vertices, texCoordinates, normals, indices);
//         auto [id, numIndices] = bxImport::loadToVAO(vertices, texCoordinates, normals, indices);
//         mesh.assignVAO(id, numIndices);


// }

// Terrain::~Terrain()
// {

// }

// Mesh& Terrain::getMesh()
// {
//     return this->mesh;
// }

// float Terrain::getXCoord()
// {
//     return this->x;
// }

// float Terrain::getZCoord()
// {
//     return this->z;
// }