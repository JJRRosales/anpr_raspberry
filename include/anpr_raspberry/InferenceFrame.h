#pragma once
#include <opencv2/opencv.hpp>

struct InferenceFrame {
    cv::Mat originalFrame;
    cv::Rect boundingBox;
    int classId;
    float confidence;
};