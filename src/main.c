#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "array.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"

// Array of triangles that should be rendered frame by frame
triangle_t *triangles_to_render = NULL;

// Global variables for execution status and game loop
vec3_t camera_position = {0, 0, 0};
float fov_factor = 540;

bool is_running = false;
int previous_frame_time = 0;

// Setup function to initialise variables and game objects
void setup(void) {
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  if (color_buffer == NULL) {
    fprintf(stderr, "Error allocating memory for color_buffer in setup\n");
  }

  // Creating a SDL texture that is used to display the color buffer
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  load_obj_file_data("./assets/f22.obj");
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
    break;
  default:
    break;
  }
}

// project: receives a 3d vector and returns a projected 2d point
vec2_t project(vec3_t point) {
  vec2_t projected_point = {.x = (fov_factor * point.x) / point.z,
                            .y = (fov_factor * point.y) / point.z};

  return projected_point;
}

void update(void) {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(),
                           previous_frame_time + FRAME_TARGET_TIME))
    ;

  previous_frame_time = SDL_GetTicks();

  // Initialise the array of triangles to render
  triangles_to_render = NULL;

  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.01;

  // Loop all triangle faces of our mesh
  int num_faces = array_length(mesh.faces);
  for (int i = 0; i < num_faces; i++) {
    face_t mesh_face = mesh.faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[mesh_face.c - 1];

    vec3_t transformed_vertices[3];

    // Loop all 3 vertices of this current face and apply transformations
    for (int j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];
      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      // Translate the vertex away from the camera
      transformed_vertex.z += 5;

      // Save transformed vertex in array of transformed vertices
      transformed_vertices[j] = transformed_vertex;
    }

    // loop all the three vertices to perform projection
    vec3_t vector_a = transformed_vertices[0];
    vec3_t vector_b = transformed_vertices[1];
    vec3_t vector_c = transformed_vertices[2];

    // Get the vector subtraction B-A and C-A
    vec3_t vector_ab = vec3_sub(vector_b, vector_a);
    vec3_t vector_ac = vec3_sub(vector_c, vector_a);
    vec3_normalize(&vector_ab);
    vec3_normalize(&vector_ac);

    // Compute the face normal (using cross product to find perpendicular)
    vec3_t normal = vec3_cross(vector_ab, vector_ac);

    // Normalize the normal
    vec3_normalize(&normal);

    // Find the ray between camera and a point in the triangle
    vec3_t camera_ray = vec3_sub(camera_position, vector_a);

    // Calculate camera ray alignment with face normal
    float dot_normal_camera = vec3_dot(normal, camera_ray);

    if (dot_normal_camera < 0) {
      continue;
    }

    triangle_t projected_triangle;
    for (int j = 0; j < 3; j++) {

      // project the current vertex
      vec2_t projected_point = project(transformed_vertices[j]);

      // Scale and translate the projected point to the middle of the screen
      projected_point.x += ((float)window_width / 2);
      projected_point.y += ((float)window_height / 2);
      projected_triangle.points[j] = projected_point;
    }
    // save the projected triangle in the array of triangles to render
    array_push(triangles_to_render, projected_triangle);
  }
}

void render(void) {
  draw_grid();

  // Loop all projected triangles and render them
  int num_triangles = array_length(triangles_to_render);
  for (int i = 0; i < num_triangles; i++) {
    triangle_t triangle = triangles_to_render[i];

    draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
                  triangle.points[1].x, triangle.points[1].y,
                  triangle.points[2].x, triangle.points[2].y, 0xFFFFFFFF);

    draw_triangle(triangle.points[0].x, triangle.points[0].y,
                  triangle.points[1].x, triangle.points[1].y,
                  triangle.points[2].x, triangle.points[2].y, 0x00000000);
  }


  // Clear the array of triangles to render every frame loop
  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
}

// Free the memory that was dynamically allocated by the program
void free_resources(void) {
  free(color_buffer);
  array_free(mesh.faces);
  array_free(mesh.vertices);
}

int main(void) {
  is_running = initialize_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();

  return 0;
}
