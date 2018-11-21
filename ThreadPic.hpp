// this doesn't work i have reverted back to sequential. File is not used.

/* PicLibrary contains store, which is a map holding the filename and its associated ThreadPic pair. ThreadPic holds
 * the Picture and the thread responsible for running any process for that picture. A thread is created every time
 * the command line interpreter detects an instruction to process a picture. The thread can be deleted in 2 ways: when
 * the picture is being unloaded from store, or when the command line interpreter receives 'exit' to terminate the
 * program.
 *
 * latest attempt of concurrency implementation is in commit hash: 25f688bb
 */

#ifndef CPP_PIC_PROC_PM3617_THREADPIC_HPP
#define CPP_PIC_PROC_PM3617_THREADPIC_HPP

#include "Picture.hpp"

#include <thread>
#include <iostream>
#include <mutex>
#include <queue>

using namespace std;

class ThreadPic {
private:
  Picture picture;

public:
  // thread responsible for each processing task for this picture.
  thread* t;

  ThreadPic();
  ~ThreadPic();

  ThreadPic(const Picture &picture) {
    this->picture = picture;
  }

  Picture getPicture() {
    return this->picture;
  }

  void setPicture(Picture pic) {
    this->picture = pic;
  }
};


#endif //CPP_PIC_PROC_PM3617_THREADPIC_HPP
