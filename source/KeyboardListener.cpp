#include "KeyboardListener.h"
#include "udp_client.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

extern std::atomic<bool> isRunning;

void keyboard_listener(UDPClient& client) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
            "SDL2 Keyboard Events",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_SHOWN
    );

    bool running = true;
    SDL_Event e;

    while (running && isRunning.load()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: client.send_command("start_forward"); break;
                    case SDLK_a: client.send_command("start_left"); break;
                    case SDLK_s: client.send_command("start_backward"); break;
                    case SDLK_d: client.send_command("start_right"); break;
                    case SDLK_1: client.send_command("autonomously-control"); break;
                    case SDLK_0: client.send_command("manual-control"); break;
                    case SDLK_c:
                        isRunning.store(false);
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                client.send_command("stop-right");
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

