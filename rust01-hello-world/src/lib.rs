// SPDX-FileCopyrightText: Christian Amsüss <chrysn@fsfe.org>
// SPDX-License-Identifier: Apache-2.0 OR MIT
#![no_std]

use riot_wrappers::riot_main;
use riot_wrappers::println;
use riot_wrappers::ztimer::Clock;

extern crate rust_riotmodules;

riot_main!(main);

fn main() {
    // Startup delay to ensure the terminal is connected
    Clock::sec().sleep_extended(core::time::Duration::from_secs(5));

    println!("Hello Rust!");
    println!("You are running RIOT on a(n) {} board.\n", riot_wrappers::BOARD);
}
