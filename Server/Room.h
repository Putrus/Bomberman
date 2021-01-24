#pragma once
#include <unordered_set>
#include "Client.cpp"


class Room{
private:
    std::unordered_set<Client*> clients;
    int maxPlayers;
    char name[255];
    bool gameIsStarted;
public:
    Room(int maxPlayers, char * name);
    
    void addClient(Client * client);
    void deleteClient(Client * client);
    std::unordered_set<Client*> getClients();
    char  * getName();
    int getMaxPlayers();
    bool ifGameStarted();
    bool clientInRoom(Client * client);
    void sendMessage(char * buffer, int count);
};