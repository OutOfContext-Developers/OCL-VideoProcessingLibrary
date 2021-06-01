#include "frame.hpp"

Frame :: Frame() {

}

Frame :: ~Frame() {
    // Deallocate all memory
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);
    avcodec_free_context(&av_codec_ctx);
}

bool Frame :: open(const char* filename, enum AVMediaType media_type) {

    // Allocate context for container
    av_format_ctx = avformat_alloc_context();
    if (!av_format_ctx) {
        std::cerr << "Could not allocate AVFormatContext" << '\n';
        return false;
    }

    // Open file in context
    if (avformat_open_input(&av_format_ctx, filename, NULL, NULL) != 0) {
      std::cerr << "Couldn't open video file, missing or invalid format" << '\n';
      return false;
    }

    stream_index = -1;
    const AVCodec* av_codec;

    // Search for required stream
    for (int i = 0; i < av_format_ctx->nb_streams; i++) {
      av_codec_params = av_format_ctx->streams[i]->codecpar;
      av_codec = avcodec_find_decoder(av_codec_params->codec_id);

      if (!av_codec) {
        continue;
      }

      // If found the stream, break
      if (av_codec_params->codec_type == media_type) {
        stream_index = i;
        time_base = av_format_ctx->streams[i]->time_base;
        break;
      }
    }

    // Check if stream was founds
    if (stream_index == -1) {
      std::cerr << "Could not find valid stream inside file" << '\n';
      return false;
    }

    return true;
}
