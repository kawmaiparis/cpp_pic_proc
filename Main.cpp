#include <iostream>
#include <thread>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

// TODO: Implement the picture library command-line interpreter

int main(int argc, char ** argv)
{
  PicLibrary piclib;

  for (int x = 1; x < argc; x++) {
	string filename;
	string filepath = argv[x];
	string::size_type slashPosi = filepath.find('/');
	while (slashPosi != string::npos) {
	  filename = filepath.substr(slashPosi + 1, filepath.size());
	  slashPosi = filename.find('/');
	}
	piclib.loadpicture(filepath, filename);
  }

  string cmd;
  while (getline(cin, cmd)) {
	stringstream input(cmd);
	string instr;
	input >> instr;
	if (instr == "liststore") {
	  piclib.print_picturestore();
	} else if (instr == "load") {
	  string filepath;
	  string filename;
	  input >> filepath >> filename;
	  piclib.loadpicture(filepath, filename);

	} else if (instr == "unload") {
	  string filename;
	  input >> filename;
	  piclib.unloadpicture(filename);

	} else if (instr == "save") {
	  string filename;
	  string filepath;
	  input >> filename >> filepath;
	  piclib.savepicture(filename, filepath);

	} else if (instr == "exit") {
	  break;

	} else if (instr == "display") {
	  string filename;
	  input >> filename;
	  piclib.display(filename);

	} else if (instr == "invert") {
	  string filename;
	  input >> filename;
	  piclib.invert(filename);

	} else if (instr == "grayscale") {
	  string filename;
	  input >> filename;
	  piclib.grayscale(filename);

	} else if (instr == "rotate") {
	  try {
		string angle;
		string filename;
		input >> angle >> filename;
		piclib.rotate(stoi(angle), filename);
	  } catch (exception &e) {
		cerr << "Invalid Angle. Only 90, 180, and 270 accepted" << endl;
	  }

	} else if (instr == "flip") {
	  try {
		char plane;
		string filename;
		input >> plane >> filename;
		piclib.flipVH(plane, filename);
	  } catch (exception &e) {
		cerr << "Invalid Plane. Only V and H accepted" << endl;
	  }


	} else if (instr == "blur") {
	  string filename;
	  input >> filename;
	  piclib.blur(filename);

	} else {
	  cerr << "Invalid input" << endl;
	}
  }

  return 0;

}

