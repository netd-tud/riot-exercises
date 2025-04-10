// SPDX-FileCopyrightText: Christian Amsüss <chrysn@fsfe.org>
// SPDX-License-Identifier: Apache-2.0 OR MIT
#![no_std]

use riot_wrappers::riot_main;
use riot_wrappers::println;
use riot_wrappers::ztimer::Clock;
use core::time::Duration;

// We are using the LED through a generic "on"/"off" interface.
use switch_hal::OutputSwitch;

extern crate rust_riotmodules;

riot_main!(main);

fn main() {
    // Startup delay to ensure the terminal is connected
    Clock::sec().sleep_extended(Duration::from_secs(5));

    println!("This is a timers example");

    // We can use sleep to just get delays
    Clock::sec().sleep_extended(Duration::from_secs(2));

    // Many times can be expressed on different timers, but usable ranges and precision vary.
    Clock::msec().sleep_extended(Duration::from_secs(2));

    println!("Timeout!");

    let mut led0 = riot_wrappers::led::LED::<0>::new_checked().expect("Our board has an LED0");


    for _ in 0..20 {
        // This blinks the LED in a 50% duty cycle once per second

        // Is "unwrap()" bad? No, we know that our LEDs do not fail.
        // In future Rust versions we can express this even more clearly
        // <https://github.com/rust-lang/rust/pull/122792>.
        led0.on().unwrap();
        Clock::msec().sleep_extended(Duration::from_millis(500));
        led0.off().unwrap();
        Clock::msec().sleep_extended(Duration::from_millis(500));
    }

    println!("Done!");
}
