#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/binding/MoreOptionsLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;
using namespace keybinds;

class SCMenu : public CCLayer {
private:
    void addButton(CCMenu* menu, const char* label, CCPoint pos, SEL_MenuHandler callback) {
        auto btn = CCMenuItemSpriteExtra::create(ButtonSprite::create(label), this, callback);
        btn->setScale(0.8f);
        btn->m_baseScale = 0.8f;
        btn->setPosition(pos);
        menu->addChild(btn);
    }

public:
    static SCMenu* create() {
        auto ret = new SCMenu();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init() override {
        if (!CCLayer::init()) return false;

        setTouchEnabled(true);
        setKeyboardEnabled(true);

#ifdef GEODE_IS_WINDOWS
        if (auto glView = CCDirector::sharedDirector()->getOpenGLView()) {
            glView->toggleLockCursor(false);
            glView->showCursor(true);
        }
#endif

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto center = CCPoint(winSize / 2);

        // Dim background
        this->addChild(CCLayerColor::create(ccc4(0, 0, 0, 128)));

        // Window background
        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({ winSize.width / 2.2f, winSize.height / 3.1f });
        bg->setPosition(center);
        this->addChild(bg);

        // Menu title
        auto title = CCLabelBMFont::create("Scroll Clicking Menu", "bigFont.fnt");
        title->setScale(0.475f);
        title->setPosition({ center.x, bg->getPositionY() + (bg->getContentSize().height / 2) + 20 });
        this->addChild(title);

        auto menu = CCMenu::create();
        menu->setPosition({ 0, 0 });
        this->addChild(menu);

        // Buttons
        addButton(menu, "Settings", { center.x, center.y + 20 }, menu_selector(SCMenu::openSettings));
        addButton(menu, "Keybinds", { center.x, center.y - 20 }, menu_selector(SCMenu::openKeybinds));

        // Close button (top-left of background)
        auto close = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
            this, menu_selector(SCMenu::onClose)
        );
        close->setScale(0.7f);
        close->m_baseScale = 0.7f;
        close->setPosition(bg->getPosition() + CCPoint(-bg->getContentSize().width / 2, bg->getContentSize().height / 2));
        menu->addChild(close);

        return true;
    }

    void openSettings(cocos2d::CCObject*) {
        Mod* mod = Mod::get();
        geode::openSettingsPopup(mod, false);
    }

    void openKeybinds(cocos2d::CCObject*) {
        MoreOptionsLayer::create()->onKeybindings(nullptr);
        if (!Mod::get()->setSavedValue("opened-keybinds", true))
            FLAlertLayer::create(
                "Attention",
                "Search for Scroll Clicking",
                "Ok"
            )->show();
    }
    
    static int getHighestZOrder(CCNode* parent) {
        int maxZ = INT_MIN;
        CCArray* children = parent->getChildren();
        CCObject* obj;
        CCARRAY_FOREACH(children, obj) {
            auto child = static_cast<CCNode*>(obj);
            maxZ = std::max(maxZ, child->getZOrder());
        }
        return maxZ;
    }

    static void onOpen(CCNode* parent) {
        if (auto existing = parent->getChildByID("SCMenu"))
            existing->removeFromParentAndCleanup(true);
        else {
            auto overlay = SCMenu::create();
            overlay->setID("SCMenu");
            parent->addChild(overlay, getHighestZOrder(parent) + 1);
        }
    }

    void onClose(cocos2d::CCObject*) {
        this->removeFromParentAndCleanup(true);
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        return true;
    }

    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
    }

    void keyDown(enumKeyCodes key) override {
        if (key == KEY_Escape) {
            this->onClose(nullptr);
            return;
        }
    }

    void keyUp(enumKeyCodes key) override {}
};

$execute{
    BindManager::get()->registerBindable({
        "open_menu"_spr,
        "Open Menu",
        "Open Scroll Clicking's menu.",
        {Keybind::create(KEY_F1)},
        "Scroll Clicking",
        false
    });

    new EventListener([=](InvokeBindEvent* event) {
        if (event->isDown())
            SCMenu::onOpen(CCDirector::sharedDirector()->getRunningScene());
    return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "open_menu"_spr));
}