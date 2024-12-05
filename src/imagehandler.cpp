#include "imagehandler.h"

vector<Texture> ImageHandler::textures;
vector<Image> ImageHandler::images;

void ImageHandler::setImage(const vector<Image> &img) {
    for (auto i : img) {
        images.push_back(ImageCopy(i));
    }
}

void ImageHandler::ImageVectorCopy(const vector<Image> &src, vector<Image> &dest) {
    for (auto i : src) {
        dest.push_back(ImageCopy(i));
    }
}

vector<Image> ImageHandler::getImages() {
    return images;
}

void ImageHandler::addImage(const Image &img) {
    images.push_back(ImageCopy(img));
}

vector<Image>& ImageHandler::setImages(string path) {
    images.clear();
    if (path == "smallmario") {
        images.push_back(LoadImage("resources/images/smallmario/idle.png"));
        images.push_back(LoadImage("resources/images/smallmario/duck.png"));
        images.push_back(LoadImage("resources/images/smallmario/walk.png"));
        images.push_back(LoadImage("resources/images/smallmario/idle.png")); // walk2
        images.push_back(LoadImage("resources/images/smallmario/jump.png"));
        images.push_back(LoadImage("resources/images/smallmario/fall.png"));
        images.push_back(LoadImage("resources/images/smallmario/pipe.png"));
        images.push_back(LoadImage("resources/images/smallmario/dead.png"));
        images.push_back(LoadImage("resources/images/smallmario/hold.png"));
        images.push_back(LoadImage("resources/images/smallmario/victory.png"));
    }
    else if (path == "bigmario") {
        images.push_back(LoadImage("resources/images/bigmario/idle.png"));
        images.push_back(LoadImage("resources/images/bigmario/duck.png"));
        images.push_back(LoadImage("resources/images/bigmario/walk.png"));
        images.push_back(LoadImage("resources/images/bigmario/walk2.png")); // walk2
        images.push_back(LoadImage("resources/images/bigmario/jump.png"));
        images.push_back(LoadImage("resources/images/bigmario/fall.png"));
        images.push_back(LoadImage("resources/images/bigmario/pipe.png"));
        images.push_back(LoadImage("resources/images/bigmario/dead.png"));
        images.push_back(LoadImage("resources/images/bigmario/hold.png"));
        images.push_back(LoadImage("resources/images/bigmario/victory.png"));
    }
    else if (path == "smallluigi") {
        images.push_back(LoadImage("resources/images/smallluigi/idle.png"));
        images.push_back(LoadImage("resources/images/smallluigi/duck.png"));
        images.push_back(LoadImage("resources/images/smallluigi/walk.png"));
        images.push_back(LoadImage("resources/images/smallluigi/idle.png")); // walk2
        images.push_back(LoadImage("resources/images/smallluigi/jump.png"));
        images.push_back(LoadImage("resources/images/smallluigi/fall.png"));
        images.push_back(LoadImage("resources/images/smallluigi/pipe.png"));
        images.push_back(LoadImage("resources/images/smallluigi/dead.png"));
        images.push_back(LoadImage("resources/images/smallluigi/hold.png"));
        images.push_back(LoadImage("resources/images/smallluigi/victory.png"));
    }
    else if (path == "bigluigi") {
        images.push_back(LoadImage("resources/images/bigluigi/idle.png"));
        images.push_back(LoadImage("resources/images/bigluigi/duck.png"));
        images.push_back(LoadImage("resources/images/bigluigi/walk.png"));
        images.push_back(LoadImage("resources/images/bigluigi/walk2.png"));
        images.push_back(LoadImage("resources/images/bigluigi/jump.png"));
        images.push_back(LoadImage("resources/images/bigluigi/fall.png"));
        images.push_back(LoadImage("resources/images/bigluigi/pipe.png"));
        images.push_back(LoadImage("resources/images/bigluigi/dead.png"));
        images.push_back(LoadImage("resources/images/bigluigi/hold.png"));
        images.push_back(LoadImage("resources/images/bigluigi/victory.png"));
    }
    else if (path == "princess") {
        images.push_back(LoadImage("resources/images/princess/free.png"));
        images.push_back(LoadImage("resources/images/princess/scream.png"));
        images.push_back(LoadImage("resources/images/princess/kiss.png"));
    }
    else if (path == "goomba") {
        images.push_back(LoadImage("resources/images/goomba/idle.png"));
        images.push_back(LoadImage("resources/images/goomba/walk.png"));
        images.push_back(LoadImage("resources/images/goomba/idle.png")); // walk2
        images.push_back(LoadImage("resources/images/goomba/dead.png"));
    }
    else if (path == "koopa") {
        images.push_back(LoadImage("resources/images/koopa/idle.png"));
        images.push_back(LoadImage("resources/images/koopa/walk1.png"));
        images.push_back(LoadImage("resources/images/koopa/walk2.png"));
        images.push_back(LoadImage("resources/images/koopa/rot3.png"));
        images.push_back(LoadImage("resources/images/koopa/rot2.png"));
        images.push_back(LoadImage("resources/images/koopa/rot1.png"));
        images.push_back(LoadImage("resources/images/koopa/rot4.png"));
    }
    else if (path == "flykoopa") {
        images.push_back(LoadImage("resources/images/koopa/fly1.png"));
        images.push_back(LoadImage("resources/images/koopa/fly2.png"));
        images.push_back(LoadImage("resources/images/koopa/rotfly1.png")); 
        images.push_back(LoadImage("resources/images/koopa/rotfly2.png"));
    }
    else if (path == "beetle") {
        images.push_back(LoadImage("resources/images/beetle/idle.png"));
        images.push_back(LoadImage("resources/images/beetle/walk.png"));
        images.push_back(LoadImage("resources/images/beetle/idle.png"));
        images.push_back(LoadImage("resources/images/beetle/rot1.png"));
        images.push_back(LoadImage("resources/images/beetle/rot2.png"));
        images.push_back(LoadImage("resources/images/beetle/rot3.png"));
        images.push_back(LoadImage("resources/images/beetle/rot4.png"));
    }
    else if (path == "bullet") {
        images.push_back(LoadImage("resources/images/object/bullet.png"));
    }
    else if (path == "castroll") {
        images.push_back(LoadImage("resources/images/object/castroll.png"));
    }
    else if (path == "fireball") {
        images.push_back(LoadImage("resources/images/object/fireball.png"));
    }
    else if (path == "spike1") {
        images.push_back(LoadImage("resources/images/object/spike1.png"));
    }
    else if (path == "spike2") {
        images.push_back(LoadImage("resources/images/object/spike2.png"));
    }
    
    // copy vector
    return images;
}

Texture ImageHandler::TextureCopy(const Texture &srcTexture) {
    // Create a new image from the source texture
    Image srcImage = LoadImageFromTexture(srcTexture);

    // Create a new texture from the image
    Texture2D newTexture = LoadTextureFromImage(srcImage);

    // Unload the image to free memory
    UnloadImage(srcImage);

    return newTexture;
}

vector<Texture>& ImageHandler::TextureCopy(const vector<Texture> &srcTextures) {
    for (auto srcTexture : srcTextures) {
        textures.push_back(TextureCopy(srcTexture));
    }
    return textures;
}

void ImageHandler::TextureCopy(const vector<Texture> &src, vector<Texture> &dest) {
    for (auto srcTexture : src) {
        dest.push_back(TextureCopy(srcTexture));
    }
}


void ImageHandler::setTextures(const vector<Texture> &img) {
    TextureCopy(img);
}

vector<Texture> ImageHandler::getTextures() {
    return textures;
}

void ImageHandler::addTexture(const Texture &img) {
    textures.push_back(TextureCopy(img));
}

vector<Texture>& ImageHandler::setTextures(string path) {
    textures.clear();
    setImages(path);
    for (auto img : images) {
        Texture texture = LoadTextureFromImage(img);
        textures.push_back(texture);
    }
    return textures;
}