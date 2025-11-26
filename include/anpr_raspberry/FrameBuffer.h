#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>
#include "InferenceFrame.h"


class FrameBuffer {
    private:
        std::deque<InferenceFrame> buffer;
        std::mutex mtx;
        std::condition_variable condVar;
        size_t maxSize;
    
    public:
        FrameBuffer(size_t size) : maxSize(size) {}

        void pushFrame(const InferenceFrame& frame){
            std::unique_lock<std::mutex> lock(mtx);
            if(buffer.size() >= maxSize){
                buffer.pop_front();
                std::cout << "FrameBuffer full, dropping oldest frame."<<std::endl;
            }
            buffer.push_back(frame);
            lock.unlock();
            condVar.notify_one();
        }

        InferenceFrame popFrame(){
            std::unique_lock<std::mutex> lock(mtx);
            condVar.wait(lock, [this]{ return !buffer.empty(); });
            
            InferenceFrame frame = buffer.fromt();
            buffer.pop_front();
            return frame;
        }
}