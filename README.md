# TBB Mandelbrot
## Generating and visualizing Mandelbrot set/fractal using Threading Building Blocks (TBB) and png++

Project explores `tdd::flow::graph` features in the implementation of a somewhat artifically created need to draw Mandelbrot fractals concurrently and then merge them (for that else could we use `tdd::flow::join`!) into another image. If you look for inspiration how to implement something with TBB, feel free to grab some code and maybe also contribute! I of course can't guarantee that the code is safe and well-written (I know it's a mess), it was my first time with TBB. But look at the images below – something as easy as pixel offset during merge of two images can make interesting effect!


## Gallery

![](images/512_512_-2.000000_-1.250000_0.500000_1.250000_100_3.png)
![](images/512_512_0.250000_-0.050000_0.350000_0.050000_100_3.png)
![](images/512_512_-0.500000_-0.500000_0.500000_0.500000_100_3.png)
![](images/512_512_-0.800000_0.050000_-0.700000_0.150000_100_3.png)
