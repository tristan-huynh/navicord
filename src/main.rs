use discord_game_sdk::{Activity, Discord};
use std::{thread, time};

fn main() {
    // Replace with your actual Discord App ID (u64)
    let app_id: u64 = 123456789012345678;
    let mut discord = Discord::new(app_id).expect("Failed to initialize Discord");

    let mut activity = Activity::new();
    activity.set_state("Exploring Rust");
    activity.set_details("Creating a Discord Rich Presence");
    activity.set_start_time(Some(std::time::SystemTime::now()));
    
    // Set large image key and text (make sure these values match your Discord application assets)
    {
        let assets = activity.assets_mut();
        assets.large_image = Some("rust_large".to_string());
        assets.large_text = Some("Rust Programming".to_string());
    }

    // Update the activity (rich presence)
    discord
        .activity
        .update_activity(activity, |result| match result {
            Ok(_) => println!("Activity updated successfully!"),
            Err(error) => eprintln!("Failed to update activity: {:?}", error),
        });

    // Main loop: run callbacks to handle Discord events
    loop {
        discord.run_callbacks();
        thread::sleep(time::Duration::from_millis(100));
    }
}