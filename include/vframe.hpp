#ifndef vframe_hpp
#define vframe_hpp

#include "frame.hpp"


class VideoFrame : public Frame {
public:
  int width;                     //! Width of video
  int height;                    //! Height of video
private:
  SwsContext* sws_scaler_ctx;    //! SwsContext
public:

  VideoFrame(const char* filename);
  ~VideoFrame();
  bool getMetaData();
  static AVPixelFormat correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt);
  bool read_video_frame();
  void update();

};

#endif
