build:
	g++ -w -std=c++14 -Wfatal-errors \
	-g ./*.cpp \
	-o demo \
	-lglfw \
	-lGL \
	-lGLU \
	-lGLEW;

clean:
	rm ./demo;

run:
	./demo;

test:
	g++ -w -std=c++14 -Wfatal-errors \
	-g ./*.cpp \
	-o demo \
	-lglfw \
	-lGL \
	-lGLU \
	-lGLEW;
	./demo;