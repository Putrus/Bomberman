#include "Room.h"


Room::Room(int maxPlayers, char * name)
{
    this->maxPlayers = maxPlayers;
    memset(this->name, 0, 255);
    strncpy(this->name, name,strlen(name));
}



void Room::addClient(Client * client)
{
    clients.insert(client);
}


void Room::deleteClient(Client * client)
{
    clients.erase(client);
}


std::unordered_set<Client*> Room::getClients()
{
    return clients;
}

char  * Room::getName()
{
    return name;
}

int Room::getMaxPlayers()
{
    return maxPlayers;
}