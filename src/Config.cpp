#include "anpr_raspberry/Config.h"
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

// Camera params - ADD Config:: SCOPE
const int Config::CAMERA_WIDTH = Config::getEnvVarInt("ANPR_CAMERA_WIDTH", 640);
const int Config::CAMERA_HEIGHT = Config::getEnvVarInt("ANPR_CAMERA_HEIGHT", 480);
const int Config::CAMERA_FPS = Config::getEnvVarInt("ANPR_CAMERA_FPS", 30);
const int Config::CAMERA_DEVICE_ID = Config::getEnvVarInt("ANPR_CAMERA_DEVICE_ID", 0);

// Buffer params
const int Config::FRAME_BUFFER_SIZE = Config::getEnvVarInt("ANPR_FRAME_BUFFER_SIZE", 30);

// Model params        
const int Config::MODEL_WIDTH = Config::getEnvVarInt("ANPR_MODEL_WIDTH", 640);
const int Config::MODEL_HEIGHT = Config::getEnvVarInt("ANPR_MODEL_HEIGHT", 480);
const float Config::DETECTION_CONFIDENCE_THRESHOLD = Config::getEnvVarFloat("ANPR_DETECTION_CONFIDENCE_THRESHOLD", 0.5f);
const std::string Config::MODEL_PATH = Config::getEnvVar("ANPR_MODEL_PATH", "/model/anpr_model.tflite");

// AWS params
const std::string Config::AWS_ENDPOINT = Config::getEnvVar("ANPR_AWS_ENDPOINT", ".amazonaws.com");
const std::string Config::CLIENT_ID = Config::getEnvVar("ANPR_CLIENT_ID", "anpr_raspberry_pi");
const std::string Config::TOPIC = Config::getEnvVar("ANPR_TOPIC", "anpr/plates");
const std::string Config::CERT_PATH = Config::getEnvVar("ANPR_CERT_PATH", "/certs/certificate.pem.crt");
const std::string Config::KEY_PATH = Config::getEnvVar("ANPR_KEY_PATH", "/certs/private.pem.key");
const std::string Config::CA_PATH = Config::getEnvVar("ANPR_CA_PATH", "/certs/AmazonRootCA1.pem");