#pragma once

extern class GlobalLightData {
public:
    static GlobalLightData& GetInstance() {
        static GlobalLightData instance;
        return instance;
    }

private:
    GlobalLightData() = default;
    
public:
    
    GlobalLightData(GlobalLightData const&) = delete;
    void operator=(GlobalLightData const&) = delete;

public:
    glm::vec3 position{};
    
};
