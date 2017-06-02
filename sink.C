#include <image.h>
#include <sink.h>
#include <logging.h>
#include <stdio.h>

Sink::Sink() {
    image1 = NULL;
    image2 = NULL;
}

void Sink::CheckInputOne() {
    if (image1 == NULL) {
        char msg[128];
        sprintf(msg, "no input1!");
        DataFlowException e(SinkName(), msg);
        throw e;
    }
}

void Sink::CheckInputTwo() {
    if (image2 == NULL) {
        char msg[128];
        sprintf(msg, "no input2!");
        DataFlowException e(SinkName(), msg);
        throw e;
    }
}

void Sink::SetInput(Image *image1) {
    this->image1 = image1;
}

void Sink::SetInput2(Image *image2) {
    this->image2 = image2;
}
