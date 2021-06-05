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

  //! VideoFrame class Constructor
  VideoFrame(const char* filename);

  //! VideoFrame class Destructor
  ~VideoFrame();

  //! Get data of video
  bool getMetaData();

  // Fix swscaler deprecated pixel format warning
  static AVPixelFormat correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt);

  //! Function to update frames queue regularly if empty
  void update();

  //! Overridden function from parent class to PostProcess the audio with the help of
  //! getSample function
  bool processSingleFrame();

};

#endif
