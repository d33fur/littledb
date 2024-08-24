#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "database.h"

void handleClient(int clientSocket, Database& db);

#endif // CLIENT_HANDLER_H
