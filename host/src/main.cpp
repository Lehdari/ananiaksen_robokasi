#include <iostream>
#include <string>
#include <sstream>
#include <serialib.h>
#include <stdlib.h>
#include <stdio.h>

#include <gut_opengl/App.hpp>
#include <gut_opengl/Mesh.hpp>
#include <gut_utils/VertexData.hpp>


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
    bool serialConnected = false;
    char receiveBuffer[512] = {0};

    // Robot arm state
    float state[6] = {90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 120.0f};

    // DH parameters
    Eigen::Matrix<double, 5, 4> dhParameters;

    // Camera
    gut::Camera camera;

    // Coordinate frame shader and mesh
    gut::Shader coordinateFrameShader;
    gut::Mesh coordinateFrameMesh;
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

    // Render arm
    context.coordinateFrameMesh.render(context.coordinateFrameShader, context.camera, Mat4f::Identity(), GL_LINES);

    if (context.serialConnected) {
        std::stringstream ss;
        ss << "A" << int(context.state[0])
           << "B" << int(context.state[1])
           << "C" << int(context.state[2])
           << "D" << int(context.state[3])
           << "E" << int(context.state[4])
           << "F" << int(context.state[5]) << std::endl;

        context.serial.writeString(ss.str().c_str());
        printf("sent: %s\n", ss.str().c_str());

        // Receive responses
        for (int i = 0; i < 512; ++i)
            context.receiveBuffer[i] = 0;
        context.serial.readString(context.receiveBuffer, '\n', 511, 100);

        context.serial.flushReceiver();

        printf("received: %s", context.receiveBuffer);
    }
}

int main(int argc, char const *argv[])
{
    // Setup app and render context
    App::Settings appSettings;
    appSettings.handleEvents = &handleEvents;
    appSettings.render = &render;
    App app(appSettings);

    Context context;
    app.setRenderContext(&context);

    // Camera setup - z upwards, fov of 60 degs
    context.camera.lookAt(Vec3f(10.0f, -3.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
    context.camera.projection(60.0f*(M_PI/180.0f), 1280.0f/720.0f, 0.1f, 100.0f);

    context.coordinateFrameShader.load("../res/shaders/vs_color.glsl", "../res/shaders/fs_color.glsl");

    // Vertex data for coordinate frame
    gut::VertexData coordinateFrameVertexData;
    coordinateFrameVertexData.addDataVector<Vec3f>("position", {
        Vec3f(0.0f, 0.0f, 0.0f),
        Vec3f(1.0f, 0.0f, 0.0f),
        Vec3f(0.0f, 0.0f, 0.0f),
        Vec3f(0.0f, 1.0f, 0.0f),
        Vec3f(0.0f, 0.0f, 0.0f),
        Vec3f(0.0f, 0.0f, 1.0f)
    });
    coordinateFrameVertexData.addDataVector<Vec3f>("color", {
        Vec3f(1.0f, 0.0f, 0.0f),
        Vec3f(1.0f, 0.0f, 0.0f),
        Vec3f(0.0f, 1.0f, 0.0f),
        Vec3f(0.0f, 1.0f, 0.0f),
        Vec3f(0.0f, 0.0f, 1.0f),
        Vec3f(0.0f, 0.0f, 1.0f)
    });
    coordinateFrameVertexData.setIndices({0,1,2,3,4,5});
    coordinateFrameVertexData.validate();
    context.coordinateFrameMesh.loadFromVertexData(coordinateFrameVertexData);

    // Setup DH parameters
    context.dhParameters <<
        0.0,    0.0,    0.2,    0.0,
        0.0,    0.0,    0.05,   -M_PI/2.0,
        0.0,    0.0,    0.25,   0.0,
        0.0,    0.0,    0.15,   0.0,
        0.0,    0.05,   0.1,    -M_PI/2.0;

    // Connection to serial port
    char errorOpening = context.serial.openDevice(SERIAL_PORT, 9600);
    context.serial.flushReceiver();

    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening==1) {
        printf("Successful connection to %s\n", SERIAL_PORT);
        context.serialConnected = true;
    }
    else {
        printf("Couldn't establish connection to %s (%d)\n",SERIAL_PORT, (int)errorOpening);
    }

    app.loop();

    context.serial.closeDevice();

    return 0;
}