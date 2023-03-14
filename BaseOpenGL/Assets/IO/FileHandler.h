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

        
    };
}
