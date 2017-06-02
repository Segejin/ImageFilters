#ifndef SOURCE_H
#define SOURCE_H

#include <image.h>

class Source {
    protected:
        Image image;
        bool isUpdated;
        virtual void Execute() = 0;

    public:
	Source();
        Image* GetOutput();
        virtual void Update();
        virtual const char *SourceName() = 0;
};

#endif
