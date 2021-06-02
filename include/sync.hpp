#ifndef sync_hpp
#define sync_hpp

extern "C" {
  #include <inttypes.h>
}

#include <iostream>
#include <utility>
#include <queue>
#include <vector>
#include <string.h>

class Syncronizer {
public:
  bool LAST_FRAME = false;                        //! A boolean variable to keep track if stream has ended or not
  std::queue<std::pair<uint8_t*, int64_t>> ALL;  //! A queue that contains all frame in order with pts
  int MAX_Q_SIZE = 100;                           //! Maximum of number of frame you need to preprocess
  uint8_t* frame_buffer;                          //! Global variable to get raw rgba data of current frame
protected:
  int pts;                                        //! Global variable to get pts of current frame
  uint8_t ALIGNMENT;
  int64_t BUFFER_SIZE;


  Syncronizer();
  ~Syncronizer();
  bool updateFramesQueue();
  bool allocateFrameBuffer(int64_t num);
public:
  std::pair<uint8_t*, int64_t> getFrame();

};

#endif
