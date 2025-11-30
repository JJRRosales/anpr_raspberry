#include 'anpr_raspberry/Config.h'
#include <string>
#include <cstdlib>
#include <iostream>

std::string Config::getEnvVar(const char* varName, const std::string& defaultValue) {
    const char* envValue = std::getenv(varName);
    if (envValue != nullptr) {
        std::cout << "Using " << varName << " from environment: " << envValue << std::endl;
        return std::string(envValue);
    }
    std::cout << varName << " not set, using default: " << defaultValue << std::endl;
    return defaultValue;
}

int Config::getEnvVarInt(const char* varName, int defaultValue) {
    const char* envValue = std::getenv(varName);
    if (envValue != nullptr) {
        try {
            return std::stoi(envValue);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing " << varName << ", using default: " << defaultValue << std::endl;
        }
    }
    return defaultValue;
}

float Config::getEnvVarFloat(const char* varName, float defaultValue) {
    const char* envValue = std::getenv(varName);
    if (envValue != nullptr) {
        try {
            return std::stof(envValue);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing " << varName << ", using default: " << defaultValue << std::endl;
        }
    }
    return defaultValue;
}

// Camera params
const int CAMERA_WIDTH = getEnvVarInt("ANPR_CAMERA_WIDTH", 640);
const int CAMERA_HEIGHT = getEnvVarInt("ANPR_CAMERA_HEIGHT", 480);
const int CAMERA_FPS = getEnvVarInt("ANPR_CAMERA_FPS", 30);
const int CAMERA_DEVICE_ID = getEnvVarInt("ANPR_CAMERA_DEVICE_ID", 0);

// Buffer params
const int FRAME_BUFFER_SIZE = getEnvVarInt("ANPR_FRAME_BUFFER_SIZE", 30);

// Model params        
const int MODEL_WIDTH = getEnvVarInt("ANPR_MODEL_WIDTH", 640);
const int MODEL_HEIGHT = getEnvVarInt("ANPR_MODEL_HEIGHT", 480);
const float DETECTION_CONFIDENCE_THRESHOLD = getEnvVarFloat("ANPR_DETECTION_CONFIDENCE_THRESHOLD", 0.5f);
const std::string MODEL_PATH = getEnvVar("ANPR_MODEL_PATH", "/model/anpr_model.tflite");

// AWS params
const std::string AWS_ENDPOINT = getEnvVar("ANPR_AWS_ENDPOINT", ".amazonaws.com");
const std::string CLIENT_ID = getEnvVar("ANPR_CLIENT_ID", "anpr_raspberry_pi");
const std::string TOPIC = getEnvVar("ANPR_TOPIC", "anpr/plates");
const std::string CERT_PATH = getEnvVar("ANPR_CERT_PATH", "/certs/certificate.pem.crt");
const std::string KEY_PATH = getEnvVar("ANPR_KEY_PATH", "/certs/private.pem.key");
const std::string CA_PATH = getEnvVar("ANPR_CA_PATH", "/certs/AmazonRootCA1.pem");
