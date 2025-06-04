#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>

#include "../lib/discord_game_sdk/include/discord.h"
#include "fmanager.hpp"
#include "nrestclient.hpp"


using namespace std;

namespace {
	volatile bool interrupted = false;
}

struct DiscordState
{
	discord::User currentUser;
	std::unique_ptr<discord::Core> core;
};


//const uint64_t APP_ID = 1377436019292504146;

int main(int, char**)
{	
	fmanager filemanager("../config.yaml");
	filemanager.parseYAML();

	const uint64_t APP_ID = std::stoull(filemanager.getValue("app_id"));
	string baseUrl = filemanager.getValue("base_url");
	string username = filemanager.getValue("username");
	string password = filemanager.getValue("password");
	nrestclient apiClient(baseUrl, username, password);

	discord::Core* core{};
	auto result = discord::Core::Create(APP_ID, DiscordCreateFlags_Default, &core);
	if (result != discord::Result::Ok || !core) {
		cerr << "Failed to create Discord core";
		return -1;
	}
	discord::Core* discordCore = core;
	discordCore->SetLogHook(discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
		std::cerr << "Log(" << static_cast<uint32_t>(level) << "): " << message << "\n";
		});

	discord::Activity activity{};
	activity.SetName("Listening to Navicord");
	activity.SetDetails("Song name");
	if (apiClient.getAlbumArtUrl().compare("none") == 0) {
		activity.GetAssets().SetLargeImage("navidrome-512");
		activity.GetAssets().SetLargeText("Navicord - A Discord Client for Navigation");

	} else {
		activity.GetAssets().SetLargeImage(apiClient.getAlbumArtUrl().c_str());
		activity.GetAssets().SetLargeText("Navicord - A Discord Client for Navigation");
		activity.GetAssets().SetSmallImage("navidrome-512");
		activity.GetAssets().SetSmallText("Navicord");
	}
	activity.SetType(discord::ActivityType::Listening);

	discordCore->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
		std::cout << ((result == discord::Result::Ok) ? "Rich presence updated\n" : "Failed updating rich presence.\n");
	});
	
	std::signal(SIGINT, [](int) { interrupted = true;});

	do {
		discordCore->RunCallbacks();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} while (!interrupted);

	return 0;
}
