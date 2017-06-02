#include <sink.h>
#include <source.h>
#include <filters.h>
#include <image.h>
#include <logging.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

void Filter::Update() {
    if (image1 != NULL) {
        char msg[128];
        sprintf(msg, "%s: about to update input1", SourceName());
        Logger::LogEvent(msg);
        image1->Update();
        sprintf(msg, "%s: done updating input1", SourceName());
        Logger::LogEvent(msg);
    }
    if (image2 != NULL) {
        char msg[128];
        sprintf(msg, "%s: about to update input2", SourceName());
        Logger::LogEvent(msg);
        image2->Update();
        sprintf(msg, "%s: done updating input2", SourceName());
        Logger::LogEvent(msg);
    }
    char msg[128];
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

const char *Filter::SourceName() {
    return FilterName();
}

const char *Filter::SinkName() {
    return FilterName();
}

Shrinker::Shrinker() {
    name = "Shrinker";
}

void Shrinker::Execute() {
    CheckInputOne();
    image.SetSize((0.5 * image1->GetWidth()), (0.5 * image1->GetHeight()));
    image.SetBuffer();
    for (int i = 0; i < image.GetHeight(); ++i) {
        for (int j = 0; j < image.GetWidth(); ++j) {
            image.GetBuffer()[i * image.GetWidth() + j].red = image1->GetBuffer()[2 * (i * image1->GetWidth() + j)].red;
            image.GetBuffer()[i * image.GetWidth() + j].green = image1->GetBuffer()[2 * (i * image1->GetWidth() + j)].green;
            image.GetBuffer()[i * image.GetWidth() + j].blue = image1->GetBuffer()[2 * (i * image1->GetWidth() + j)].blue;
        }
    }
}

const char *Shrinker::FilterName() {
    return name;
}

LRConcat::LRConcat() {
    name = "LRConcat";
}

void LRConcat::Execute() {
    CheckInputOne();
    CheckInputTwo();
    if (image1->GetHeight() != image2->GetHeight()) {
        char msg[1024];
        sprintf(msg, "heights must match: %d, %d", image1->GetHeight(), image2->GetHeight());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    int width = image1->GetWidth() + image2->GetWidth();
    int height = image1->GetHeight();
    int w1 = image1->GetWidth();
    int w2 = image2->GetWidth();
    image.SetSize(width, height);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    Pixel *buffer1 = image1->GetBuffer();
    Pixel *buffer2 = image2->GetBuffer();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < w1; ++j) {
            int idx = i * width + j;
            int idx1 = i * w1 + j;
            buffer[idx].red = buffer1[idx1].red;
            buffer[idx].green = buffer1[idx1].green;
            buffer[idx].blue = buffer1[idx1].blue;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = w1; j < width; ++j) { 
            int idx = i * width + j;
            int idx2 = i * w2 + j - w1;
            buffer[idx].red = buffer2[idx2].red;
            buffer[idx].green = buffer2[idx2].green;
            buffer[idx].blue = buffer2[idx2].blue;
        }    
    }
}

const char *LRConcat::FilterName() {
    return name;
}

TBConcat::TBConcat() {
    name = "TBConcat";
}

void TBConcat::Execute() {
    CheckInputOne();
    CheckInputTwo();
    const int w1 = image1->GetWidth();
    const int w2 = image2->GetWidth();
    if (w1 != w2) {
        char msg[1024];
        sprintf(msg, "widths must match: %d, %d", w1, w2);
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    const int h1 = image1->GetHeight();
    const int h2 = image2->GetHeight();
    const int width = w1;
    const int height = h1 + h2;
    image.SetSize(width, height);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    const  Pixel *buffer1 = image1->GetBuffer();
    const Pixel *buffer2 = image2->GetBuffer();
    for (int i = 0; i < h1; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = i * width + j;
            int idx1 = i * w1 + j;
            buffer[idx].red = buffer1[idx1].red;
            buffer[idx].green = buffer1[idx1].green;
            buffer[idx].blue = buffer1[idx1].blue;
        }
    }
    for (int i = h1; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = i * width + j;
            int idx2 = (i - h1) * w2 + j;
            buffer[idx].red = buffer2[idx2].red;
            buffer[idx].green = buffer2[idx2].green;
            buffer[idx].blue = buffer2[idx2].blue; 
        }
    }
}

const char *TBConcat::FilterName() {
    return name;
}

Blender::Blender() {
    name = "Blender";
}

void Blender::SetFactor(float factor) {
    this->blendFactor = factor;
}

void Blender::Execute() {
    CheckInputOne();
    CheckInputTwo();
    if (blendFactor > 1 || blendFactor < 0) {
        char msg[1024];
        sprintf(msg, "Invalid factor for Blender: %f", blendFactor);
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    image.SetSize(image1->GetWidth(), image1->GetHeight());
    image.SetBuffer();
    for (int i = 0; i < image.GetHeight(); ++i) {
        for (int j = 0; j < image.GetWidth(); ++j) {
            image.GetBuffer()[i * image.GetWidth() + j].red = blendFactor * image1->GetBuffer()[i * image.GetWidth() + j].red
                              + image2->GetBuffer()[i * image.GetWidth() + j].red * (1.0 - blendFactor);
            image.GetBuffer()[i * image.GetWidth() + j].green = blendFactor * image1->GetBuffer()[i * image.GetWidth() + j].green
                              + image2->GetBuffer()[i * image.GetWidth() + j].green * (1.0 - blendFactor);
            image.GetBuffer()[i * image.GetWidth() + j].blue = blendFactor * image1->GetBuffer()[i * image.GetWidth() + j].blue
                              + image2->GetBuffer()[i * image.GetWidth() + j].blue * (1.0 - blendFactor);
        }
    }   

}

const char *Blender::FilterName() {
    return name;
}

Crop::Crop() {
    name = "Crop";
    Istart = Istop = Jstart = Jstop = -1;
}

void Crop::SetRegion(int Istart_, int Istop_, int Jstart_, int Jstop_) {
    Istart = Istart_;
    Istop = Istop_;
    Jstart = Jstart_;
    Jstop = Jstop_;
}

void Crop::Execute() {
    CheckInputOne();
    const int inputWidth = image1->GetWidth();
    const int inputHeight = image1->GetHeight();
    if (Istart < 0 || Istop < 0 || Jstart < 0 || Jstop < 0) {
        char msg[1024];
        sprintf(msg, "%s: uninitialized region", FilterName());
        DataFlowException e(FilterName(), msg);
        throw e;
    }
    if (Istart >= inputWidth || Istop >= inputWidth) {
        char msg[1024];
        sprintf(msg, "%s: region outside image width", FilterName());
        DataFlowException e(FilterName(), msg);
        throw e;
    }
    if (Jstart >= inputHeight || Jstop >= inputHeight) {
        char msg[1024];
        sprintf(msg, "%s: region outside image height", FilterName());
        DataFlowException e(FilterName(), msg);
        throw e;
    }
    if (Istop < Istart || Jstop < Jstart) {
        char msg[1024];
        sprintf(msg, "%s: invalid region", FilterName());
        DataFlowException e(FilterName(), msg);
        throw e;
    }
    int w = Istop-Istart+1;
    int h = Jstop-Jstart+1;
    image.SetSize(w, h);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    const Pixel *buffer1 = image1->GetBuffer();
    for (int i = Istart; i <= Istop; ++i) {
        for (int j = Jstart; j <= Jstop; ++j) {
            int idx = (j-Jstart) * w + (i - Istart);
            int idx1 = j * inputWidth + i;
            buffer[idx].red = buffer1[idx1].red;
            buffer[idx].green = buffer1[idx1].green;
            buffer[idx].blue = buffer1[idx1].blue;
        }
    }
}

const char *Crop::FilterName() {
    return name;
}

Transpose::Transpose() {
    name = "Transpose";
}

void Transpose::Execute() {
    CheckInputOne();
    const int inputWidth = image1->GetWidth();
    const int inputHeight = image1->GetHeight();
    image.SetSize(inputHeight, inputWidth);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    const Pixel *buffer1 = image1->GetBuffer();
    for (int i = 0; i < inputWidth; ++i) {
        for (int j = 0; j < inputHeight; ++j) {
            int idx = i * inputHeight + j;
            int idx1 = j * inputWidth + i;
            buffer[idx].red = buffer1[idx1].red;
            buffer[idx].green = buffer1[idx1].green;
            buffer[idx].blue = buffer1[idx1].blue;
        }   
    }
}

const char *Transpose::FilterName() {
    return name;
}

Invert::Invert() {
    name = "Invert";
}

void Invert::Execute() {
    CheckInputOne();
    const int inputWidth = image1->GetWidth();
    const int inputHeight = image1->GetHeight();
    image.SetSize(inputWidth, inputHeight);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    const Pixel *buffer1 = image1->GetBuffer();
    for (int i = 0; i < inputHeight; ++i) {
        for (int j = 0; j < inputWidth; ++j) {
            int idx = i * inputWidth + j;
            int idx1 = i * inputWidth + j;
            buffer[idx].red = 255 - buffer1[idx1].red;
            buffer[idx].green = 255 - buffer1[idx1].green;
            buffer[idx].blue = 255 - buffer1[idx1].blue;
        }   
    }
}

const char *Invert::FilterName() {
    return name;
}

Checkerboard::Checkerboard() {
    name = "Checkerboard";
}

void Checkerboard::Execute() {
    CheckInputOne();
    CheckInputTwo();
    const int w1 = image1->GetWidth();
    const int h1 = image1->GetHeight();
    const int w2 = image2->GetWidth();
    const int h2 = image2->GetHeight(); 
    if (w1 != w2 || h1 != h2) {
        char msg[1024];
        sprintf(msg, "image sizes must be identical: (%d, %d) (%d, %d)", w1, h1, w2, h2);
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    image.SetSize(w1, h1);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    const Pixel *buffer1 = image1->GetBuffer();
    const Pixel *buffer2 = image2->GetBuffer();
    for (int i = 0; i < h1; ++i) {
        for (int j = 0; j < w1; ++j) {
            int idx = i * w1 + j;
            if ((i/10 + j/10) % 2 == 0) {
                int idx1 = i * w1 + j;
                buffer[idx].red = buffer1[idx1].red;
                buffer[idx].green = buffer1[idx1].green;
                buffer[idx].blue = buffer1[idx1].blue;
            } else { 
                int idx2 = i * w2 + j;
                buffer[idx].red = buffer2[idx2].red;
                buffer[idx].green = buffer2[idx2].green;
                buffer[idx].blue = buffer2[idx2].blue;
            }
        }   
    }
}

const char *Checkerboard::FilterName() {
    return name;
}

Color::Color(int w, int h, unsigned char r, unsigned char g, unsigned char b) {
    red = r;
    green = g;
    blue = b;
    name = "Color";
    image.SetSize(w, h);
    image.SetBuffer();
    Pixel *buffer = image.GetBuffer();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int idx = i * w + j;
            buffer[idx].red = red;
            buffer[idx].green = green;
            buffer[idx].blue = blue;
        }
    }
}

void Color::Execute() {}

const char *Color::SourceName() {
    return name;
}

CheckSum::CheckSum() {
    red = green = blue = 0;
    name = "CheckSum";
}

void CheckSum::OutputCheckSum(const char *fileName) {
    CheckInputOne();
    const int inputWidth = image1->GetWidth();
    const int inputHeight = image1->GetHeight();
    const Pixel *buffer = image1->GetBuffer();
    for (int i = 0; i < inputHeight; ++i) {
        for (int j = 0; j < inputWidth; ++j) {
            int idx = i * inputWidth + j;
            red += buffer[idx].red;
            green += buffer[idx].green;
            blue += buffer[idx].blue;
        }
    }
    file = fopen(fileName, "w");
    fprintf(file, "CHECKSUM: %u, %u, %u\n", red, green, blue);
    fclose(file);
}

const char *CheckSum::SinkName() {
    return name;
}
