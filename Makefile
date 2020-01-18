main: main.cpp
	g++ --std=c++2a main.cpp mandelbrot.cpp -o tbb_mandelbrot -ltbb `libpng-config --cflags` `libpng-config --ldflags`

clean:
	rm tbb_mandelbrot
