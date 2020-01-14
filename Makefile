main: main.cpp
	g++ --std=c++17 main.cpp -o tbb_mandelbrot -ltbb `libpng-config --cflags` `libpng-config --ldflags`

clean:
	rm tbb_mandelbrot
