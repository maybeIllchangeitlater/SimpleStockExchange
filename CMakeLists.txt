CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(SimpleStockExchange)

add_subdirectory(3rdParty/libpqxx build-pqxx)

find_package(Boost 1.40 COMPONENTS thread system unit_test_framework REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIR} 3rdParty/cryptopp890)

set(CMAKE_CXX_STANDARD 17)
set(GMOCK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/gmock)

include(FetchContent)
FetchContent_Declare(
        gmock_t
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(gmock_t)
include_directories(${gmock_t_SOURCE_DIR}/googlemock/include)
include_directories(${gmock_t_SOURCE_DIR}/googletest/include)

add_compile_options(-Wall -Werror -Wextra -pedantic)

add_executable(Test
        Tests/BD_Tests.cpp
        Tests/Service_Tests.cpp
        Tests/TestsMain.cpp
        Tests/Controller_Tests.cpp
        Repository/BdNames.hpp
        Repository/BalanceRepository.cpp
        Repository/UserRepository.cpp
        Repository/TransactionRepository.cpp
        Repository/BidRepository.cpp
        Service/BalanceService.hpp
        Service/BidService.cpp
        Service/TransactionService.cpp
        Service/UserService.cpp
        Utility/ServerMessage.cpp
        Service/BalanceServiceInterface.hpp
        Service/BidServiceInterface.hpp
        Service/TransactionServiceInterface.hpp
        Service/UserServiceInterface.hpp
        Controller/UserController.cpp
        Controller/UserController.hpp
        Controller/BidController.cpp
        Controller/BidController.hpp
        Controller/TransactionController.cpp
        Controller/TransactionController.hpp
        Utility/Encoder.hpp
        )
target_compile_definitions(Test PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY=1)
add_executable(ServerR
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
        Utility/ResponseError.hpp
        Repository/DatabaseInterface.hpp
        Repository/DatabaseImpl.hpp
        Service/BalanceServiceInterface.hpp
        Service/BidServiceInterface.hpp
        Service/TransactionServiceInterface.hpp
        Service/UserServiceInterface.hpp)

target_link_libraries(ServerR PRIVATE Threads::Threads ${Boost_LIBRARIES} pqxx
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/cryptopp890/libcryptopp.a)

target_link_libraries(Test PRIVATE Threads::Threads ${Boost_LIBRARIES} gmock_main pqxx
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/cryptopp890/libcryptopp.a)