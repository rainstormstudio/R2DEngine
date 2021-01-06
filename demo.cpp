#define DEBUG_ENABLED 1
#include "R2DEngine.hpp"

class Demo : public R2DEngine {
    double t;
    uint8_t calcR(int n) {
        n += t;
        n /= innerWidth / 1.2;
        int x = round(t) + n;
        x += 255;
        x = x % 500;
        return (abs(255 - x));
    }
    uint8_t calcG(int n) {
        n += t;
        n /= innerWidth / 1.2;
        int x = round(t) + n;
        x = x % 500;
        return (abs(255 - x));
    }
    uint8_t calcB(int n) {
        n += t;
        n /= innerWidth / 1.2;
        int x = round(t) + n;
        x += 128;
        x = x % 500;
        return (abs(255 - x));
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
        for (int x = 0; x < innerWidth; x ++) {
            for (int y = 0; y < innerHeight; y ++) {
                drawPoint({x, y}, {
                    ((calcR(x * (innerHeight - y)))),
                    ((calcG(y * (innerWidth - x)))),
                    ((calcB(x * y)))
                });
            }
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