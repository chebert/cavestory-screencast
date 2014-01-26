compile:
	clang++ -g -o gen/cavestory CaveStory/src/**.cc `sdl-config --cflags --libs`
run:
	cd gen && ./cavestory
