#include "data.h"
#include "utils.h"

using boost::asio::ip::tcp;

#define MAX_BUF_SIZE 4096
#define ERR_MSG "ERROR: cannot parse query "

Messenger* messenger;

int main(int argc, char* argv[])
{
    try {
        // TODO: move client-server part to network.h
        // TODO: opt-args, choose port

        //load everything

        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8888));

        // TODO load from somewhere
        messenger = new Messenger();

        std::cerr << "Server started, port 8888." << std::endl;

        for (;;) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            char buf[MAX_BUF_SIZE];
            memset(buf, 0, MAX_BUF_SIZE);
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }

            std::vector<std::string> answer;
            std::vector<std::string> query;
            if (parseQuery(buf, len, query) == SUCCESS) {
                exec result = processQuery(messenger, query, answer);
                if (result == SUCCESS) {
                    std::cerr << "OK: " << buf << std::endl;
                    socket.write_some(boost::asio::buffer("OK"));
                    for (auto i = 0; i < answer.size(); ++i) {
                        socket.write_some(boost::asio::buffer("\t"));
                        socket.write_some(boost::asio::buffer(answer[i]));
                    }
                } else {
                    std::cerr << "NO: " << buf << std::endl;
                    socket.write_some(boost::asio::buffer("FAILED"));
                }
            } else {
                std::string print_error;
                print_error = ERR_MSG;
                print_error += buf;
                std::cerr << print_error << std::endl;
            }
            std::cerr << "DEBUG" << std::endl;
            std::cerr << "Users=" << messenger->getUsersSize() << std::endl;
            for (auto i = 0; i < messenger->getUsersSize(); ++i) {
                User* u = messenger->getUserPtr(i);
                std::cerr << "User " << i << ": id=" << u->getId() << " name=" << u->getName() << " email=" << u->getEmail() << std::endl;
            }
            std::cerr << "Channels=" << messenger->getChannelsSize() << std::endl;
            for (auto i = 0; i < messenger->getChannelsSize(); ++i) {
                Channel* c = messenger->getChannelPtr(i);
                std::cerr << "Channel " << i << ": id=" << c->getId() << " name=" << c->getName() << " link=" << c->getLink() << " messages size=" << c->getMessagesSize() << std::endl;
                if (c->getMessagesSize()) {
                    std::vector<Message> messages = c->getMessages();
                    for (auto j = 0; j < messages.size(); ++j) {
                        std::cerr << " Message " << j << " user=" << messages[j].getUserPtr()->getName() << " msg=" << messages[j].getText() << std::endl;
                    }
                }
            }
            std::cerr << "===" << std::endl;
            
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Ugh, something wrong" << std::endl;
    }
    return 0;
}
