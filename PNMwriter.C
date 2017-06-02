#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <PNMwriter.h>

using namespace std;

PNMwriter::PNMwriter() {
    name = "(PNMwriter)";
}

void PNMwriter::Write(char *fileName) {
    ofstream file;
    file.open(fileName, ios::out | ios::binary);
    file << "P6" << endl << image1->GetWidth() << " " << image1->GetHeight() << endl << 255 << endl;
    file.write((char *)image1->GetBuffer(), 3 * image1->GetHeight() * image1->GetWidth());
    file.close();
}

const char* PNMwriter::SinkName() {
    return name;
}
