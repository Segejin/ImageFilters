#ifndef SINK_H
#define SINK_H

#include <image.h>

class Sink {
    protected:
        const Image *image1;
        const Image *image2;

    public:
	Sink();
        void CheckInputOne();
        void CheckInputTwo();
        void SetInput(Image *image1);
        void SetInput2(Image *image2);
        virtual const char *SinkName() = 0;
};

#endif
