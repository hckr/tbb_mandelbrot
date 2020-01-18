#include "mandelbrot.hpp"

namespace mandelbrot {

auto highest_mandelbrot_iteration(const std::complex<double> &p, unsigned iter_limit) {
    auto z = std::complex{0.0, 0.0};
    unsigned i = 0;
    for (; std::abs(z) < 2 && i < iter_limit; ++i) {
        z = std::pow(z, 2) + p;
    }
    return i;
}

std::vector<png::rgb_pixel> get_colors(unsigned iter_limit) {
    std::vector<png::rgb_pixel> colors(iter_limit);

    for (auto i = 0; i < iter_limit; ++i) {
        colors[i].red = 255.0 * i / iter_limit;
        colors[i].green = 255.0 * i / iter_limit;
        colors[i].blue = 255.0 * std::log(i) / std::log(iter_limit);
    }

    return colors;
}

std::vector<png::rgb_pixel> get_colors2(unsigned iter_limit) {
    std::vector<png::rgb_pixel> colors(iter_limit);

    for (auto i = 0; i < iter_limit; ++i) {
        colors[i].red = i % 255;
        colors[i].green = i % 255;
        colors[i].blue = i % 255;
    }

    return colors;
}

auto image(image_args args, std::function<std::vector<png::rgb_pixel>(unsigned iter_limit)> custom_get_colors) -> png::image<png::rgb_pixel> {
    auto image = png::image<png::rgb_pixel>{args.width, args.height};
    auto re_ratio = (std::real(args.bottom_right) - std::real(args.top_left)) / args.width;
    auto im_ratio = (std::imag(args.bottom_right) - std::imag(args.top_left)) / args.height;
    auto colors = custom_get_colors ? custom_get_colors(args.iter_limit) : get_colors(args.iter_limit);

    for (png::uint_32 y = 0; y < image.get_height(); ++y) {
        auto im = y * im_ratio + std::imag(args.top_left);
        for (png::uint_32 x = 0; x < image.get_width(); ++x) {
            auto re = x * re_ratio + std::real(args.top_left);
            auto max_iter = highest_mandelbrot_iteration({re, im}, args.iter_limit);
            image[y][x] = colors[max_iter];
        }
    }

    return image;
}

auto imageFromPixels(image_args args, std::vector<png::rgb_pixel> pixels) -> png::image<png::rgb_pixel> {
    auto image = png::image<png::rgb_pixel>{args.width, args.height};

    for (png::uint_32 y = 0; y < image.get_height(); ++y) {
        for (png::uint_32 x = 0; x < image.get_width(); ++x) {
            image[y][x] = pixels[y * args.width + x];
        }
    }

    return image;
}

auto imagePixels(image_args args, std::function<std::vector<png::rgb_pixel>(unsigned iter_limit)> custom_get_colors) -> std::vector<png::rgb_pixel> {
    auto pixels = std::vector<png::rgb_pixel>(args.width * args.height);
    auto re_ratio = (std::real(args.bottom_right) - std::real(args.top_left)) / args.width;
    auto im_ratio = (std::imag(args.bottom_right) - std::imag(args.top_left)) / args.height;
    auto colors = custom_get_colors ? custom_get_colors(args.iter_limit) : get_colors(args.iter_limit);

    for (png::uint_32 y = 0; y < args.height; ++y) {
        auto im = y * im_ratio + std::imag(args.top_left);
        for (png::uint_32 x = 0; x < args.width; ++x) {
            auto re = x * re_ratio + std::real(args.top_left);
            auto max_iter = highest_mandelbrot_iteration({re, im}, args.iter_limit);
            pixels[y * args.width + x] = colors[max_iter];
        }
    }

    return pixels;
}

} // namespace mandelbrot
