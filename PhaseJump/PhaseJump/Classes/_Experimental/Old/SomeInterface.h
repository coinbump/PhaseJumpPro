#ifndef PJSOMEINTERFACE_h
#define PJSOMEINTERFACE_h

#include "Class.h"
#include "ClassIds.h"
#include <stdexcept>
#include <vector>
#include <memory>

//#define CLASS(id, Type)

namespace PJ {
    /// <summary>
    /// A window for displaying GUI
    /// </summary>
    class SomeWindow {
        virtual void Render() = 0;
    };

    /// <summary>
    /// Manages GUI
    /// </summary>
    class SomeInterface {
        using WindowSharedPtr = SP<SomeWindow>;
        using WindowList = std::vector<WindowSharedPtr>;
        WindowList windows;

    public:
        virtual WindowSharedPtr NewWindow(Vector2 size, String title) {
            MakeWindow(size, title);
            windows.push_back(window);
        }

        virtual WindowSharedPtr MakeWindow(Vector2 size, String title) = 0;

        virtual ~SomeInterface();

        virtual void Render() {
            for (auto window : windows) {
                if (!window.get()) { continue; }
                window->Render();
            }
        }
    };
}

namespace GLFWKit {
//    class Window : public SomeWindow {
//        Window(Vector2 size, String title) {
//            GLFWwindow* window;
//            window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
//            if (!window) {
//                glfwTerminate();
//                throw std::exception();
//            }
//            glfwMakeContextCurrent(window);
//        }
//
//        void Render() override {
//            float ratio;
//            int width, height;
//
//            glfwGetFramebufferSize(window, &width, &height);
//            ratio = (float) width / (float) height;
//
//           glViewport(0, 0, width, height);
//            glClear(GL_COLOR_BUFFER_BIT);
//
//               glMatrixMode(GL_PROJECTION);
//                glLoadIdentity();
//                glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//                glMatrixMode(GL_MODELVIEW);
//                glLoadIdentity();
//
//            glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
//                glBegin(GL_TRIANGLES);
//                glColor3f(1.f, 0.f, 0.f);
//                glVertex3f(-0.6f, -0.4f, 0.f);
//                glColor3f(0.f, 1.f, 0.f);
//                glVertex3f(0.6f, -0.4f, 0.f);
//                glColor3f(0.f, 0.f, 1.f);
//                glVertex3f(0.f, 0.6f, 0.f);
//                glEnd();
//
//            glfwSwapBuffers(window);
//            glfwPollEvents();
//        }
//    };
//
//    class Interface : public SomeInterface() {
//    protected:
//        bool isLoaded = false;
//
//    bool:
//        void Go() {
//            if (isLoaded) { return; }
//
//            if (!glfwInit()) {
//                return;
//            }
//
//            isLoaded = true;
//        }
//
//        SomeWindow* MakeWindow(Vector2 size, String title) override {
//            Go();
//        }
//    }
}

#endif
