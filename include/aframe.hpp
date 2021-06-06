#ifndef aframe_hpp
#define aframe_hpp

#include "frame.hpp"

#define RAW_OUT_ON_PLANAR false

#ifdef __cplusplus
    #define REINTERPRET_CAST(type, variable) reinterpret_cast<type>(variable)
    #define STATIC_CAST(type, variable) static_cast<type>(variable)
#else
    #define C_CAST(type, variable) ((type)variable)
    #define REINTERPRET_CAST(type, variable) C_CAST(type, variable)
    #define STATIC_CAST(type, variable) C_CAST(type, variable)
#endif


class AudioFrame : public Frame {
public:
  unsigned int sampleRate;    //! Sample rate of audio
  unsigned int channelCount;  //! Channel count of audio stream
  int sampleSize;             //! Sample size
private:
  //! Fill required data variable from audio stream
  bool getMetaData();

public:

  //! Constructor for Audio Frame class
  /*!
      /param filename - Filename of the audio/video file
                        supported containers are same as of FFMPEG
  */
  AudioFrame(const char* filename);

  //! Destructor for Audio frame class
  ~AudioFrame();

  //! Function to update frames queue regularly if empty
  void update();

  //! A static function to convert sample values into int16_t type data structure
  /*!
      /param av_codec_ctx - AVCodecContext of the audio stream
      /param buffer       - uint8_t type Buffer array which contains sound data
      /param sampleIndex  - Index at which data is present
  */
  static int16_t getSample(AVCodecContext* av_codec_ctx, uint8_t* buffer, int sampleIndex);

  //! Overridden function from parent class to PostProcess the audio with the help of
  //! getSample function
  bool processSingleFrame();

  //! Overridden function to allow this class to send multiple frames instead of one
  std::pair<void*, int64_t> getFrame();

};

#endif
