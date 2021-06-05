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
  std::queue<std::pair<void*, int64_t>> ALL;     //! A queue that contains all frame in order with pts
  int MAX_Q_SIZE = 1;                            //! Maximum of number of frame you need to preprocess
protected:
  void* frame_buffer;                            //! Global variable to get raw rgba data of current frame
  std::queue<void*> all;                         //! Frames extracted from single packet are are put into this variable
  std::queue<int> allPts;                        //! Pts of all frames from single packets

  int pts;                                       //! Global variable to get pts of current frame

  bool LAST_FRAME = false;                       //! A boolean variable to keep track if stream has ended or not
  int64_t BUFFER_SIZE;                           //! Number of elements to be stored per frame
  size_t BITS_SIZE;                              //! Size of single element values that all, ALL are going to store


  //! Constructor for Syncronizer class
  Syncronizer();

  //! Destructor for Syncronizer class
  ~Syncronizer();

  //! Update the Frames queue to transfer data from all (temprory queue) to ALL (main queue)
  bool updateFramesQueue();

  //! Allocate memory to frame_buffer in which a single frame is extracted
  /*!
      /param num - Number of elements per frame (pixels, audio samples etc.)
      /param _bits - Number of bits required to store single element of the buffer
  */
  bool allocateFrameBuffer(int64_t num, size_t _bits);

  //! A function to put current frame in to temprory queue until it is moved to permanent one later
  void updateFrame() ;

public:
  //! A function which returns a single frame when requested
  //! This function return a pair of buffer and pts, where buffer contains frame data
  //! And pts contains Presentation time stamp for those frames
  std::pair<void*, int64_t> getFrame();

};

#endif
