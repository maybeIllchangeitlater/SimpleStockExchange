#include "Client/Client.hpp"

int main(){
    s21::Client client;
    client.Connect("127.0.0.1", 5050);
    size_t command;
    while(true) {
        if(client.Connected()) {
//            std::cout << "connected\n";
            if(!client.Inbox().Empty()){
                std::cout << "server response: ";
                std::cout << client.Inbox().PopFront().second;
            }
            std::cout
                    << "Enter command:\n1 - LogIn\n2 - Register\n3 - Create Sell Bid\n4 - Create Buy Bid\n5 - CancelBid\n6 - View Bid"
                       "\n7 - View All Sell Bids\n8 - View All Buy Bids\n";
            std::cin >> command;
            switch (command) {
                case 1:
                    client.LogIn();
                    break;
                case 2:
                    client.Register();
                    break;
                case 3:
                    client.CreateSellBid();
                    break;
                case 4:
                    client.CreateBuyBid();
                    break;
                case 5:
                    client.CancelBid();
                    break;
                case 6:
                    client.CancelBid();
                    break;
                case 7:
                    client.GetMySellBids();
                    break;
                case 8:
                    client.GetMyBuyBids();
                    break;
                default:
                    std::cout << "invalid input\n";
            }

        }
    }
///fix ques. Client get responses, Server get request. Client has response q and reference to output q(should it? probably no)

}