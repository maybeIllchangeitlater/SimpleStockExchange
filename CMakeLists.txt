CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(SimpleStockExchange)

ADD_SUBDIRECTORY(3rdParty/libpqxx build-pqxx)

FIND_PACKAGE(Boost 1.40 COMPONENTS thread system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${PQXX_INCLUDE_DIRECTORIES} 3rdParty/cryptopp890)

SET(CMAKE_CXX_STANDARD 17)



ADD_EXECUTABLE(ServerR
        Repository/BdNames.hpp
        Repository/UserRepository.cpp
        Repository/BidRepository.cpp
        Repository/TransactionRepository.cpp
        Service/UserService.hpp
        Service/BidService.hpp
        Service/TransactionService.hpp
        Controller/ControllerMapping.cpp
        Controller/UserController.cpp
        Controller/BidController.cpp
        Controller/TransactionController.cpp
        HTTP/Http.hpp
        Connection/Connection.cpp
        Server/Server.cpp
        Utility/Encoder.hpp
        Utility/RequestBuilder.hpp
        Utility/ServerMessage.cpp
        Utility/ThreadSafeQ.hpp
        Utility/Timestamper.hpp
        Utility/UUIDGenerator.hpp
        3rdParty/json.hpp
        main.cpp
)
TARGET_LINK_LIBRARIES(ServerR PRIVATE Threads::Threads ${Boost_LIBRARIES} pqxx ${PQ_LIBRARIES} ${PQXX_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/cryptopp890/libcryptopp.a)
