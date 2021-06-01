#ifndef vframe_hpp
#define vframe_hpp

#include "frame.hpp"
#include <queue>
#include <utility>


class VideoFrame :public Frame {
public:
  int width;                  //! Width of video
  int height;                 //! Height of video
  std::queue<std::pair<uint8_t*, int64_t>> frames;
private:
  SwsContext* sws_scaler_ctx;        //! SwsContext
public:

VideoFrame(const char* filename);
~VideoFrame();
bool getMetaData();
bool read_frame(uint8_t* frame_buffer, int64_t* pts);
bool seek_frame(int64_t ts);

};

#endif
