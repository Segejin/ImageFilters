#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <PNMreader.h>
#include <logging.h>
#include <stdio.h>

using namespace std;

PNMreader::PNMreader(char *fileName) {
    this->_fileName = fileName;
    name = "PNMreader";
}

void PNMreader::Update() {
    char msg[128];
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

void PNMreader::Execute() {
    ifstream file;
    file.open(this->_fileName, ios::in | ios::binary); 
	if (!file) {
        cout << "I don't think " << this->_fileName << " is a valid file." << endl;
        exit(EXIT_FAILURE);
    }
    file >> magicNum >> w >> h >> mV;
    image.SetSize(w, h);
    image.SetBuffer();
    while (file.peek() != EOF) {
        if (file.peek() != '\n') {
            file.read((char *)image.GetBuffer(), 3 * image.GetWidth() * image.GetHeight()); 
        } else { 
            file.ignore(100, '\n');
        }  
    }
    file.close();
}

const char* PNMreader::SourceName() {
    return name;
}
