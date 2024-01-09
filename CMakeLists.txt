CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(SimpleStockExchange)

ADD_SUBDIRECTORY(3rdParty/libpqxx build-pqxx)

FIND_PACKAGE(Boost 1.40 COMPONENTS thread system unit_test_framework REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} 3rdParty/cryptopp890)

SET(CMAKE_CXX_STANDARD 17)
SET(GMOCK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/gmock)

include(FetchContent)
FetchContent_Declare(
        gmock_t
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(gmock_t)
INCLUDE_DIRECTORIES(${gmock_t_SOURCE_DIR}/googlemock/include)
INCLUDE_DIRECTORIES(${gmock_t_SOURCE_DIR}/googletest/include)

ADD_COMPILE_OPTIONS(-Wall -Werror -Wextra -pedantic)



ADD_EXECUTABLE(Test
        Tests/BD_Tests.cpp
        Repository/BdNames.hpp
        Repository/BalanceRepository.cpp
        Repository/UserRepository.cpp
        Repository/TransactionRepository.cpp
        Repository/BidRepository.cpp
        Utility/ServerMessage.cpp)
#add_dependencies(Test gmock_t)
target_compile_definitions(Test PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY=1)
ADD_EXECUTABLE(ServerR
        Repository/BdNames.hpp
        Repository/UserRepository.cpp
        Repository/BidRepository.cpp
        Repository/TransactionRepository.cpp
        Service/BidService.cpp
        Service/UserService.cpp
        Service/TransactionService.cpp
        Controller/ControllerMapping.cpp
        Controller/UserController.cpp
        Controller/BidController.cpp
        Controller/TransactionController.cpp
        HTTP/Http.hpp
        Connection/Connection.cpp
        Server/Server.cpp
        Utility/Encoder.hpp
        Utility/ServerMessage.cpp
        Utility/ThreadSafeQ.hpp
        Utility/Timestamper.hpp
        3rdParty/json.hpp
        main.cpp
        Connection/Connection.cpp
        Connection/Connection.hpp
        Utility/ExtraJSONKeys.hpp
        Repository/BalanceRepository.cpp
        Repository/BalanceRepository.hpp
        Service/BalanceService.hpp
        Utility/ResponseParser.cpp
        Utility/ResponseParser.hpp
        Utility/ResponseError.hpp Repository/DatabaseInterface.hpp Repository/DatabaseImpl.hpp)

TARGET_LINK_LIBRARIES(ServerR PRIVATE Threads::Threads ${Boost_LIBRARIES} pqxx
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/cryptopp890/libcryptopp.a)

target_link_libraries(Test PRIVATE Threads::Threads ${Boost_LIBRARIES} gmock_main pqxx)