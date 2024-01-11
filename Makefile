all: build

build: clean
	cd 3rdParty/cryptopp890 && make libcryptopp.a
	mkdir build && cd build && cmake ..

serv: build
	cd build && make ServerR && ./ServerR

docker_compose:
	docker-compose up -d

test: build
	cd build && make Test && ./Test

coverage: build
	cd build && make coverage && open coverage_report.html

clean:
	rm -rf build