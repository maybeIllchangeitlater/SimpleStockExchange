all:
	mkdir build && cd build && cmake .. && make

serv:
	cd build && ./ServerR

cli:
	cd build && ./ClientR

clean:
	rm -rf build