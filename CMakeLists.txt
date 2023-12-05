CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(SimpleStockExchange)

include(SelectLibraryConfigurations)

find_path(libpqxx_INCLUDE_DIR /pqxx/pqxx)
find_library(libpqxx_LIBRARY_RELEASE NAMES libpqxx)
find_library(libpqxx_LIBRARY_DEBUG NAMES libpqxxd)
select_library_configurations(libpqxx)

FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
#FIND_PACKAGE(PQXX REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${PQXX_INCLUDE_DIRECTORIES})

SET(CMAKE_CXX_STANDARD 17)



ADD_EXECUTABLE(ServerR
        Server/Session.hpp
        Server/Session.cpp
        Utility/Binder.hpp
        Repository/Handlers.hpp
        main.cpp
        Server/Server.cpp
        Repository/UserRepository.hpp
        Repository/BidRepository.hpp
        Repository/UserRepository.cpp
        Utility/Encoder.hpp
        Utility/UUIDGenerator.hpp
        Repository/BidRepository.cpp)
#target_compile_options(ServerR PRIVATE -lboost_thread)
TARGET_LINK_LIBRARIES(ServerR PRIVATE Threads::Threads ${Boost_LIBRARIES} ${PQ_LIBRARIES} ${PQXX_LIBRARIES})


#ADD_EXECUTABLE(ClientR Session/main.cc)
#TARGET_LINK_LIBRARIES(ClientR PRIVATE Threads::Threads ${Boost_LIBRARIES})