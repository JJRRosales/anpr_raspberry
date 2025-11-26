#pragma once
#include <opencv2/opencv.hpp>

struct InferenceFrame {
    cv::Mat image;
    cv::Rect boundingBox;
    int classId;
    float confidence;
}