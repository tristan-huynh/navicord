#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>

#include "../lib/discord_game_sdk/include/discord.h"
#include "fmanager.hpp"
//#include "nrestclient.hpp"


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
	fmanager fman("../config.yml");
	fman.parseYAML();

	const uint64_t APP_ID = std::stoull(fman.getValue("appid"));
	string baseUrl = fman.getValue("serverurl");
	string username = fman.getValue("serverusername");
	string password = fman.getValue("serverpassword");
	//nrestclient apiClient(baseUrl, username, password);
	cout << password << endl;

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

	//auto currentTrack = apiClient.getCurrentTrack();
	discord::Activity activity{};
	//if (currentTrack.has_value()) {
		// // Update activity details based on the current track.
		// activity.SetDetails((std::string("Playing: ") + currentTrack->title).c_str());
		// activity.GetAssets().SetLargeImage("navidrome-512");
		// activity.GetAssets().SetLargeText((std::string("Now playing: ") + currentTrack->artist).c_str());
		// activity.GetAssets().SetSmallImage("navidrome-512");
		// activity.GetAssets().SetSmallText(currentTrack->album.c_str());
	//} else {
        // Fallback if no track is playing
        activity.SetDetails("No track playing");
        activity.GetAssets().SetLargeImage("navidrome-512");
        activity.GetAssets().SetLargeText("Navicord");
        // activity.GetAssets().SetSmallImage("navidrome-512");
        // activity.GetAssets().SetSmallText("Navicord");
    //}
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
