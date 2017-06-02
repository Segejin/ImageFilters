#ifndef FILTERS_H
#define FILTERS_H

#include <image.h>
#include <source.h>
#include <sink.h>
#include <stdio.h>

class Filter : public Source, public Sink {
    public:
        virtual void Update();
        virtual const char *FilterName() = 0;
        virtual const char *SourceName();
        virtual const char *SinkName();    
};

class Shrinker : public Filter {
    public:
        Shrinker();
        virtual void Execute();
        virtual const char *FilterName();
    private:
        const char *name;
};

class LRConcat : public Filter {
    public:
        LRConcat();
        virtual void Execute();
        virtual const char *FilterName();      
    private:
        const char *name;
};

class TBConcat : public Filter {
    public:
        TBConcat();
        virtual void Execute();
        virtual const char *FilterName();    
    private:
        const char *name;
};

class Blender : public Filter {
    protected:
        float blendFactor;
    public:
        Blender();
        void SetFactor(float blendFactor);
        virtual void Execute();
        virtual const char *FilterName();    
    private:
        const char *name;
};

class Crop : public Filter {
    public:
        Crop();
        virtual const char *FilterName();
        virtual void Execute();
        void SetRegion(int Istart_, int Istop_, int Jstart_, int jStop_);
    private:
        const char *name;
        int Istart, Istop, Jstart, Jstop;
};

class Invert : public Filter {
    public:
        Invert();
        virtual void Execute();
        virtual const char *FilterName();
    private:
        const char *name;
};

class Transpose : public Filter {
    public:
        Transpose();
        virtual void Execute();
        virtual const char *FilterName();
    private:
        const char *name;
};

class Checkerboard : public Filter {
    public:
        Checkerboard();
        virtual void Execute();
        virtual const char *FilterName();
    private:
        const char *name;
};

class Color : public Source {
    public:
        Color(int w, int h, unsigned char r, unsigned char g, unsigned char b);
        virtual void Execute();
        const char *SourceName();
    private:
        const char *name;
        unsigned char red, green, blue;
};

class CheckSum : public Sink {
    public:
        CheckSum();
        const char *SinkName();
        void OutputCheckSum(const char *fileName); 
    private:
        const char *name;
        unsigned char red, green, blue;
        FILE *file;
};

#endif
