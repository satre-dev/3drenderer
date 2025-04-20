#include "triangle.h"
#include "display.h"
#include <__stddef_null.h>
#include <_types/_uint32_t.h>

void int_swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// fill_flat_bottom_triangle calculates the slope of each leg of the triangle,
// finds the x_start and x_end value based on the slope applied to the initial
// x0 position, and draws a line from x_start to x_end line by line.
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float inv_slope_1 = (float)(x1-x0)/(y1-y0);
    float inv_slope_2 = (float)(x2-x0)/(y2-y0);

    float x_start = x0;
    float x_end = x0;
    for(int y = y0; y <= y2; y++) {
       draw_line(x_start, y, x_end, y, color);
       x_start += inv_slope_1;
       x_end += inv_slope_2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float inv_slope_1= (float)(x2-x0)/(y2-y0);
    float inv_slope_2= (float)(x2-x1)/(y2-y1);

    float x_start = x2;
    float x_end = x2;
    for (int y = y2; y >= y0; y--){
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Sort vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1){
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y0 > y1){
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    // Check if triangle is already flat top/flat bottom (prevents div by 0)
    if (y1 == y2) {
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        int My = y1;
        int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
    }
}
