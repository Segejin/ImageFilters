#ifndef PNMREADER_H
#define PNMREADER_H

#include <stdio.h>
#include <source.h>

class PNMreader : public Source {
    protected:
        char* _fileName;
        int w, h, mV;
        char magicNum[128];
    public:
        PNMreader(char* fileName);
        virtual void Execute();
        virtual void Update();
        virtual const char* SourceName();
    private:
        const char* name;
};

#endif
