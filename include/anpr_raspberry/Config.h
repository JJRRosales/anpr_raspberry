#pragma once
#include <string>

class Config {
public:
    // Camera params
    static const int CAMERA_WIDTH;
    static const int CAMERA_HEIGHT;
    static const int CAMERA_FPS;
    static const int CAMERA_DEVICE_ID;
    
    // Buffer params
    static const int FRAME_BUFFER_SIZE;
    
    // Model params        
    static const int MODEL_WIDTH;
    static const int MODEL_HEIGHT;
    static const float DETECTION_CONFIDENCE_THRESHOLD;
    static const std::string MODEL_PATH;
    
    // AWS params
    static const std::string AWS_ENDPOINT;
    static const std::string CLIENT_ID;
    static const std::string TOPIC;
    static const std::string CERT_PATH;
    static const std::string KEY_PATH;
    static const std::string CA_PATH;

private:
    static std::string getEnvVar(const char* varName, const std::string& defaultValue = "");
    static int getEnvVarInt(const char* varName, int defaultValue);
    static float getEnvVarFloat(const char* varName, float defaultValue);
    
};

