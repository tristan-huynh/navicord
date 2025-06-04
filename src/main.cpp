#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>

#include "../discord/header/discord.h"
#include "fmanager.h"

using namespace std;

#ifdef DEBUG_MODE
#define DEBUG(msg) cerr << "DEBUG: " << msg << endl;
#else
#define DEBUG(msg)
#endif

namespace {
	volatile bool interrupted = false;
}

struct DiscordState
{
	discord::User currentUser;
	std::unique_ptr<discord::Core> core;
};


const uint64_t APP_ID = 1377436019292504146;

int main(int, char**)
{	
	fmanager filemanager("./config.yaml");
	filemanager.parseYAML();


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
	activity.SetState("Using Navicord");
	activity.SetDetails("Developed by Navicord Team");
	//activity.GetAssets().SetLargeImage("navicord_large_image");
	//activity.GetAssets().SetLargeText("Navicord - A Discord Client for Navigation");
	activity.SetType(discord::ActivityType::Playing);

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
