build: clean
	mkdir -p build
	gcc parent.c -o ./build/program
	gcc child1.c -o ./build/child1
	gcc child2.c -o ./build/child2

run: build
	./build/program

clean:
	rm -rf ./build/
