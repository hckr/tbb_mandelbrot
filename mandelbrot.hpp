#pragma once
#include <complex>
#include <functional>
#include <png++/png.hpp>
#include <string>
#include <vector>

namespace mandelbrot {

struct image_args {
    unsigned width = 512u;
    unsigned height = 512u;
    std::complex<double> top_left = {-2.0, -1.25};
    std::complex<double> bottom_right = {0.5, 1.25};
    unsigned iter_limit = 100u;

    std::string filename() {
        return std::to_string(width) + "_" +
               std::to_string(height) + "_" +
               std::to_string(std::real(top_left)) + "_" +
               std::to_string(std::imag(top_left)) + "_" +
               std::to_string(std::real(bottom_right)) + "_" +
               std::to_string(std::imag(bottom_right)) + "_" +
               std::to_string(iter_limit);
    }
};
png::image<png::rgb_pixel> image(image_args args = {}, std::function<std::vector<png::rgb_pixel>(unsigned iter_limit)> custom_get_colors = nullptr);
png::image<png::rgb_pixel> imageFromPixels(image_args args, std::vector<png::rgb_pixel> pixels);
std::vector<png::rgb_pixel> imagePixels(image_args args = {}, std::function<std::vector<png::rgb_pixel>(unsigned iter_limit)> custom_get_colors = nullptr);

std::vector<png::rgb_pixel> get_colors(unsigned iter_limit);
std::vector<png::rgb_pixel> get_colors2(unsigned iter_limit);

}; // namespace mandelbrot
