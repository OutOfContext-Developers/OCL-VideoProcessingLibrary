#include "vframe.hpp"

VideoFrame :: VideoFrame(const char* filename) {
    open(filename, AVMEDIA_TYPE_VIDEO);
}

VideoFrame :: ~VideoFrame() {
    sws_freeContext(sws_scaler_ctx);
}

bool VideoFrame :: getMetaData() {
    width = av_codec_params->width;
    height = av_codec_params->height;
}

bool VideoFrame :: read_frame(uint8_t* frame_buffer, int64_t* pts) {

}
