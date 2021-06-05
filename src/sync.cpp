#include "sync.hpp"

Syncronizer :: Syncronizer() {

}

Syncronizer :: ~Syncronizer() {
    // Free all memory
    free(frame_buffer);
}

void Syncronizer :: updateFrame() {

  // Allocate memory to the frame and copy it into the temprory queue
  void* temp_frame_buffer = malloc(BITS_SIZE*BUFFER_SIZE);
  memcpy(temp_frame_buffer, frame_buffer, BITS_SIZE*BUFFER_SIZE);

  // Push the value of pts and frame buffer
  all.push(temp_frame_buffer);
  allPts.push(pts);

}

bool Syncronizer :: updateFramesQueue() {

    // If queue is full or Last frame was already processed, return
    if(ALL.size()>=MAX_Q_SIZE || LAST_FRAME) {
      return false;
    }

    // Move all elements of temprory queue to main queue unless the queue is filled
    while(!all.empty() && ALL.size()<MAX_Q_SIZE) {

        ALL.push(std::make_pair(all.front(), allPts.front()));
        all.pop();
        allPts.pop();
    }

    return true;
}

std::pair<void*, int64_t> Syncronizer :: getFrame() {

    // If queue is empty return NULL values
    if(ALL.size()<=0) {
      return {NULL, -1};
    }

    // Else return appropriate element from the queue
    std::pair<void*, int64_t> currentElement = ALL.front();
    ALL.pop();

    return currentElement;

}

bool Syncronizer :: allocateFrameBuffer(int64_t num, size_t _bits) {
    BUFFER_SIZE = num;
    BITS_SIZE = _bits;
    frame_buffer = malloc(BITS_SIZE*BUFFER_SIZE);
}
