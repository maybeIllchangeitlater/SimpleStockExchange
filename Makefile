all: clean
	mkdir build && cd build && cmake .. && make

serv:
	cd build && ./ServerR

test:
	cd build && ./Test

clean:
	rm -rf build
