#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"

bool is_running = false;

void setup(void) {
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        fprintf(stderr, "Error allocating memory for color_buffer in setup\n");
    }
    printf("successfully allocated memory for color_buffer\n");


    // Creating a SDL texture that is used to display the color buffer 
    color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height
            );
    printf("created color_buffer_texture\n");
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            if (event.key.keysym.sym == SDLK_r)
                draw_rectangle(300, 500, 500, 200, 0xFFFF0000);
            break;
        default:
            break;
    }
}

void update(void) {
    // TODO:
    return;
}


void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid();
    render_color_buffer();
    //clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(void) {
    is_running = initialize_window();

    setup();
    printf("setup complete\n");
    while(is_running){
        process_input();
        update();
        render();
    }
    destroy_window();
    return 0;
}
