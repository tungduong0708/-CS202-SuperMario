#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include "include.h"
#include "animation.h"

class ImageHandler {
private:
    static vector<Texture> textures;
    static vector<Image> images;
public:
    // ImageHandler();
    // ImageHandler(const vector<Image> &imgs);
    // ImageHandler(const ImageHandler &ih);
    // ~ImageHandler();
    static void setImage(const vector<Image> &img);
    static void ImageVectorCopy(const vector<Image> &src, vector<Image> &dest);
    vector<Image> getImages();
    static void addImage(const Image &img);
    static vector<Image>& setImages(string path = "smallmario");

    // --- TextureHandler ---
    static Texture TextureCopy(const Texture &img);
    static vector<Texture>& TextureCopy(const vector<Texture> &imgs);
    static void TextureCopy(const vector<Texture> &src, vector<Texture> &dest);
    static void setTextures(const vector<Texture> &img);
    vector<Texture> getTextures();
    static void addTexture(const Texture &img);
    static vector<Texture>& setTextures(string path = "smallmario");
};

class AnimationHandler {
private:
    static vector<Animation> animations;
public:
    static void setAnimations(const vector<Animation> &anim);
    static vector<Animation>& setAnimations(string path);
    static void AnimationVectorCopy(const vector<Animation> &src, vector<Animation> &dest);
};

#endif // IMAGEHANDLER_H