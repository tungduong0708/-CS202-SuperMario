#include "image.h"

ImageHandler::ImageHandler() {
    images = {};
}

ImageHandler::ImageHandler(vector<Image> imgs): images(imgs) {}

ImageHandler::ImageHandler(const ImageHandler &ih) {
    images = ih.images;
}

ImageHandler::~ImageHandler() {
    images = {};
}

void ImageHandler::setImage(const Image &img) {
    images.push_back(img);
}

vector<Image> ImageHandler::getImages() {
    return images;
}

void ImageHandler::addImage(const Image &img) {
    images.push_back(img);
}

void ImageHandler::setImages(string path) {
    images.clear();
    // read type from the path
    if (path == "smallmario") {
        images.push_back(LoadImage("resources/smallmario/idle.png"));
        images.push_back(LoadImage("resources/smallmario/duck.png"));
        images.push_back(LoadImage("resources/smallmario/walk.png"));
        images.push_back(LoadImage("resources/smallmario/jump.png"));
        images.push_back(LoadImage("resources/smallmario/fall.png"));
        images.push_back(LoadImage("resources/smallmario/pipe.png"));
        images.push_back(LoadImage("resources/smallmario/dead.png"));
        images.push_back(LoadImage("resources/smallmario/hold.png"));
        images.push_back(LoadImage("resources/smallmario/victory.png"));
    }
    else if (path == "bigmario") {
        images.push_back(LoadImage("resources/bigmario/idle.png"));
        images.push_back(LoadImage("resources/bigmario/duck.png"));
        images.push_back(LoadImage("resources/bigmario/walk.png"));
        images.push_back(LoadImage("resources/bigmario/jump.png"));
        images.push_back(LoadImage("resources/bigmario/fall.png"));
        images.push_back(LoadImage("resources/bigmario/pipe.png"));
        images.push_back(LoadImage("resources/bigmario/dead.png"));
        images.push_back(LoadImage("resources/bigmario/hold.png"));
        images.push_back(LoadImage("resources/bigmario/victory.png"));
    }
    else if (path == "smallluigi") {
        images.push_back(LoadImage("resources/smallluigi/idle.png"));
        images.push_back(LoadImage("resources/smallluigi/duck.png"));
        images.push_back(LoadImage("resources/smallluigi/walk.png"));
        images.push_back(LoadImage("resources/smallluigi/jump.png"));
        images.push_back(LoadImage("resources/smallluigi/fall.png"));
        images.push_back(LoadImage("resources/smallluigi/pipe.png"));
        images.push_back(LoadImage("resources/smallluigi/dead.png"));
        images.push_back(LoadImage("resources/smallluigi/hold.png"));
        images.push_back(LoadImage("resources/smallluigi/victory.png"));
    }
    else if (path == "bigluigi") {
        images.push_back(LoadImage("resources/bigluigi/idle.png"));
        images.push_back(LoadImage("resources/bigluigi/duck.png"));
        images.push_back(LoadImage("resources/bigluigi/walk.png"));
        images.push_back(LoadImage("resources/bigluigi/jump.png"));
        images.push_back(LoadImage("resources/bigluigi/fall.png"));
        images.push_back(LoadImage("resources/bigluigi/pipe.png"));
        images.push_back(LoadImage("resources/bigluigi/dead.png"));
        images.push_back(LoadImage("resources/bigluigi/hold.png"));
        images.push_back(LoadImage("resources/bigluigi/victory.png"));
    }
    else if (path == "princess") {
        images.push_back(LoadImage("resources/princess/free.png"));
        images.push_back(LoadImage("resources/princess/scream.png"));
        images.push_back(LoadImage("resources/princess/kiss.png"));
    }
    else if (path == "goomba") {
        images.push_back(LoadImage("resources/goomba/idle.png"));
        images.push_back(LoadImage("resources/goomba/walk.png"));
        images.push_back(LoadImage("resources/goomba/dead.png"));
    }
    else if (path == "koopa") {
        images.push_back(LoadImage("resources/koopa/idle.png"));
        images.push_back(LoadImage("resources/koopa/walk1.png"));
        images.push_back(LoadImage("resources/koopa/walk2.png"));
        images.push_back(LoadImage("resources/koopa/rot3.png"));
        images.push_back(LoadImage("resources/koopa/rot2.png"));
        images.push_back(LoadImage("resources/koopa/rot1.png"));
        images.push_back(LoadImage("resources/koopa/rot4.png"));
    }
    else if (path == "flykoopa") {
        images.push_back(LoadImage("resources/koopa/fly1.png"));
        images.push_back(LoadImage("resources/koopa/fly2.png"));
        images.push_back(LoadImage("resources/koopa/rotfly1.png")); 
        images.push_back(LoadImage("resources/koopa/rotfly2.png"));
    }
    else if (path == "beetle") {
        images.push_back(LoadImage("resources/beetle/idle.png"));
        images.push_back(LoadImage("resources/beetle/walk.png"));
        images.push_back(LoadImage("resources/beetle/rot1.png"));
        images.push_back(LoadImage("resources/beetle/rot2.png"));
        images.push_back(LoadImage("resources/beetle/rot3.png"));
        images.push_back(LoadImage("resources/beetle/rot4.png"));
    }
    else if (path == "bullet") {
        images.push_back(LoadImage("resources/object/bullet.png"));
    }
    else if (path == "castroll") {
        images.push_back(LoadImage("resources/object/castroll.png"));
    }
    else if (path == "fireball") {
        images.push_back(LoadImage("resources/object/fireball.png"));
    }
    else if (path == "spike1") {
        images.push_back(LoadImage("resources/object/spike1.png"));
    }
    else if (path == "spike2") {
        images.push_back(LoadImage("resources/object/spike2.png"));
    }
    
}