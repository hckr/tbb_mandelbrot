#include "mandelbrot.hpp"
#include <cmath>
#include <iterator>
#include <tbb/flow_graph.h>
#include <vector>

template <typename T>
class vector_source {

    std::vector<T>::const_iterator current, end;

public:
    vector_source(const std::vector<T> &vec) : current(std::begin(vec)), end(std::end(vec)) {}

    bool operator()(T &obj) {
        if (current != end) {
            obj = *current++;
            return true;
        } else {
            return false;
        }
    }
};

struct pixels_with_args {
    std::vector<png::rgb_pixel> pixels;
    mandelbrot::image_args image_args;
};

int main() {
    std::vector<mandelbrot::image_args> image_args_vec{
        {.width = 512u,
         .height = 512u,
         .top_left = {-2.0, -1.25},
         .bottom_right = {0.5, 1.25}},
        {.width = 512u,
         .height = 512u,
         .top_left = {-0.5, -0.5},
         .bottom_right = {0.5, 0.5}},
        {.width = 512u,
         .height = 512u,
         .top_left = {0.25, -0.05},
         .bottom_right = {0.35, 0.05}},
        {.width = 512u,
         .height = 512u,
         .top_left = {-0.80, 0.05},
         .bottom_right = {-0.70, 0.15}}};

    tbb::flow::graph g;

    tbb::flow::source_node<mandelbrot::image_args> source(g, vector_source(image_args_vec), false);

    tbb::flow::function_node<mandelbrot::image_args, pixels_with_args> draw1(g, tbb::flow::serial, [](auto args) {
        return pixels_with_args{mandelbrot::imagePixels(args, mandelbrot::get_colors), args};
    });

    tbb::flow::function_node<mandelbrot::image_args, pixels_with_args> draw2(g, tbb::flow::serial, [](auto args) {
        return pixels_with_args{mandelbrot::imagePixels(args, mandelbrot::get_colors2), args};
    });

    tbb::flow::join_node<tbb::flow::tuple<pixels_with_args, pixels_with_args>> join(g);

    tbb::flow::function_node<tbb::flow::tuple<pixels_with_args, pixels_with_args>, std::tuple<pixels_with_args, pixels_with_args, pixels_with_args>> merge(g, tbb::flow::serial, [](auto args_tuple) {
        pixels_with_args &data1 = tbb::flow::get<0>(args_tuple);
        pixels_with_args &data2 = tbb::flow::get<1>(args_tuple);

        pixels_with_args merged = data1;

        const std::size_t offset = 25;
        const std::size_t pixels_size = merged.pixels.size();

        for (std::size_t i = 0; i < pixels_size; ++i) {
            const auto offset_i = (i + offset) % pixels_size;
            merged.pixels[i].red += data2.pixels[offset_i].red;
            merged.pixels[i].red /= 2;
            merged.pixels[i].green += data2.pixels[offset_i].green;
            merged.pixels[i].green /= 2;
            merged.pixels[i].blue += data2.pixels[offset_i].blue;
            merged.pixels[i].blue /= 2;
        }

        return std::tuple{data1, data2, merged};
    });

    tbb::flow::function_node<std::tuple<pixels_with_args, pixels_with_args, pixels_with_args>, int> save(g, tbb::flow::serial, [](auto results) {
        auto &results1 = std::get<0>(results);
        auto &results2 = std::get<1>(results);
        auto &results3 = std::get<2>(results);

        mandelbrot::imageFromPixels(results1.image_args, results1.pixels).write(results1.image_args.filename() + "_1.png");
        mandelbrot::imageFromPixels(results2.image_args, results2.pixels).write(results2.image_args.filename() + "_2.png");
        mandelbrot::imageFromPixels(results3.image_args, results3.pixels).write(results3.image_args.filename() + "_3.png");
        return 1;
    });

    tbb::flow::make_edge(source, draw1);
    tbb::flow::make_edge(source, draw2);
    tbb::flow::make_edge(draw1, tbb::flow::get<0>(join.input_ports()));
    tbb::flow::make_edge(draw2, tbb::flow::get<1>(join.input_ports()));
    tbb::flow::make_edge(join, merge);
    tbb::flow::make_edge(merge, save);

    source.activate();
    g.wait_for_all();

    // image.write("mandelbrot.png");
}
