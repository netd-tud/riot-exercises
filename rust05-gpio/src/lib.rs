// SPDX-FileCopyrightText: Christian Amsüss <chrysn@fsfe.org>
// SPDX-License-Identifier: Apache-2.0 OR MIT
#![no_std]

use riot_wrappers::riot_main;
use riot_wrappers::println;
use riot_wrappers::ztimer::Clock;
use core::time::Duration;

// [TASK 1: Add convenience `use` line]

// [TASK 2: Add convenience `use` line]

extern crate rust_riotmodules;

riot_main!(main);

// [TASK 2: Define the interrupt's struct]

fn main() {
    // Startup delay to ensure the terminal is connected
    Clock::sec().sleep_extended(Duration::from_secs(5));

    println!("GPIOs example.");

    // [TASK 1: Initialize led0 here]
    
    // [TASK 2: Initialize led1 and button here]
    
    // [TASK 1: Loop here]
}

// [TASK 2: Define button_callback here]
