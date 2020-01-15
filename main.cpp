#include "mandelbrot.hpp"
#include <cmath>

int main() {
    auto image = mandelbrot::image({.width = 512u,
                                    .height = 512u,
                                    .top_left = {-2.0, -1.25},
                                    .bottom_right = {0.5, 1.25}});

    image.write("mandelbrot.png");
}
