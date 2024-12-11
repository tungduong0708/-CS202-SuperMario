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
        images.push_back(LoadImage("resources/images/koopa/rot3.png")); // dead
    }
    else if (path == "flykoopa") {
        images.push_back(LoadImage("resources/images/koopa/fly1.png"));
        images.push_back(LoadImage("resources/images/koopa/fly2.png"));
        images.push_back(LoadImage("resources/images/koopa/rotfly1.png")); 
        images.push_back(LoadImage("resources/images/koopa/rotfly2.png"));
        images.push_back(LoadImage("resources/images/koopa/rot3.png")); // dead
    }
    else if (path == "beetle") {
        images.push_back(LoadImage("resources/images/beetle/idle.png"));
        images.push_back(LoadImage("resources/images/beetle/walk.png"));
        images.push_back(LoadImage("resources/images/beetle/idle.png"));
        images.push_back(LoadImage("resources/images/beetle/rot1.png"));
        images.push_back(LoadImage("resources/images/beetle/rot2.png"));
        images.push_back(LoadImage("resources/images/beetle/rot3.png"));
        images.push_back(LoadImage("resources/images/beetle/rot4.png"));
        images.push_back(LoadImage("resources/images/beetle/rot1.png")); // dead
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
    else if (path == "coin") {
        images.push_back(LoadImage("resources/images/coin/coin1.png"));
        images.push_back(LoadImage("resources/images/coin/coin2.png"));
        images.push_back(LoadImage("resources/images/coin/coin3.png"));
        images.push_back(LoadImage("resources/images/coin/coin4.png"));
    }
    else if (path == "mushroom") {
        images.push_back(LoadImage("resources/images/mushroom/mushroom.png"));
    }
    else if (path == "star") {
        images.push_back(LoadImage("resources/images/star/star1.png"));
        images.push_back(LoadImage("resources/images/star/star2.png"));
        images.push_back(LoadImage("resources/images/star/star3.png"));
        images.push_back(LoadImage("resources/images/star/star4.png"));
    }
    else if (path == "fireflower") {
        images.push_back(LoadImage("resources/images/fireflower/fireflower1.png"));
        images.push_back(LoadImage("resources/images/fireflower/fireflower2.png"));
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

vector<Texture>& ImageHandler::setTexturesDirect(const std::string &path)
{
    textures.clear();
    std::string imgPath = "resources/images/" + path + "/" + path + ".png";
    Image image = LoadImage(imgPath.c_str());
    Rectangle srcRect = {0, 0, (float)IMAGE_WIDTH, (float)IMAGE_WIDTH};
    for (int i = 0; i < image.width / IMAGE_WIDTH; i++) {
        srcRect.x = i * IMAGE_WIDTH;
        Texture texture = LoadTextureFromImage(ImageFromImage(image, srcRect));
        textures.push_back(texture);
    }
    return textures;
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

/////////////////////////////////////
// Animation Handler

vector<Animation> AnimationHandler::animations;


void AnimationHandler::setAnimations(const vector<Animation> &anim) {
    for (auto a : anim) {
        animations.push_back(a);
    }
}

// set the animations from the character type
vector<Animation>& AnimationHandler::setAnimations(string path) {
    animations.clear();
    vector<Texture> textures = ImageHandler::setTextures(path);
    if (path == "smallmario" or path == "bigmario" or path == "smallluigi" or path=="bigluigi") {
        animations = {
            Animation({Frame(0.1f, textures[0])}), // idle
            Animation({Frame(0.1f, textures[1])}), // duck
            Animation({Frame(0.1f, textures[2]), Frame(0.1f, textures[3])}), // walk & walk2
            Animation({Frame(0.1f, textures[4])}), // jump
            Animation({Frame(0.1f, textures[5])}), // fall
            Animation({Frame(0.1f, textures[6])}), // pipe
            Animation({Frame(0.1f, textures[7])}), // dead
            Animation({Frame(0.25f, textures[8])}), // hold
            Animation({Frame(0.1f, textures[9])}) // victory
        };
    }
    else if (path == "princess") {
        animations = {
            Animation({Frame(0.1f, textures[0])}), // free
            Animation({Frame(0.1f, textures[1])}), // scream
            Animation({Frame(0.1f, textures[2])}) // kiss
        };
    }
    else if (path == "goomba") {
        animations = {
            Animation({Frame(0.1f, textures[0])}), // idle
            Animation({Frame(0.1f, textures[1]), Frame(0.1f, textures[2])}), // walk & walk2
            Animation({Frame(0.1f, textures[3])}) // dead
        };
    }
    else if (path == "koopa") {
        animations = {
            Animation({Frame(0.1f, textures[0])}), // idle
            Animation({Frame(0.1f, textures[1]), Frame(0.1f, textures[2])}), // walk1 & walk2
            Animation({Frame(0.1f, textures[3]), Frame(0.1f, textures[4]), Frame(0.1f, textures[5]), Frame(0.1f, textures[6])}), // rotate
            Animation({Frame(0.1f, textures[7])}) // dead
        };
    }
    else if (path == "flykoopa") {
        animations = {
            Animation({Frame(0.1f, textures[0]), Frame(0.1f, textures[1])}), // fly1 & fly2
            Animation({Frame(0.1f, textures[2]), Frame(0.1f, textures[3])}), // rotfly1 & rotfly2
            Animation({Frame(0.1f, textures[4])}) // dead
        };
    }
    else if (path == "beetle") {
        animations = {
            Animation({Frame(0.1f, textures[0])}), // idle
            Animation({Frame(0.1f, textures[1]), Frame(0.1f, textures[2])}), // walk & walk2
            Animation({Frame(0.1f, textures[3]), Frame(0.1f, textures[4]), Frame(0.1f, textures[5]), Frame(0.1f, textures[6])}), // rotate
            Animation({Frame(0.1f, textures[7])}) // dead
        };
    }
    else if (path == "bullet") {
        animations = {
            Animation({Frame(0.1f, textures[0])}) // bullet
        };
    }
    else if (path == "castroll") {
        animations = {
            Animation({Frame(0.1f, textures[0])}) // castroll
        };
    }
    else if (path == "fireball") {
        animations = {
            Animation({Frame(0.1f, textures[0])}) // fireball
        };
    }
    else if (path == "spike1") {
        animations = {
            Animation({Frame(0.1f, textures[0])}) // spike1
        };
    }
    else if (path == "spike2") {
        animations = {
            Animation({Frame(0.1f, textures[0])}) // spike2
        };
    }
    else if (path == "coin") {
        animations = {
            Animation({Frame(0.1f, textures[0]), Frame(0.1f, textures[1]), Frame(0.1f, textures[2]), Frame(0.1f, textures[3])}) // coin
        };
    }
    else if (path == "mushroom") {
        animations = {
            Animation({Frame(0.1f, textures[0])}) // mushroom
        };
    }
    else if (path == "star") {
        animations = {
            Animation({Frame(0.15f, textures[0]), Frame(0.15f, textures[1]), Frame(0.15f, textures[2]), Frame(0.15f, textures[3])}) // star
        };
    }
    else if (path == "fireflower") {
        animations = {
            Animation({Frame(0.1f, textures[0]), Frame(0.1f, textures[1])}) // fireflower
        };
    }

    return animations;
}

void AnimationHandler::AnimationVectorCopy(const vector<Animation> &src, vector<Animation> &dest) {
    for (auto a : src) {
        dest.push_back(a);
    }
}
