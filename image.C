#include <image.h>
#include <source.h>
#include <string.h>

Image::Image() {
    _source = NULL;
    width = 0;
    height = 0;
    buffer = NULL;
    isUpdated = false;
}

Image::Image(int w, int h) {
    width = w;
    height = h;
    buffer = new Pixel[width * height];
}

Image::~Image() {
    delete[] buffer;
}

Image::Image(Image &img) {
    width = img.GetWidth();
    height = img.GetHeight();
    buffer = new Pixel[width * height];
    memcpy(buffer, img.GetBuffer(), width * height);
}

void Image::SetWidth(int w) {
    width = w;
}

void Image::SetHeight(int h) {
    height = h;
}

void Image::SetSize(int w, int h) {
    width = w;
    height = h;
}

void Image::SetBuffer() {
    if (buffer != NULL) {
        delete[] buffer;
    }
    buffer = new Pixel[width * height];
}

int Image::GetWidth() const {
    return width;
}

int Image::GetHeight() const {
    return height;
}

Pixel* Image::GetBuffer() const {
    return buffer;
}

void Image::SetSource(Source *source) {
    _source = source;
}

void Image::Update() const {
    if (_source != NULL && isUpdated == false) {
        isUpdated = true;
        _source->Update();
    }
}
