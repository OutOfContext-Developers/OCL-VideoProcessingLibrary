#include "aframe.hpp"

AudioFrame :: AudioFrame(const char* filename) {

    // Open the file
    open(filename, AVMEDIA_TYPE_AUDIO);

    // Get audio data
    getMetaData();

    // Reinitialize Maximum queue size to the sample rate of sound
    MAX_Q_SIZE = sampleRate;

    // Allocate memory to frame buffer
    allocateFrameBuffer(1, sizeof(int16_t));

    // Read first set of frames before the update function is called
    read_frame();
}

AudioFrame :: ~AudioFrame() {

}

bool AudioFrame :: getMetaData() {
    sampleRate = av_codec_ctx->sample_rate;
    channelCount = av_codec_ctx->channels;
    sampleSize = av_get_bytes_per_sample(av_codec_ctx->sample_fmt);
}

int16_t AudioFrame :: getSample(AVCodecContext* av_codec_ctx , uint8_t* buffer, int sampleIndex) {
    int64_t val = 0;
    float ret = 0;
    int sampleSize = av_get_bytes_per_sample(av_codec_ctx->sample_fmt);
    switch(sampleSize) {
        case 1:
            // 8bit samples are always unsigned
            val = REINTERPRET_CAST(uint8_t*, buffer)[sampleIndex];
            // make signed
            val -= 127;
            break;

        case 2:
            val = REINTERPRET_CAST(int16_t*, buffer)[sampleIndex];
            break;

        case 4:
            val = REINTERPRET_CAST(int32_t*, buffer)[sampleIndex];
            break;

        case 8:
            val = REINTERPRET_CAST(int64_t*, buffer)[sampleIndex];
            break;

        default:
            fprintf(stderr, "Invalid sample size %d.\n", sampleSize);
            return 0;
    }

    // Check which data type is in the sample.
    switch(av_codec_ctx->sample_fmt) {
        case AV_SAMPLE_FMT_U8:
        case AV_SAMPLE_FMT_S16:
        case AV_SAMPLE_FMT_S32:
        case AV_SAMPLE_FMT_U8P:
        case AV_SAMPLE_FMT_S16P:
        case AV_SAMPLE_FMT_S32P:
            // integer => Scale to [-1, 1] and convert to float.
            ret = val / STATIC_CAST(float, ((1 << (sampleSize*8-1))-1));
            break;

        case AV_SAMPLE_FMT_FLT:
        case AV_SAMPLE_FMT_FLTP:
            // float => reinterpret
            ret = *REINTERPRET_CAST(float*, &val);
            break;

        case AV_SAMPLE_FMT_DBL:
        case AV_SAMPLE_FMT_DBLP:
            // double => reinterpret and then static cast down
            ret = STATIC_CAST(float, *REINTERPRET_CAST(double*, &val));
            break;

        default:
            fprintf(stderr, "Invalid sample format %s.\n", av_get_sample_fmt_name(av_codec_ctx->sample_fmt));
            return 0;
    }

    // Convert float audio sample to unsigned int 16 bits
    ret = ret * 32678;
    if(ret>32767) ret = 32767;
    if(ret<-32768) ret = -32768;
    return (int16_t)ret;
}


bool AudioFrame :: processSingleFrame() {
  if(av_sample_fmt_is_planar(av_codec_ctx->sample_fmt) == 1) {
      // This means that the data of each channel is in its own buffer.
      // => frame->extended_data[i] contains data for the i-th channel.
      for(int s = 0; s < av_frame->nb_samples; ++s) {
          for(int c = 0; c < av_codec_ctx->channels; ++c) {
            *((int16_t*)frame_buffer) = getSample(av_codec_ctx, av_frame->extended_data[c], s);
              updateFrame();
          }
      }
  }
  else {
      // This means that the data of each channel is in the same buffer.
      // => av_frame->extended_data[0] contains data of all channels.
      if(RAW_OUT_ON_PLANAR) {
          uint8_t* arr = av_frame->extended_data[0];
          for(int i=0; i<av_frame->linesize[0]; i++) {
            *((uint8_t*)frame_buffer) = arr[i];
          }

      } else {
          for(int s = 0; s < av_frame->nb_samples; ++s) {
              for(int c = 0; c < av_codec_ctx->channels; ++c) {
                *((int16_t*)frame_buffer) = getSample(av_codec_ctx, av_frame->extended_data[0], s*av_codec_ctx->channels+c);
                  updateFrame();
              }
          }
      }
  }
}

void AudioFrame :: update() {

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
