#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "include.h"

class ImageHandler {
private:
    static vector<Image> images;
public:
    // ImageHandler();
    // ImageHandler(const vector<Image> &imgs);
    // ImageHandler(const ImageHandler &ih);
    // ~ImageHandler();
    static void setImage(const Image &img);
    vector<Image> getImages();
    static void addImage(const Image &img);
    static vector<Image>& setImages(string path = "smallmario");
};

#endif // IMAGEHANDLER_H