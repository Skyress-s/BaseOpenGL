#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Shader/Shader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


class Model 
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader &shader);

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

struct KTTexture2D {
public:
    KTTexture2D() = default;
    KTTexture2D(const KTTexture2D& other) { // copy constructor
        width = other.width;
        height = other.height;
        nrChannels = other.nrChannels;
        textureID = other.textureID;
        
        data = new unsigned char[width * height * nrChannels];
        memcpy(data, other.data, width * height * nrChannels); 
    }

    
    
    unsigned char* data{}; 
    unsigned int width;
    unsigned int height;
    unsigned int nrChannels;
    unsigned int textureID;

    std::vector<int> ValueAt(float u, float v) {
        // clamp u and v to [0, 1]

        // use u and v to get the index of the pixel
        int x = u * (width-1); // max index = size - 1
        int y = v * (height-1);
        y = height - y - 1; // flip y axis // thank you github copilot

        if (x < 0) 
            x = 0;
        if (x > width-1)
            x = width-1;
        if (y < 0) 
            y = 0;
        if (y > height-1)
            y = height-1;
        
        int index = (y * width + x) * 4;

        // get the red, green, and blue color channel values of the pixel
        std::vector<int> values(nrChannels, 0);
        for (int i = 0; i < nrChannels; ++i) {
            values[i] = (data[index+i]);
        }
        
        return values;
    }
    std::vector<int> ValueAt(float u, float v, int offsetx, int offsety) {
        // clamp u and v to [0, 1]

        // use u and v to get the index of the pixel
        int x = u * (width-1); // max index = size - 1
        int y = v * (height-1);
        y = height - y - 1; // flip y axis // thank you github copilot

        // apply offset
        x+=offsetx;
        y+=offsety;
        
        if (x < 0) 
            x = 0;
        if (x > width-1)
            x = width-1;
        if (y < 0) 
            y = 0;
        if (y > height-1)
            y = height-1;
        
        int index = (y * width + x) * 4;

        // get the red, green, and blue color channel values of the pixel
        std::vector<int> values{};
        for (int i = 0; i < nrChannels; ++i) {
            values.push_back(data[index+i]);
        }
        
        return values;
    }
    //TODO remember to deallocate memory
};
KTTexture2D KTTextureFromFile(const string &directory, bool gamma = false);

#endif
