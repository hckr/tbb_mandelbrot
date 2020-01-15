#pragma once
#include <complex>
#include <png++/png.hpp>

namespace mandelbrot {

struct image_args {
    unsigned width = 512u;
    unsigned height = 512u;
    std::complex<double> top_left = {-2.0, -1.25};
    std::complex<double> bottom_right = {0.5, 1.25};
    unsigned iter_limit = 100u;
};
png::image<png::rgb_pixel> image(image_args args = {});

}; // namespace mandelbrot
