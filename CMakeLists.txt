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
        main.cpp
        Controller/UserController.hpp
        Controller/BidController.hpp
        Repository/BidRepository.cpp
        Repository/TransactionRepository.cpp
        Repository/UserRepository.cpp
        Service/BidService.hpp
        Service/TransactionService.hpp
        Utility/Binder.hpp
        Utility/Timestamper.hpp
        HTTP/Http.hpp
        Utility/ServerMessage.hpp
        Utility/ServerMessage.cpp
        Controller/TransactionController.hpp
)
#target_compile_options(ServerR PRIVATE -lboost_thread)
TARGET_LINK_LIBRARIES(ServerR PRIVATE Threads::Threads ${Boost_LIBRARIES} ${PQ_LIBRARIES} ${PQXX_LIBRARIES})


#ADD_EXECUTABLE(ClientR Session/main.cc)
#TARGET_LINK_LIBRARIES(ClientR PRIVATE Threads::Threads ${Boost_LIBRARIES})