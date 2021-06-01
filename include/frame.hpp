#ifndef frame_hpp
#define frame_hpp

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

#include <iostream>

//! Frame class
/*!
  This class reads a video file and tries to extract various streams for those
  containers into specified data structures.
*/

class Frame {

public:
    AVRational time_base;               //! A fractional represntation of number to describe time

protected:
    AVFormatContext* av_format_ctx;     //! Variable for AVFormatContext
    AVCodecContext* av_codec_ctx;       //! Variable for AVCodecContext
    int stream_index;                   //! Index at which stream is available
    AVFrame* av_frame;                  //! AVFrame
    AVPacket* av_packet;                //! AVPacket
    AVCodecParameters* av_codec_params; //! Parameters(information) of(about) the stream

public:
    //! Frame class contructor
    Frame();

    //! Frame class destructor
    ~Frame();

    //! A function to open file for stream input and initialize the index of stream
    /*!
      \param filename - Full path of file to be opened
      \param media_type - Type of media that needs to be extracted https://ffmpeg.org/doxygen/2.0/group__lavu__misc.html
    */
    bool open(const char* filename, enum AVMediaType media_type);

    //! A virtual function to read specified frame into the frame buffer
    /*!
      \param frame_buffer - Data array for individual frame
      \param pts - Presentation Time stamp of frame
    */
    virtual bool read_frame(uint8_t* frame_buffer, int64_t* pts);

    //! A virtual function to seek pointer to specific frame
    /*!
      \param int64_t ts - Seek time
    */
    virtual bool seek_frame(int64_t ts);
};



#endif
