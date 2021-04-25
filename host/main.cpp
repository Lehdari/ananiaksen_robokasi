#include <iostream>
#include <string>
#include <sstream>
#include <serialib.h>

#include <stdlib.h>
#include <stdio.h>


#include <gut_opengl/App.hpp>


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#if defined (_WIN32) || defined(_WIN64)
    #define SERIAL_PORT "COM1"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyUSB0"

#endif


struct Context {
    // Serial object
    serialib serial;

    // Robot arm state
    float state[6] = {90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 120.0f};

    char receiveBuffer[512] = {0};
};


// Alias for the app type
using App = gut::App<Context>;

// Pipeline function for the event handling
void handleEvents(SDL_Event& event, App::Context& appContext)
{
    // Handle SDL events
    switch (event.type) {
        case SDL_QUIT:
            *appContext.quit = true;
            break;

        case SDL_KEYDOWN:
            // Skip events if imgui widgets are being modified
            if (ImGui::IsAnyItemActive())
                return;
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    *appContext.quit = true;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

// Pipeline function for rendering
void render(Context& context, App::Context& appContext)
{
    static Vec3f cameraPos(0.0f, 1.0f, 5.0f);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render imgui
    {
        ImGui::Begin("Camera");
        ImGui::SliderFloat("A", &context.state[0], 45.f, 135.f);
        ImGui::SliderFloat("B", &context.state[1], 45.f, 135.f);
        ImGui::SliderFloat("C", &context.state[2], 45.f, 135.f);
        ImGui::SliderFloat("D", &context.state[3], 45.f, 135.f);
        ImGui::SliderFloat("E", &context.state[4], 45.f, 135.f);
        ImGui::SliderFloat("F", &context.state[5], 90.f, 150.f);
        ImGui::End();
    }

    std::stringstream ss;
    ss  << "A" << int(context.state[0])
        << "B" << int(context.state[1])
        << "C" << int(context.state[2])
        << "D" << int(context.state[3])
        << "E" << int(context.state[4])
        << "F" << int(context.state[5]) << std::endl;

    context.serial.writeString(ss.str().c_str());
    printf("sent: %s\n", ss.str().c_str());

    // Receive responses
    for (int i=0; i<512; ++i)
        context.receiveBuffer[i] = 0;
    context.serial.readString(context.receiveBuffer, '\n', 511, 100);

    context.serial.flushReceiver();

    printf("received: %s", context.receiveBuffer);
}

int main(int argc, char const *argv[])
{
    // Setup app and render context
    App::Settings appSettings;
    appSettings.handleEvents = &handleEvents;
    appSettings.render = &render;

    Context context;

    // Connection to serial port
    char errorOpening = context.serial.openDevice(SERIAL_PORT, 9600);
    context.serial.flushReceiver();

    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening==1) {
        printf("Successful connection to %s\n", SERIAL_PORT);
    }
    else {
        printf("Couldn't establish connection to %s (%d)\n",SERIAL_PORT, (int)errorOpening);
        return -1;
    }

    App app(appSettings);
    app.setRenderContext(&context);

    app.loop();

    context.serial.closeDevice();

    return 0;
}
