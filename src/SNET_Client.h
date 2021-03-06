#pragma once
#include "enet/enet.h"
#include <vector>
#include <unordered_map>
#include "SNET_Definitions.h"

class SNET_NetworkedPlayer;

class SNET_Client
{
private:
	std::string username = "Player";
	UINT16 clientID = 0;
	std::string serverIP;
	std::string servername = "";
	
	bool connected = false;
	bool inGame = false;

	ENetHost* host = nullptr;
	ENetPeer* serverPeer = nullptr;

	unsigned int playerDataReceivedDelay = 0;

	SNET_Func_PlayerConnection playerAddCallback = nullptr;
	SNET_Func_PlayerConnection playerRemoveCallback = nullptr;
	SNET_Func_GeneratePlayerData genPlayerDataCallback = nullptr;

	std::unordered_map<UINT16, SNET_NetworkedPlayer*> players;
	//std::unordered_map<UINT16, SNET_NetworkedEntity*> entities;

	std::vector<SNET_LeaderboardEntry> leaderboard;

	void ReceivedPacket(const ENetEvent* const event);
	void ReceivedConnectionPacket(const ENetPacket* const packet);
	void ReceivedEntityPacket(const ENetPacket* const packet);
	void ReceivedLeaderboardPacket(const ENetPacket* const packet);
	
	void OnDisconnect();

	void AddPlayer(SNET_Packet_UserInfo userInfo);
	void RemovePlayer(const UINT16 id);

public:
	SNET_Client() = default;
	~SNET_Client();

	int Initialise();
	void SetCallbacks(SNET_Func_PlayerConnection onPlayerAdd,
	SNET_Func_PlayerConnection onPlayerRemove,
	SNET_Func_GeneratePlayerData onGenPlayerData);
	
	// Whether the player is connected to the server
	bool IsConnected() const;
	// Whether the player has joined the game as a player on the server
	bool IsInGame() const;

	// Attempts to connect to a given ip string
	bool Connect(const std::string ipAddress);
	void Disconnect();

	// Check if a player with a given ID exists
	bool GetPlayerExists(const UINT16 id) const;
	SNET_NetworkedPlayer* GetPlayer(const UINT16 id);
	//SNET_NetworkedEntity* GetEntity(const UINT16 id);

	// Get the user ID of the client (as determined by the server)
	UINT16 GetClientID() const;

	std::string GetUsername() const;
	void SetUsername(const std::string name);

	// Number of network updates since the client received player data from the server
	unsigned int GetPlayerDataReceivedDelay() const;

	void Update();

	// Asks the server for updated leaderboard data
	void RequestLeaderboard();
	// Returns a reference to the last leaderboard data vector received from the server
	std::vector<SNET_LeaderboardEntry>& GetLeaderboard();
};