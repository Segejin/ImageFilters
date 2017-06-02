#ifndef PNMWRITER_H
#define PNMWRITER_H

#include <stdio.h>
#include <sink.h>

class PNMwriter : public Sink {
    public:
        PNMwriter();
        void Write(char *fileName);
        virtual const char* SinkName();
    private:    
        const char* name;
};

#endif
