#include "vframe.hpp"

VideoFrame :: VideoFrame(const char* filename) {
    // Open the file
    open(filename, AVMEDIA_TYPE_VIDEO);

    // Get data metadata
    getMetaData();

    // Allocate frame buffer with the number of pixels on screen
    allocateFrameBuffer(width*height*4, sizeof(uint8_t));

    // Pre read first frame
    read_frame();
}

VideoFrame :: ~VideoFrame() {
    sws_freeContext(sws_scaler_ctx);
}

bool VideoFrame :: getMetaData() {
    width = av_codec_params->width;
    height = av_codec_params->height;
}

AVPixelFormat VideoFrame :: correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt) {
    // Fix swscaler deprecated pixel format warning
    // (YUVJ has been deprecated, change pixel format to regular YUV)
    switch (pix_fmt) {
        case AV_PIX_FMT_YUVJ420P: return AV_PIX_FMT_YUV420P;
        case AV_PIX_FMT_YUVJ422P: return AV_PIX_FMT_YUV422P;
        case AV_PIX_FMT_YUVJ444P: return AV_PIX_FMT_YUV444P;
        case AV_PIX_FMT_YUVJ440P: return AV_PIX_FMT_YUV440P;
        default:                  return pix_fmt;
    }
}

bool VideoFrame :: processSingleFrame() {
    // Set up sws scaler
    if (!sws_scaler_ctx) {
        auto source_pix_fmt = correct_for_deprecated_pixel_format(av_codec_ctx->pix_fmt);
        sws_scaler_ctx = sws_getContext(width, height, source_pix_fmt,
                                        width, height, AV_PIX_FMT_RGB0,
                                        SWS_BILINEAR, NULL, NULL, NULL);
    }
    if (!sws_scaler_ctx) {
        printf("Couldn't initialize sw scaler\n");
        return false;
    }

    uint8_t* dest[4] = { (uint8_t*)frame_buffer, NULL, NULL, NULL };
    int dest_linesize[4] = { width * 4, 0, 0, 0 };
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);

    updateFrame();
    return true;
}

void VideoFrame :: update() {

    // If this is last frame then nothing to do
    if(LAST_FRAME)
      return;

    // While the queue is not full, keep filling it
    while(updateFramesQueue()) {
      if(!read_frame()) {
        LAST_FRAME = true;
      }
    }
}
