all: build

build: clean
	cd 3rdParty/cryptopp890 && make libcryptopp.a
	mkdir build && cd build && cmake ..

serv: build
	cd build && make ServerR && ./ServerR

docker_serv:
	docker network inspect mynetwork > /dev/null 2>&1 || docker network create mynetwork
	if [ ! "$(shell docker ps -a -q -f name=postgres-container)" ]; then \
		echo "Postgre Container does not exist. Initializing..."; \
      	docker run --name postgres-container --network mynetwork -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=postgres -e POSTGRES_DB=postgres -p 5432:5432 -v mypgdata:/var/lib/postgresql/data -v $(CURDIR):/docker-entrypoint-initdb.d -d postgres:latest; \
      	docker volume create mypgdata; \
    else \
    	echo "Starting existing Postgre Container..."; \
        docker start postgres-container; \
    fi
	docker build -t simple_stock_exchange .
	docker run --network mynetwork -p 5050:5050 simple_stock_exchange

docker_compose:
	docker-compose up -d

test: build
	cd build && make Test && ./Test

coverage: build
	cd build && make coverage && open coverage_report.html

run_server:
	cd build && ./ServerR

clean:
	rm -rf build