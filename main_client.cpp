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
                    << "Enter command:\n1 - LogIn\n2 - Register\n3 - Create Sell Bid\n4 - Create Buy Bid\n5 - CancelBid\n6 - ChangePassword"
                       "\n7 - View All Sell Bids\n8 - View All Buy Bids\n9 - ChangeUsername\n10 - DeleteMe\n"
                       "11 - UpdateBidRate\n12 - UpdateBidQuantity\n";
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
                    client.ChangePassword();
                    break;
                case 7:
                    client.GetMySellBids();
                    break;
                case 8:
                    client.GetMyBuyBids();
                    break;
                case 9:
                    client.ChangeName();
                    break;
                case 10:
                    client.DeleteMe();
                    break;
                case 11:
                    client.UpdateBidRate();
                    break;
                case 12:
                    client.UpdateBidQuantity();
                    break;
                default:
                    std::cout << "invalid input\n";
            }

        }
    }

}

///login, register, created bid(sell/buy), delete user, change name, change password, get all sell/buy bids

///fix occasional null at request start read by server