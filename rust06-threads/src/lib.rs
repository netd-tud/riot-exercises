// SPDX-FileCopyrightText: Christian Amsüss <chrysn@fsfe.org>
// SPDX-License-Identifier: Apache-2.0 OR MIT
#![no_std]

use core::time::Duration;
use riot_wrappers::println;
use riot_wrappers::riot_main;
use riot_wrappers::ztimer::Clock;

// We are using the LED through a generic "on"/"off" interface.
use switch_hal::OutputSwitch;

extern crate rust_riotmodules;

riot_main!(main);

fn main() {
    // Startup delay to ensure the terminal is connected
    Clock::sec().sleep_extended(Duration::from_secs(5));

    println!("Thread example.");

    /* [TASK 1: create the thread here] */

    let mut led0 = riot_wrappers::led::LED::<0>::new_checked().expect("Our board has an LED0");

    loop {
        println!(
            "Thread {}\n",
            riot_wrappers::thread::get_pid()
                .get_name()
                .unwrap_or("unnamed")
        );

        led0.on().unwrap();
        Clock::msec().sleep_extended(Duration::from_millis(100));
        led0.off().unwrap();
        Clock::msec().sleep_extended(Duration::from_millis(900));
    }
}
