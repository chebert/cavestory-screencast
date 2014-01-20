compile:
<<<<<<< HEAD
	clang++ -o -g gen/cavestory CaveStory/src/**.cc -lSDL
=======
	clang++ -o gen/cavestory ./CaveStory/src/**.cc -lSDL
>>>>>>> 73dbb5e... End of Episode 017
run:
	cd gen && ./cavestory
