## Simple Stock Exchange client-server application
### Overview:
- Simple application simulating stock exchange
  
- Login/Register screen
- User settings (change username/password/delete account as well as veiwing USD and RUB balance, name and unique id)
- Sell/buy bids screen (create, update, delete), transactions are made when bids are matched (automatically) during create/update
- Transaction screen (view buy and sell transactions)
- Quotation screen (view quotations for last day/week/month/year)
  
- used libraries: boost::asio, cryptopp, libpqxx, nlohmann json, googlemock, boost tests
- requirments: docker to run server, client part tested on macos hopefully works on linux


### Installation 
- make docker_compose in source directory, launch docker compose to run server
- make install for client application part (installed to desktop)
  
