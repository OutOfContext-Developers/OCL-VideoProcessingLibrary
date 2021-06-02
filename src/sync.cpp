#include "sync.hpp"

Syncronizer :: Syncronizer() {

}

Syncronizer :: ~Syncronizer() {

}


bool Syncronizer :: updateFramesQueue() {
    if(ALL.size()>=MAX_Q_SIZE || LAST_FRAME) {
      return false;
    }
    uint8_t* temp_frame_buffer = new uint8_t[BUFFER_SIZE];
    for(int i=0; i<BUFFER_SIZE; i++)  temp_frame_buffer[i] = frame_buffer[i];
    ALL.push(std::make_pair(temp_frame_buffer, pts));
    return true;
}

std::pair<uint8_t*, int64_t> Syncronizer :: getFrame() {
    if(ALL.size()<=0) {
      return {NULL, -1};
    }
    std::pair<uint8_t*, int64_t> temp = ALL.front();
    ALL.pop();

    return temp;

}

bool Syncronizer :: allocateFrameBuffer(int64_t num) {
    BUFFER_SIZE = num;
    frame_buffer = new uint8_t[BUFFER_SIZE];
}
