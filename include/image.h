#include "include.h"

class ImageHandler {
private:
    vector<Image> images;
public:
    ImageHandler();
    ImageHandler(vector<Image> imgs);
    ImageHandler(const ImageHandler &ih);
    ~ImageHandler();
    void setImage(const Image &img);
    vector<Image> getImages();
    void addImage(const Image &img);
    void setImages(string path = "mario");
};