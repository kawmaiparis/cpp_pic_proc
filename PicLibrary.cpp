#include "PicLibrary.hpp"
#include "Colour.hpp"
#include "Utils.hpp"
#include <thread>

int threadCounts = thread::hardware_concurrency();

// TODO: implement the PicLibrary class
void PicLibrary::print_picturestore() {
  for (auto &itr : store) {
	cout << itr.first << endl;
  }
}

void PicLibrary::loadpicture(string path, string filename) {
  if (!picExist(filename)) {
	Picture pic = Picture(path);
	if (!pic.getimage().empty()) {
	  store.insert(pair<string, Picture>(filename, pic));
	} else {
	  cerr << "image doesn't exist" << endl;
	}
  } else {
	cerr << "filename already existed" << endl;
  }
}

void PicLibrary::unloadpicture(string filename) {
  if (picExist(filename)) {
	store.erase(filename);

  } else {
	cerr << "no matching filename in store" << endl;
  }
}

void PicLibrary::savepicture(string filename, string path) {
  if (picExist(filename)) {
	Picture pic = store.at(filename);
	util.saveimage(pic.getimage(), path);

  } else {
	cerr << "no matching filename in store" << endl;
  }
}

void PicLibrary::display(string filename) {
  if (picExist(filename)) {
	Picture pic = store.at(filename);
	util.displayimage(pic.getimage());

  } else {
	cerr << "no matching filename in store" << endl;
  }
}

void PicLibrary::invert(string filename) {
  if (picExist(filename)) {
	Picture pic = store.at(filename);
	for (int x = 0; x < pic.getwidth(); x++) {
	  for (int y = 0; y < pic.getheight(); y++) {
		Colour c = pic.getpixel(x, y);
		c.setred(MAXCOLOUR - c.getred());
		c.setgreen(MAXCOLOUR - c.getgreen());
		c.setblue(MAXCOLOUR - c.getblue());
		pic.setpixel(x, y, c);
	  }
	}

  } else {
	cerr << "no matching filename in store" << endl;
  }

}

void PicLibrary::grayscale(string filename) {
  if (picExist(filename)) {
	Picture pic = store.at(filename);
	for (int x = 0; x < pic.getwidth(); x++) {
	  for (int y = 0; y < pic.getheight(); y++) {
		Colour c = pic.getpixel(x, y);
		int average = (c.getred() + c.getblue() + c.getgreen()) / 3;
		c.setred(average);
		c.setgreen(average);
		c.setblue(average);
		pic.setpixel(x, y, c);
	  }
	}

  } else {
	cerr << "no matching filename in store" << endl;
  }
}

//create a second picture to overwrite the original
void PicLibrary::rotate(int angle, string filename) {

  if (picExist(filename)) {
	Picture pic = store.at(filename);
	int width = pic.getwidth();
	int height = pic.getheight();

	if (angle == 90) {
	  Picture newPicture = Picture(height, width);
	  for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
		  Colour c = pic.getpixel(x, y);
		  newPicture.setpixel(height - y - 1, x , c);
		}
	  }
	  store.at(filename) = newPicture;

	} else if (angle == 180) {
	  Picture newPicture = Picture(width, height);
	  for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
		  Colour c = pic.getpixel(x, y);
		  newPicture.setpixel(width - x - 1, height - y - 1, c);
		}
	  }
	  store.at(filename) = newPicture;

	} else if (angle == 270) {
	  Picture newPicture = Picture(height, width);
	  for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
		  Colour c = pic.getpixel(x, y);
		  newPicture.setpixel(y, width - x - 1, c);
		}
	  }
	  store.at(filename) = newPicture;
	}

  } else {
	cerr << "no matching filename in store" << endl;
  }
}

void PicLibrary::flipVH(char plane, string filename) {
  if (picExist(filename)) {
	Picture pic = store.at(filename);
	int width = pic.getwidth();
	int height = pic.getheight();
	Picture newPicture = Picture(width, height);

	if (plane == 'H') {
	  for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
		  Colour c = pic.getpixel(x, y);
		  newPicture.setpixel(width - x - 1, y, c);
		}
	  }
	  store.at(filename) = newPicture;


	} else if (plane == 'V') {
	  for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
		  Colour c = pic.getpixel(x, y);
		  newPicture.setpixel(x, height - y - 1, c);
		}
	  }
	  store.at(filename) = newPicture;

	} else {
	  cerr << "H or V only please" << endl;
	}

  } else {
	cerr << "no matching filename in store" << endl;
  }

}

void PicLibrary::blurColWithSectors(Picture pic, Picture newPicture, int col, int threadWidth) {
  int height = pic.getheight();
  int width = pic.getwidth();
  for (int x = col; x < col + threadWidth; x++) {
	for (int y = 0; y < height; y++) {
	  if (y == 0 || y == height- 1 || x == 0 || x == width - 1) {
		Colour c = pic.getpixel(x, y);
		newPicture.setpixel(x, y, c);
	  } else {
		int Red = 0;
		int Green = 0;
		int Blue = 0;
		for (int hori = x - 1; hori < x + 2; hori++) {
		  for (int verti = y - 1; verti < y + 2; verti++) {

			Colour temp = pic.getpixel(hori, verti);
			Red += temp.getred();
			Green += temp.getgreen();
			Blue += temp.getblue();
		  }
		}
		Red = Red / 9;
		Green = Green / 9;
		Blue = Blue / 9;
		Colour c = Colour(Red, Green, Blue);
		newPicture.setpixel(x, y, c);
	  }
	}
  }
}

// SECTOR BY COLUMN METHOD - FASTEST
void PicLibrary::blur(string filename) {
  if (picExist(filename)) {
	Picture pic = store.at(filename);
	int width = pic.getwidth();
	int height = pic.getheight();
	Picture newPicture = Picture(width, height);

	vector <thread*> threads;
	int widthPerThread = width / threadCounts;

	for (int startCol = 0; startCol < threadCounts; startCol++) {
	  thread* t = new thread(PicLibrary::blurColWithSectors, pic, newPicture, startCol*widthPerThread, widthPerThread);
	  threads.push_back(t);
	}
	for (auto &itr : threads) {
	  itr->join();
	  delete(itr);
	}
	store.at(filename) = newPicture;
  } else {
	cerr << "no matching filename in store" << endl;
  }
}



// check if the input filename exists in store
bool PicLibrary::picExist(string filename) {
  return !(store.find(filename) == store.end());
}



// -------------------------- BLUR OPTIMISATION EXPERIMENT --------------------------------------


// ---------------------------- SEQUENTIAL METHOD ---------------------------------------------
/*
 * //void PicLibrary::blur(string filename) {
//  if (picExist(filename)) {
//	Picture pic = store.at(filename);
//	int width = pic.getwidth();
//	int height = pic.getheight();
//	Picture newPicture = Picture(width, height);
//
//	for (int x = 0; x < width; x++) {
//	  for (int y = 0; y < height; y++) {
//		if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
//		  Colour c = pic.getpixel(x, y);
//		  newPicture.setpixel(x, y, c);
//		} else {
//		  int Red = 0;
//		  int Green = 0;
//		  int Blue = 0;
//		  for (int hori = x -1; hori < x + 2; hori++) {
//			for (int verti = y -1; verti < y + 2; verti++) {
//			  Colour temp = pic.getpixel(hori, verti);
//			  Red += temp.getred();
//			  Green += temp.getgreen();
//			  Blue += temp.getblue();
//			}
//		  }
//		  Red = Red / 9;
//		  Green = Green / 9;
//		  Blue = Blue / 9;
//		  Colour c = Colour(Red, Green, Blue);
//		  newPicture.setpixel(x, y, c);
//		}
//	  }
//	}
//
//	store.at(filename) = newPicture;
//
//  } else {
//	cerr << "no matching filename in store" << endl;
//  }
 */


// -------------------------- COLUMN BY COLUMN METHOD -------------------------------------------
/*
void PicLibrary::blurColbyCol(Picture pic, Picture newPicture) {
	int height = pic.getheight();
	int width = pic.getwidth();
	while (true) {
		m1.lock();
		int x = currentCol;
		if (currentCol >= width) {
			m1.unlock();
			break;
		}
		currentCol++;
		m1.unlock();

		for (int y = 0; y < height; y++) {
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				Colour c = pic.getpixel(x, y);
				newPicture.setpixel(x, y, c);
			} else {
				int Red = 0;
				int Green = 0;
				int Blue = 0;
				for (int hori = x - 1; hori < x + 2; hori++) {
					for (int verti = y - 1; verti < y + 2; verti++) {

						Colour temp = pic.getpixel(hori, verti);
						Red += temp.getred();
						Green += temp.getgreen();
						Blue += temp.getblue();
					}
				}
				Red = Red / 9;
				Green = Green / 9;
				Blue = Blue / 9;
				Colour c = Colour(Red, Green, Blue);
				newPicture.setpixel(x, y, c);
			}
		}
	}
}

void PicLibrary::blur(string filename) {
	if (picExist(filename)) {
		Picture pic = store.at(filename);
		int width = pic.getwidth();
		int height = pic.getheight();
		Picture newPicture = Picture(width, height);
		currentCol = 0;

		vector <thread*> threads;
		for (int i = 0; i < threadCounts; i++) {
			auto* t = new thread(PicLibrary::blurColbyCol, pic, newPicture);
			threads.push_back(t);
		}
		for (auto &itr : threads) {
			itr->join();
			delete(itr);
		}
		store.at(filename) = newPicture;
	} else {
		cerr << "no matching filename in store" << endl;
	}

}
*/

// ------------------------------------ ROW BY ROW -------------------------------------------------
/*
void PicLibrary::blurRowbyRow(Picture pic, Picture newPicture) {
	int height = pic.getheight();
	int width = pic.getwidth();
	while (true) {
		m1.lock();
		int y = currentRow;
		if (currentRow >= height) {
			m1.unlock();
			break;
		}
		currentRow++;
		m1.unlock();

		for (int x = 0; x < width; x++) {
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				Colour c = pic.getpixel(x, y);
				newPicture.setpixel(x, y, c);
			} else {
				int Red = 0;
				int Green = 0;
				int Blue = 0;
				for (int hori = x - 1; hori < x + 2; hori++) {
					for (int verti = y - 1; verti < y + 2; verti++) {

						Colour temp = pic.getpixel(hori, verti);
						Red += temp.getred();
						Green += temp.getgreen();
						Blue += temp.getblue();
					}
				}
				Red = Red / 9;
				Green = Green / 9;
				Blue = Blue / 9;
				Colour c = Colour(Red, Green, Blue);
				newPicture.setpixel(x, y, c);
			}
		}
	}
}

void PicLibrary::blur(string filename) {
	if (picExist(filename)) {
		Picture pic = store.at(filename);
		int width = pic.getwidth();
		int height = pic.getheight();
		Picture newPicture = Picture(width, height);
		currentRow = 0;

		vector <thread*> threads;
		for (int i = 0; i < threadCounts; i++) {
			auto* t = new thread(PicLibrary::blurRowbyRow, pic, newPicture);
			threads.push_back(t);
		}
		for (auto &itr : threads) {
			itr->join();
			delete(itr);
		}
		store.at(filename) = newPicture;
	} else {
		cerr << "no matching filename in store" << endl;
	}
}
 */

// --------------------------------------- SECTOR BY COLUMN METHOD --------------------------------------
/*
void PicLibrary::blurColWithSectors(Picture pic, Picture newPicture, int col, int threadWidth) {
	int height = pic.getheight();
	int width = pic.getwidth();
	for (int x = col; x < col + threadWidth; x++) {
		for (int y = 0; y < height; y++) {
			if (y == 0 || y == height- 1 || x == 0 || x == width - 1) {
				Colour c = pic.getpixel(x, y);
				newPicture.setpixel(x, y, c);
			} else {
				int Red = 0;
				int Green = 0;
				int Blue = 0;
				for (int hori = x - 1; hori < x + 2; hori++) {
					for (int verti = y - 1; verti < y + 2; verti++) {

						Colour temp = pic.getpixel(hori, verti);
						Red += temp.getred();
						Green += temp.getgreen();
						Blue += temp.getblue();
					}
				}
				Red = Red / 9;
				Green = Green / 9;
				Blue = Blue / 9;
				Colour c = Colour(Red, Green, Blue);
				newPicture.setpixel(x, y, c);
			}
		}
	}
}

void PicLibrary::blur(string filename) {
	if (picExist(filename)) {
		Picture pic = store.at(filename);
		int width = pic.getwidth();
		int height = pic.getheight();
		Picture newPicture = Picture(width, height);

		vector <thread*> threads;
		int widthPerThread = width / threadCounts;

		for (int startCol = 0; startCol < threadCounts; startCol++) {
			thread* t = new thread(PicLibrary::blurColWithSectors, pic, newPicture, startCol*widthPerThread, widthPerThread);
			threads.push_back(t);
		}
		for (auto &itr : threads) {
			itr->join();
			delete(itr);
		}
		store.at(filename) = newPicture;
	} else {
		cerr << "no matching filename in store" << endl;
	}
}
 */

// -------------------------------------- SECTOR BY ROW METHOD ----------------------------------------
/*
void PicLibrary::blurRowWithSectors(Picture pic, Picture newPicture, int row, int threadHeight) {
	int height = pic.getheight();
	int width = pic.getwidth();
	for (int y = row; y < row + threadHeight; y++) {
		for (int x = 0; x < width; x++) {
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				Colour c = pic.getpixel(x, y);
				newPicture.setpixel(x, y, c);
			} else {
				int Red = 0;
				int Green = 0;
				int Blue = 0;
				for (int hori = x - 1; hori < x + 2; hori++) {
					for (int verti = y - 1; verti < y + 2; verti++) {

						Colour temp = pic.getpixel(hori, verti);
						Red += temp.getred();
						Green += temp.getgreen();
						Blue += temp.getblue();
					}
				}
				Red = Red / 9;
				Green = Green / 9;
				Blue = Blue / 9;
				Colour c = Colour(Red, Green, Blue);
				newPicture.setpixel(x, y, c);
			}
		}
	}
}

void PicLibrary::blur(string filename) {
	if (picExist(filename)) {
		Picture pic = store.at(filename);
		int width = pic.getwidth();
		int height = pic.getheight();
		Picture newPicture = Picture(width, height);

		vector <thread*> threads;
		int heightPerThread = height / threadCounts;

		for (int startRow = 0; startRow < threadCounts; startRow++) {
			thread* t = new thread(PicLibrary::blurRowWithSectors, pic, newPicture, startRow*heightPerThread, heightPerThread);
			threads.push_back(t);
		}

		for (auto &itr : threads) {
			itr->join();
			delete(itr);
		}
		store.at(filename) = newPicture;

	} else {
		cerr << "no matching filename in store" << endl;
	}
}
*/

// -------------------------------------- PIXEL BY PIXEL METHOD ------------------------------------------------
/*
void PicLibrary::blurPixel(Picture pic, Picture newPicture) {
	int height = pic.getheight();
	int width = pic.getwidth();
	while (true) {
		m1.lock();
		int x = currentPixelCol;
		int y = currentPixelRow;

		if (currentPixelCol >= width) {
			m1.unlock();
			break;
		} else if (currentPixelRow + 1 > height - 1) {
			currentPixelCol++;
			currentPixelRow = 0;
		} else {
			currentPixelRow++;
		}
		m1.unlock();

		if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
			Colour c = pic.getpixel(x, y);
			newPicture.setpixel(x, y, c);
		} else {
			int Red = 0;
			int Green = 0;
			int Blue = 0;
			for (int hori = x - 1; hori < x + 2; hori++) {
				for (int verti = y - 1; verti < y + 2; verti++) {
					Colour temp = pic.getpixel(hori, verti);
					Red += temp.getred();
					Green += temp.getgreen();
					Blue += temp.getblue();
				}
			}
			Red = Red / 9;
			Green = Green / 9;
			Blue = Blue / 9;
			Colour c = Colour(Red, Green, Blue);
			newPicture.setpixel(x, y, c);
		}
	}
}

void PicLibrary::blur(string filename) {
	if (picExist(filename)) {
		Picture pic = store.at(filename);
		int width = pic.getwidth();
		int height = pic.getheight();
		Picture newPicture = Picture(width, height);
		currentPixelCol = 0;
		currentPixelRow = 0;

		vector <thread*> threads;
		for (int i = 0; i < threadCounts; i++) {
			auto *t = new thread(PicLibrary::blurPixel, pic, newPicture);
			threads.push_back(t);
		}

		for (auto &itr : threads) {
			itr->join();
			delete(itr);
		}
		store.at(filename) = newPicture;
	} else {
		cerr << "no matching filename in store" << endl;
	}
}
*/
