#include "Room.h"


Room::Room(int maxPlayers, char * name)
{
    this->maxPlayers = maxPlayers;
    memset(this->name, 0, 255);
    strncpy(this->name, name,strlen(name));
    this->gameIsStarted = false;
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

bool Room::ifGameStarted()
{
    return gameIsStarted;
}

bool Room::clientInRoom(Client * client)
{
    for(Client * c : clients)
    {
        if(c == client)
        {
            return true;
        }
    }
    return false;
}

void Room::sendMessage(char * buffer, int count)
{
    for(Client * c : clients)
    {
        c->write(buffer,count);
    }
}