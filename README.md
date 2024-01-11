## Simple Stock Exchange client-server application
### Overview:
- Simple application simulating stock exchange
  
- Login/Register screen

![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/loginregister.gif)
- User settings (change username/password/delete account as well as veiwing USD and RUB balance, name and unique id)

![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/usersettings.gif)
- Sell/buy bids screen (create, update, delete), transactions are made when bids are matched (automatically) during create/update

![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/bid.gif)
- Transaction screen (view buy and sell transactions)

![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/transactions.gif)
- Quotation screen (view quotations for last day/week/month/year)

![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/quotations.png)

- used libraries: boost::asio, cryptopp, libpqxx, nlohmann json, googlemock, boost tests
- requirments: docker to run server, client part tested on macos hopefully works on linux


### Installation 
- make docker_compose in source directory, launch docker compose to run server
![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/docker%20app.png)
![alt text](https://github.com/maybeIllchangeitlater/SimpleStockExchange/blob/develop/ReadmeResources/docker.png)
- make install for client application part (installed to desktop)
  
