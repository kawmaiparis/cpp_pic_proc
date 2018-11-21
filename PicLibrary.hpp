/* Attempt to implement the program concurrently doesn't work. Reverted back to sequential
 *
 * PicLibrary contains store, which is a map holding the filename and its associated Picture. The command line first
 * pre load the arguments called and then wait for the user to input command. If 'exit' is detected, the program
 * terminates. The interpreter tokenizes each command and passes it to the right process function which lies in
 * PicLibrary class.
 *
 *
 * Concurrent Attempt Explanation
 * PicLibrary contains store, which is a map holding the filename and its associated ThreadPic pair. ThreadPic holds
 * the Picture and the thread responsible for running any process for that picture. A thread is created every time
 * the command line interpreter detects an instruction to process a picture. The thread can be deleted in 2 ways: when
 * the picture is being unloaded from store, or when the command line interpreter receives 'exit' to terminate the
 * program.
 *
 * latest attempt of concurrency implementation is in commit hash: 25f688bb
 *
 * Blur optimisation methods are commented down below
 */

#ifndef PICLIB_H
#define PICLIB_H
#define MAXCOLOUR 255

#include "Picture.hpp"
#include "Utils.hpp"

class PicLibrary {

 private:
  // TODO: define internal picture storage
  map <string, Picture> store;
  Utils util;

 public:
  // default constructor/deconstructor
  PicLibrary(){};
  ~PicLibrary(){};

  // command-line interpreter routines
  void print_picturestore();
  void loadpicture(string path, string filename);
  void unloadpicture(string filename);
  void savepicture(string filename, string path);
  void display(string filename);

  // picture transformation routines
  void invert(string filename);
  void grayscale(string filename);
  void rotate(int angle, string filename);
  void flipVH(char plane, string filename);
  void blur(string filename);

  bool picExist(string filename);

  //static void blurColbyCol(Picture pic, Picture newPicture)
  //static void blurRowbyRow(Picture pic, Picture newPicture)
  static void blurColWithSectors(Picture pic, Picture newPicture, int col, int threadWidth);
  //static void blurRowWithSectors(Picture pic, Picture newPicture, int row, int threadHeight)
  //static void blurPixel(Picture pic, Picture newPicture)
};

#endif





