#include "HelloWorldScene.h"
#include "sqlite3.h"

USING_NS_CC;

#define TEST_SQLCIPHER
#define TEST_OPENSSL

#ifdef TEST_SQLCIPHER

void testSQLCipher() {
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath() + "test.db";
    std::string passphrase("abcdefghijklmn");
    sqlite3 *db;
    sqlite3_open(path.c_str(), &db);
    sqlite3_key(db, passphrase.c_str(), static_cast<int>(passphrase.length()));
    sqlite3_exec(db, "CREATE TABLE chara(id INTEGER, name TEXT);", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "INSERT INTO chara VALUES(1, 'キャラA');", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "INSERT INTO chara VALUES(2, 'キャラB');", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "INSERT INTO chara VALUES(3, 'キャラC');", nullptr, nullptr, nullptr);

    sqlite3_stmt *stmt;
    sqlite3_prepare(db, "SELECT * FROM chara;", -1, &stmt, nullptr);
    while (SQLITE_DONE != sqlite3_step(stmt)) {
        auto id = sqlite3_column_int(stmt, 0);
        auto name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        CCLOG("id = %d, name = %s", id, name);
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

#endif

#ifdef TEST_OPENSSL

#include <array>
#include <vector>
#include <openssl/evp.h>

cocos2d::Sprite *createSpriteFromEncryptedFile(const std::string &fileName) {
    static const std::array<unsigned char, 16> KEY = {
            'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
            'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};

    auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    if (auto texture = cocos2d::Director::getInstance()->getTextureCache()->getTextureForKey(fullPath)) {
        return cocos2d::Sprite::createWithTexture(texture);
    }

    auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(fullPath);

    EVP_CIPHER_CTX context;
    EVP_DecryptInit(&context, EVP_aes_128_ecb(), KEY.data(), nullptr);

    std::vector<unsigned char> buff(data.getSize());
    int len;
    int outLen = 0;
    if (!EVP_DecryptUpdate(&context, buff.data(), &len, data.getBytes(), static_cast<int>(data.getSize()))) {
        return nullptr;
    }
    outLen += len;

    if (!EVP_DecryptFinal(&context, buff.data() + len, &len)) {
        return nullptr;
    }
    outLen += len;

    cocos2d::Image image;
    image.initWithImageData(buff.data(), outLen);

    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(&image, fullPath);
    return cocos2d::Sprite::createWithTexture(texture);
}

#endif

Scene *HelloWorld::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

#ifdef TEST_SQLCIPHER
    testSQLCipher();
#endif

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
            origin.y + closeItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
#ifdef TEST_OPENSSL
    auto sprite = createSpriteFromEncryptedFile("HelloWorld.dat");
#else
    auto sprite = Sprite::create("HelloWorld.png");
#endif

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    return true;
}


void HelloWorld::menuCloseCallback(Ref *pSender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
