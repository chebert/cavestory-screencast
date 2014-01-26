compile:
<<<<<<< HEAD
	clang++ -g -o gen/cavestory ./CaveStory/src/**.cc -lSDL
=======
	clang++ -o gen/cavestory src/**.cc `sdl-config --cflags --libs`
>>>>>>> 41bfe0a... Fixes: OSX crash
run:
	cd gen && ./cavestory
