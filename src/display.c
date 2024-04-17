#include "display.h"

// SDL window and renderer
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// buffers
SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;

// window vars
int window_width = 800;
int window_height = 600;

// initialize_window: generates SDL_window values based on currently set `window_width` and `window_height`
// returning a bool which we use to validate success of process. True=success, false=failure.
// Also automatically sets window to fullscreen by default.
bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_width = display_mode.w;    
    window_height = display_mode.h;

    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}


// destroy_window: frees memory consumed by color_buffer, renderer and window.
void destroy_window(){
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// draw_pixel: takes x, y as cartesian coordinates in a 2D plane to populate color_buffer entry 
// as value of arg `color`.
void draw_pixel(int x, int y, uint32_t color) {
    if(x < window_width && y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    }
}

// draw_rectangle: takes arguments `x` and `y` as integers for offset from top-left of screen (0 point in buffer)
// and draws rectangle from left-to-right and bottom-to-top populating each entry in color_buffer with value of
// argument `color`.
void draw_rectangle(int x, int y, int width, int height, uint32_t color){
    for (int dx = x; dx < (x + width); dx++) {
        for(int dy = y; dy < (y + height); dy++) {
            color_buffer[(window_width * dy) + dx] = color;
        }
    }
}

// draw_grid: steps 10 pixels across x and y in our color_buffer and assigns this a uint32_t value of 0xFF333333.
void draw_grid(void) {
    for (int y = 0; y < window_height; y+=10) {
        for (int x = 0; x < window_width; x+=10) {
            color_buffer[(window_width * y) + x] = 0xFF333333;
        }
    }
}

// render_color_buffer: calles `SDL_UpdateTexture` and `SDL_RenderCopy` to pass our color_buffer 
// and color_buffer_texture variables to SDL libs which render it in a window.
void render_color_buffer(void) {
    SDL_UpdateTexture(
            color_buffer_texture,
            NULL,
            color_buffer,
            (int)(window_width * sizeof(uint32_t))
            );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

// clear_color_buffer: takes uint32_t `color` as arg and sets each entry in color_buffer array to `color`.
void clear_color_buffer(uint32_t color) {
    for (int i = 0, buffer_size = window_height * window_width; i < buffer_size; i++){
        *(color_buffer+i) = color;
    }
}

