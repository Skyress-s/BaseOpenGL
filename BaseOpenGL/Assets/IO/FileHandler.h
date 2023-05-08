#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "../Structure/Vertex.h"

namespace KT {
    static class FileHandler {
    public:
        static void VertexToFile(const std::string& filePath, const std::vector<Vertex>& vertices) {
            std::fstream o;
            o.open(filePath.c_str(), std::ios::out);
            o << vertices.size() << std::endl;
            for (int i = 0; i < vertices.size(); ++i) {
                o << vertices[i] << std::endl;
            }
            o.close();
        }

        static std::vector<Vertex> VertexFromFile(const std::string& filePath) {
            std::ifstream inn;
            inn.open(filePath.c_str());
            std::vector<Vertex> vertices{};
            if (inn.is_open()) {
                int n;
                Vertex vertex;
                inn >> n;
                vertices.reserve(n);
                for (int i = 0; i < n; ++i) {
                    inn >> vertex;
                    vertices.push_back(vertex);
                    // std::cout << vertex << std::endl;
                }
            }
            return vertices;
        }

        static void FromAssimp(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<int>& indices) {
            Model model = Model(filePath);
            vertices.clear();
            indices.clear();

            for (int i = 0; i < model.meshes.size(); ++i) {
                for (int x = 0; x < model.meshes[i].indices.size(); ++x) {
                    indices.push_back(model.meshes[i].indices[x]);
                }

                for (int v = 0; v < model.meshes[i].vertices.size(); ++v) {
                    ::Vertex vert = model.meshes[i].vertices[v];
                    vertices.push_back(KT::Vertex(vert.Position, vert.Normal, vert.TexCoords));
                }
            }
        }

    static void Import_obj_importer(std::string filename, std::vector<Vertex>& vertices, std::vector<int>& indices) {
        std::ifstream fileIn;
        fileIn.open(filename, std::ifstream::in);
        if (!fileIn) {
            std::cout << "OBJIMPORTER::ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return;
        }
        // One line at a time variable
        std::string oneLine;
        // One word at a time variable
        std::string oneWord;

        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec3> tempNormals;
        std::vector<glm::vec3> tempUVs;

        // Variable for constructing the indices vector
        unsigned int temp_index = 0;

        while (std::getline(fileIn, oneLine)) {
            // Doing a trick to get one word at a time
            std::stringstream sStream;
            // Pushing line into stream
            sStream << oneLine;
            // Streaming one word out of line
            oneWord = ""; // resetting the value or else the last value might survive!
            sStream >> oneWord;

            if (oneWord == "#") {
                //qDebug() << "Line is comment";
                continue;
            }
            if (oneWord == "") {
                //qDebug() << "Line is blank";
                continue;
            }
            if (oneWord == "v") {
                //qDebug() << "Line is vertex " << QString::fromStdString(oneWord) << " ";
                glm::vec3 tempVertex;
                sStream >> oneWord;
                tempVertex.x = stof(oneWord);
                sStream >> oneWord;
                tempVertex.y = stof(oneWord);
                sStream >> oneWord;
                tempVertex.z = stof(oneWord);

                // Vertex made - pushing it into vertex-vector
                tempVertices.push_back(tempVertex);

                continue;
            }
            if (oneWord == "vt") {
                //qDebug() << "Line is UV-coordinate " << QString::fromStdString(oneWord) << " ";
                glm::vec3 tempUV;
                sStream >> oneWord;
                tempUV.x = stof(oneWord);
                sStream >> oneWord;
                tempUV.y = stof(oneWord);

                // UV made - pushing it into UV-vector
                tempUVs.push_back(tempUV);

                continue;
            }
            if (oneWord == "vn") {
                //qDebug() << "Line is normal " << QString::fromStdString(oneWord) << " ";
                glm::vec3 tempNormal;
                sStream >> oneWord;
                tempNormal.x = stof(oneWord);
                sStream >> oneWord;
                tempNormal.y = stof(oneWord);
                sStream >> oneWord;
                tempNormal.z = stof(oneWord);

                // Normal made - pushing it into normal-vector
                tempNormals.push_back(tempNormal);

                continue;
            }
            if (oneWord == "f") {
                //qDebug() << "Line is a face " << QString::fromStdString(oneWord) << " ";
                int index, normal, uv;
                for (int i = 0; i < 3; i++) {
                    sStream >> oneWord; // one word read
                    std::stringstream tempWord(oneWord); // to use getline on this one word
                    std::string segment; // the numbers in the f-line
                    std::vector<std::string> segmentArray; // temp array of the numbers
                    while (std::getline(tempWord, segment, '/')) // splitting word in segments
                    {
                        segmentArray.push_back(segment);
                    }
                    index = std::stoi(segmentArray[0]); // first is vertex
                    if (segmentArray[1] != "") // second is uv
                    {
                        uv = std::stoi(segmentArray[1]);
                    }
                    else {
                        //qDebug() << "No uvs in mesh";
                        uv = 0;
                    }
                    normal = std::stoi(segmentArray[2]); // third is normal

                    // Fixing the indices
                    // because obj f-lines starts with 1, not 0
                    --index;
                    --uv;
                    --normal;

                    if (uv > -1) // uv present!
                    {
                        vertices.push_back(Vertex(tempVertices[index].x, tempVertices[index].y, tempVertices[index].z,
                                                   tempNormals[normal].x, tempNormals[normal].y, tempNormals[normal].z,
                                                   tempUVs[uv].x, tempUVs[uv].y));
                    }
                    else {
                        vertices.push_back(Vertex(tempVertices[index].x, tempVertices[index].y, tempVertices[index].z,
                                                   tempNormals[normal].x, tempNormals[normal].y, tempNormals[normal].z,
                                                   0.0f, 0.0f));
                    }
                    indices.push_back(temp_index++);
                }
                continue;
            }
        }
        fileIn.close();
    }

        
    };
}
