#ifndef IMAGE_HEADER
#define IMAGE_HEADER

#include<cstddef>

class Source;

struct Pixel {
    unsigned char red, green, blue;
};

class Image {
    private:
        int width, height;
        Pixel *buffer;
        Source *_source;
        mutable bool isUpdated;

    public:
	Image();
	~Image();
	Image(int w, int h);
	Image(Image &img);
	void SetWidth(int w);
        void SetHeight(int h);
        void SetSize(int w, int h);
        void SetBuffer();
        int GetWidth() const;
        int GetHeight() const;
        Pixel* GetBuffer() const;
        void SetSource(Source *source);
        void Update() const;
};

#endif
