#include <iostream>
#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>
#include <thread>
#include <atomic>
#include <memory>
#include "anpr_raspberry/FrameBuffer.h"
#include "anpr_raspberry/InferenceFrame.h"



// Global vars
FrameBuffer processingBuffer(30);
std::atomic<bool> keepRunning = true;
std::unique_ptr<tflite::Interpreter> interpreter;

const int MODEL_WIDTH = 640;
const int MODEL_HEIGHT = 480;
const char* getModelPath();
const char* MODEL_PATH = getModelPath();

const char* getModelPath() {
    const char* env_path = std::getenv("ANPR_MODEL_PATH");
    
    if (env_path != nullptr) {
        return env_path;
    } else {
        std::cerr << "--- CRITICAL ERROR ---" << std::endl;
        std::cerr << "The environment variable ANPR_MODEL_PATH is not set." << std::endl;
        std::cerr << "Please set ANPR_MODEL_PATH to the full path of the model file (e.g., /path/to/model.tflite)." << std::endl;
        std::cerr << "----------------------" << std::endl;
        std::exit(EXIT_FAILURE); 
    }
}

std::unique_ptr<tflite::Interpreter> loadModelAndCreateInterpreter(const char* model_path) {
    // 1. Load the model
    std::unique_ptr<tflite::FlatBufferModel> model = 
        tflite::FlatBufferModel::BuildFromFile(model_path);
    
    if (!model) {
        std::cerr << "Failed to load model: " << model_path << std::endl;
        return nullptr;
    }

    // 2. Create OpResolver
    tflite::ops::builtin::BuiltinOpResolver resolver;

    // 3. Build the interpreter
    std::unique_ptr<tflite::Interpreter> interpreter;
    if (tflite::InterpreterBuilder(*model, resolver)(&interpreter) != kTfLiteOk) {
        std::cerr << "Failed to create interpreter" << std::endl;
        return nullptr;
    }

    // 4. Allocate tensors
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "Failed to allocate tensors" << std::endl;
        return nullptr;
    }

    return interpreter;
}

bool runInference(tflite::Interpreter* interpreter, const cv::Mat& input, 
                  cv::Rect& detectedBox, float& confidence) {
    
    TfLiteTensor* input_tensor = interpreter->tensor(interpreter->inputs()[0]);

    cv::Mat processed;
    cv::resize(input, processed, cv::Size(input_tensor->dims->data[2], 
                                         input_tensor->dims->data[1]));
    
    // Convert BGR to RGB
    cv::cvtColor(processed, processed, cv::COLOR_BGR2RGB);
    
    // Normalize pixel values if required by model
    processed.convertTo(processed, CV_32F, 1.0/255.0);
    
    // Copy data to input tensor
    float* input_data = interpreter->typed_input_tensor<float>(0);
    memcpy(input_data, processed.data, processed.total() * processed.elemSize());

    // Run inference
    if (interpreter->Invoke() != kTfLiteOk) {
        std::cerr << "Inference failed" << std::endl;
        return false;
    }

    // Process output
    TfLiteTensor* output_tensor = interpreter->tensor(interpreter->outputs()[0]);
    float* output_data = interpreter->typed_output_tensor<float>(0);
    
    // [x, y, width, height, confidence]
    if (output_data[4] > 0.5f) { // confidence threshold
        detectedBox = cv::Rect(
            static_cast<int>(output_data[0] * input.cols),
            static_cast<int>(output_data[1] * input.rows),
            static_cast<int>(output_data[2] * input.cols),
            static_cast<int>(output_data[3] * input.rows)
        );
        confidence = output_data[4];
        return true;
    }
    
    return false;
}

// --- THREAD 1: CAPTURE & INFERENCE ---
void captureAndInferenceThread(int width, int height) {
    cv::VideoCapture cap(0); 
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return;
    }

    cv::Mat frame;

    while (keepRunning) {
        cap >> frame; 
        if (frame.empty()) {
            std::cerr << "Warning: Empty frame received" << std::endl;
            continue;
        }

        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(width, height));
     
        cv::Rect detectedBox;
        float confidence = 0.0f;
        bool objectDetected = runInference(interpreter.get(), frame, detectedBox, confidence);

        if (objectDetected && confidence > 0.5f) {
            InferenceFrame result;
            result.originalFrame = frame.clone(); 
            result.boundingBox = detectedBox;
            result.classId = 1;
            result.confidence = confidence;
        }
        
        // std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

// --- THREAD 2: OCR & CLOUD UPLOAD ---
void ocrAndUploadThread() {
    // Setup OCR Engine (Tesseract would go here)
    // Setup AWS IoT SDK would go here

    std::cout << "OCR thread started" << std::endl;
    
    while (keepRunning) {
        try {
            InferenceFrame data;
            if (processingBuffer.popFrame(data, std::chrono::milliseconds(100))) {
                
                // Ensure bounding box is within image bounds
                cv::Rect validBox = data.boundingBox & 
                    cv::Rect(0, 0, data.originalFrame.cols, data.originalFrame.rows);
                
                if (validBox.area() > 0) {
                    // Crop the region of interest
                    cv::Mat roi = data.originalFrame(validBox);
                    
                    // TODO: Run OCR on 'roi'
                    std::string text = "ABC123"; // Replace with actual OCR
                    float ocrConfidence = 0.95f; // Replace with actual confidence
                    
                    std::cout << "Detected: " << text << " with confidence: " 
                             << ocrConfidence << std::endl;

                    // Filter by confidence and upload
                    if (ocrConfidence > 0.80f) {
                        std::cout << "High confidence (" << ocrConfidence 
                                 << "). Would upload to AWS: " << text << std::endl;
                        // TODO: Call AWS IoT MQTT Publish
                    } else {
                        std::cout << "Low OCR confidence. Discarding: " << text << std::endl;
                    }
                } else {
                    std::cerr << "Warning: Invalid bounding box" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in OCR thread: " << e.what() << std::endl;
        }
    }
}

int main() {
    
    // load model
    interpreter = loadModelAndCreateInterpreter(MODEL_PATH);
    
    if (!interpreter) {
        std::cerr << "Failed to load model. Exiting." << std::endl;
        return -1;
    }

    std::cerr << "Model loaded succesfully." << std::endl;

    // Start threads
    std::thread t1(captureAndInferenceThread, MODEL_WIDTH, MODEL_HEIGHT);
    std::thread t2(ocrAndUploadThread);

    std::cout << "Pipeline running. Press Enter to stop." << std::endl;
    std::cin.get();

    keepRunning = false;
    
    t1.join();
    t2.join();
    std::cout << "Pipeline stopped successfully." << std::endl;
    return 0;
}