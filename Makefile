compile:
	clang++ -o gen/cavestory src/**.cc -lSDL
run:
	cd gen && ./cavestory
