compile:
	clang++ -o gen/cavestory src/**.cc `sdl-config --cflags --libs`
run:
	gen/cavestory
