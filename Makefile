compile:
	clang++ -o -g gen/cavestory CaveStory/src/**.cc -lSDL
run:
	cd gen && ./cavestory
