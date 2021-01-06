#define DEBUG_ENABLED 1
#include "R2DEngine.hpp"

class Demo : public R2DEngine {
    double t;
    uint8_t calcR(int n) {
        n += t;
        n /= screenWidth;
        int x = round(t) + n;
        x += 255;
        x = x % 500;
        return static_cast<uint8_t>(abs(255 - x));
    }
    uint8_t calcG(int n) {
        n += t;
        n /= screenWidth;
        int x = round(t) + n;
        x = x % 500;
        return static_cast<uint8_t>(abs(255 - x));
    }
    uint8_t calcB(int n) {
        n += t;
        n /= screenWidth;
        int x = round(t) + n;
        x += 128;
        x = x % 500;
        return static_cast<uint8_t>(abs(255 - x));
    }
public:
    bool onCreate() override {
        windowTitle = "DEMO";
        t = 0.0f;
        return true;
    }

    bool onUpdate(double deltaTime) override {
        t += deltaTime * 120.0;
        if (t > 500.0f) {
            t = 0.0f;
        }
        for (int x = 0; x < screenWidth; x ++) {
            for (int y = 0; y < screenHeight; y ++) {
                drawPoint({x, y}, {
                    static_cast<uint8_t>(round(calcR(x * (screenHeight - y)))),
                    static_cast<uint8_t>(round(calcG(y * (screenWidth - x)))),
                    static_cast<uint8_t>(round(calcB(x * y)))
                });
            }
        }
        for (int x = 0; x < 10; x ++) {
            drawPoint({mousePosX + x - 5, mousePosY}, {255, 255, 255, 100});
        }
        for (int y = 0; y < 10; y ++) {
            drawPoint({mousePosX, mousePosY + y - 5}, {255, 255, 255, 100});
        }
        return true;
    }
};

int main() {
    Demo demo;
    if (demo.construct()) {
        demo.init();
    }

    return 0;
}