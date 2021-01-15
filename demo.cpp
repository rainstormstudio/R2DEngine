#define DEBUG_ENABLED 1
//#define USE_SDL2 1
#include "R2DEngine.hpp"

class Demo : public R2DEngine {
    GLint t_loc;
    GLint innerWidth_loc;
    GLint innerHeight_loc;

    double t;
    
public:
    bool onCreate() override {
        windowTitle = "DEMO";
        t = 0.0f;

        t_loc = glGetUniformLocation(shader, "t");
        innerWidth_loc = glGetUniformLocation(shader, "innerWidth");
        innerHeight_loc = glGetUniformLocation(shader, "innerHeight");

        return true;
    }

    bool onUpdate(double deltaTime) override {
        t += deltaTime * 120.0;
        if (t > 500.0f) {
            t = 0.0f;
        }

        glUniform1f(t_loc, t);
        glUniform1i(innerWidth_loc, innerWidth);
        glUniform1i(innerHeight_loc, innerHeight);

        return true;
    }
};

int main() {
    Demo demo;
    if (demo.construct(1280, 720, 1920, 1080)) {
        demo.init("", "./shaders/final.fsh");
    }

    return 0;
}