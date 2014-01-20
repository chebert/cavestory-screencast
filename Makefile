compile:
	clang++ -o gen/cavestory ./CaveStory/src/**.cc -lSDL
run:
	cd gen && ./cavestory
