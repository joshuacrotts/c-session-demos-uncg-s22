# 362/462/662 - Multithreaded Fractal Generator

This is a multithreaded fractal generator.

# Recompiling

To recompile, first install the development X11 libraries via

```
sudo apt-get install libx11-dev
```

Then compile via `make`.

# Running

The argument specification is below. Because of how the app is programmed, you must enter a number of threads that divides into the width of the frame. For instance, if we have a widow width of 600, we can choose any number of threads that divides into 600. Arguments as follows:

```
./fractal width height max_iterations num_threads c1 c2 min_x min_y max_x max_y
```

