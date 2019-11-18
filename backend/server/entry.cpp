#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>

#include "Server.h"
#include "round/Game.h"
#include "round/Player.h"
#include "round/StockGenerator.h"

Server server(2878);
Game game;

/*
====================================================
====	Network Communication
====================================================
*/
void * asyncSend(void *raw) {
	struct Server::Connector *args = (struct Server::Connector *) raw;
	for(;;) {
		usleep(5000000);
		std::cout << "sending woohoo" << std::endl;
		server.sendMessage(*args, "woohoo");
	}
	free(args);
}

void * listenBlocking(void *raw) {
	for(;;)
		server.loop();
}
void onMessage(uint16_t df, char *buffer) {
	std::cout << "[" << df << "]\t<" << buffer << ">" << std::endl; 
	std::string msg(buffer);
	if (msg.size() >= 4 && msg.substr(0,5) == "LOGIN") {
		game.clientLogin(msg.substr(5,msg.size()-1));
	} else if (msg.size() > 4 && msg.substr(0,4) == "DONE") {
		msg.erase(0,4);
		std::string buf;
		std::stringstream ss(msg);
		std::string username;
		ss >> username;
		double balance;
		ss >> balance;
		Player* p = game.getPlayer(username);
		if (p) {
			p->balance = balance;
		} else {
			std::cout << "Failed to find player [" << username << "]" << std::endl;
		}
	} else {
		std::cout << "No action taken." << std::endl;
	}
	// server.sendMessage((struct Server::Connector){.source_fd=df}, "yeet");
}

void onDisconnect(uint16_t df) {
	std::cout << df << " disconnected." << std::endl;
}

void onConnect(uint16_t df) {
	std::cout << df << " connected." << std::endl;
	// pthread_t t;
	// struct Server::Connector *args = (Server::Connector *) malloc(sizeof(struct Server::Connector));
	// args->source_fd=df;
	// if (pthread_create(&t, NULL, asyncSend, (void * ) args) != 0){
	// 	printf("shit's fucked yo\n");
	// }
}


void * setupServer(void* raw) {
	std::cout << "Launching server" << std::endl;
	std::cout << "Binding socket events...";
	{
		server.onConnect(&onConnect);
		server.onDisconnect(&onDisconnect);
		server.onInput(&onMessage);
	}
	std::cout << "\t\tOK!" << std::endl;
	std::cout << "Binding socket to port...";
	{
		server.init();
	}
	std::cout << "\t\tOK!" << std::endl;
	std::cout << "Creating socket background thread...";
	{
		// pthread_t t;
		// if (pthread_create(&t, NULL, listenBlocking, NULL) != 0) {
		// 	std::cout << "FAIL!" << std::endl;
		// 	return 1;
		// }	
		listenBlocking(nullptr);
	}
	std::cout << "\tOK!" << std::endl;
	std::cout << "Server launched successfully!" << std::endl;
}

/*
====================================================
====	Game Logic
====================================================
*/
void addPlayer(std::string username) {
	game.clientLogin(username);
}



void roundLoop() {
	for (;;) {
		// WARMUP
		{
			std::cout << "Warmup started" << std::endl;
			game.roundsPlayed++;
			game.roundStatus = "warmup";
		}

		// BEGIN
		{
			for(;game.players.size()==0;) {
				std::cout << "No players in the game! Waiting for more..." << std::endl;
				this_thread::sleep_for(chrono::seconds(5));
			}
			std::cout << "Beginning round" << std::endl;
			// this_thread::sleep_for(chrono::seconds(2));
			game.roundStatus = "begin";
			for (auto& player : game.players)
				player.status = "WAITING";
		}

		// WAIT (BLOCKING) 
		{
			bool done;
			const int timeout = 30;
			for (int roundTimer = 0; !done || roundTimer > timeout; roundTimer++) {
				// std::cout << "Waiting for all players to respond, " << (timeout - roundTimer) << "s remaining." << std::endl;
				this_thread::sleep_for(chrono::seconds(1));
				done=true;
				for (auto& player : game.players)
					if (player.status != "FINISHED")
						done=false;
			}
		}

		// END ROUND
		{
			std::cout << "Round over!" << std::endl;
			for (auto& player : game.players) {
				std::cout << "<" << player.name << "> $" << player.balance << std::endl;
				player.status = "READY";
			}
		}
	}
}


/*
====================================================
====	Entrypoint
====================================================
*/
int main() {
	pthread_t t;
	if (pthread_create(&t, NULL, setupServer, NULL) != 0) {
		std::cout << "FAIL!" << std::endl;
		return 1;
	}
	//setupServer();

	roundLoop();

	return 0;
}