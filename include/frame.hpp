#ifndef frame_hpp
#define frame_hpp

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include "sync.hpp"

//! Frame class
/*!
  This class reads a video file and tries to extract various streams for those
  containers into specified data structures.
*/

class Frame : public Syncronizer {

public:
    AVRational time_base;               //! A fractional represntation of number to describe time

protected:
    AVFormatContext* av_format_ctx;     //! Variable for AVFormatContext
    AVCodecContext* av_codec_ctx;       //! Variable for AVCodecContext
    int stream_index = -1;              //! Index at which stream is available
    AVFrame* av_frame;                  //! AVFrame
    AVPacket* av_packet;                //! AVPacket
    AVCodecParameters* av_codec_params; //! Parameters(information) of(about) the stream

public:
    //! Frame class contructor
    Frame();

    //! Frame class destructor
    ~Frame();

    static const char* av_make_error(int errnum);

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
    bool read_frame();

    //! A Function to seek pointer to specific frame
    /*!
      \param ts - Seek time
    */
    bool seek_frame(int64_t ts);

    //! A pure virtual function for post processing of single frame ( which can be audio/ video anything)
    //! To be processed in child class inheriting this
    virtual bool processSingleFrame()  = 0;

};



#endif
