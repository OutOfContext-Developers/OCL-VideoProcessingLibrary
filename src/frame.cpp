#include "frame.hpp"

Frame :: Frame() {
  // Allocate context for container
  av_format_ctx = avformat_alloc_context();
  if (!av_format_ctx) {
      std::cerr << "Could not allocate AVFormatContext" << '\n';
  }

}

Frame :: ~Frame() {
    // Deallocate all memory
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);
    avcodec_free_context(&av_codec_ctx);
}

// av_err2str returns a temporary array. This doesn't work in gcc.
// This function can be used as a replacement for av_err2str.
const char* Frame :: av_make_error(int errnum) {
    static char str[AV_ERROR_MAX_STRING_SIZE];
    memset(str, 0, sizeof(str));
    return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}


bool Frame :: open(const char* filename, enum AVMediaType media_type) {

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

    // Set up a codec context for the decoder
    av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx) {
        printf("Couldn't create AVCodecContext\n");
        return false;
    }
    if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0) {
        printf("Couldn't initialize AVCodecContext\n");
        return false;
    }
    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) {
        printf("Couldn't open codec\n");
        return false;
    }

    av_frame = av_frame_alloc();
    if (!av_frame) {
        printf("Couldn't allocate AVFrame\n");
        return false;
    }
    av_packet = av_packet_alloc();
    if (!av_packet) {
        printf("Couldn't allocate AVPacket\n");
        return false;
    }

    return true;
}

bool Frame :: read_frame() {

    // Decode one frame
    int response;
    while (av_read_frame(av_format_ctx, av_packet) >= 0) {
        if (av_packet->stream_index != stream_index) {
            av_packet_unref(av_packet);
            continue;
        }

        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0) {
            printf("Failed to decode packet: %s\n", av_make_error(response));
            return false;
        }

        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            av_packet_unref(av_packet);
            continue;
        } else if (response < 0) {
            printf("Failed to decode packet: %s\n", av_make_error(response));
            return false;
        }

        av_packet_unref(av_packet);
        break;
    }

    pts = av_frame->pts;
    return true;
}

bool Frame :: seek_frame(int64_t pts) {

    av_seek_frame(av_format_ctx, stream_index, pts, AVSEEK_FLAG_BACKWARD);

    // av_seek_frame takes effect after one frame, so I'm decoding one here
    // so that the next call to video_reader_read_frame() will give the correct
    // frame
    int response;
    while (av_read_frame(av_format_ctx, av_packet) >= 0) {
        if (av_packet->stream_index != stream_index) {
            av_packet_unref(av_packet);
            continue;
        }

        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0) {
            printf("Failed to decode packet: %s\n", av_make_error(response));
            return false;
        }

        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            av_packet_unref(av_packet);
            continue;
        } else if (response < 0) {
            printf("Failed to decode packet: %s\n", av_make_error(response));
            return false;
        }

        av_packet_unref(av_packet);
        break;
    }

    return true;
}

bool Frame :: updateFrames() {
  return updateFramesQueue();
}
