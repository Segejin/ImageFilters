#include <image.h>
#include <source.h>
#include <logging.h>
#include <stdio.h>

Source::Source() {
    image.SetSource(this);
}

void Source::Update() {
    char msg[128];
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

Image* Source::GetOutput() {
    return &image;
}


