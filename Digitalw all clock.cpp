#include <iostream>
#include <SDL.h>
#include <cmath>
#include <ctime>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

// Function to draw a hand on the clock
void drawHand(int length, double angle, SDL_Color color) {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int endX = centerX + static_cast<int>(length * sin(angle));
    int endY = centerY - static_cast<int>(length * cos(angle));

    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(gRenderer, centerX, centerY, endX, endY);
}

// Function to update the clock hands' angles
void updateClockHands(double& hourAngle, double& minuteAngle, double& secondAngle) {
    std::time_t currentTime = std::time(nullptr);
    struct tm* localTime = std::localtime(&currentTime);

    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;

    hourAngle = (hours % 12) * (2 * M_PI / 12) + (minutes / 60.0) * (2 * M_PI / 12);
    minuteAngle = minutes * (2 * M_PI / 60);
    secondAngle = seconds * (2 * M_PI / 60);
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    gWindow = SDL_CreateWindow("Digital Wall Clock", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 2;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 3;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        double hourAngle, minuteAngle, secondAngle;
        updateClockHands(hourAngle, minuteAngle, secondAngle);

        // Draw clock hands
        drawHand(150, hourAngle, {0, 0, 0, 255});    // Hour hand
        drawHand(200, minuteAngle, {0, 0, 0, 255});  // Minute hand
        drawHand(220, secondAngle, {255, 0, 0, 255}); // Second hand

        SDL_RenderPresent(gRenderer);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}

