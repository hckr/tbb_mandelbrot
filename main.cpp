#include <iostream>
#include <png++/png.hpp>
#include <tbb/parallel_invoke.h>

int main() {
    tbb::parallel_invoke([]() { std::cout << "test1 "; }, []() { std::cout << "test2\n"; });

    png::image<png::rgb_pixel>
        image(128, 128);
    for (png::uint_32 y = 0; y < image.get_height(); ++y) {
        for (png::uint_32 x = 0; x < image.get_width(); ++x) {
            image[y][x] = png::rgb_pixel(x, y, x + y);
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    image.write("rgb.png");
}
