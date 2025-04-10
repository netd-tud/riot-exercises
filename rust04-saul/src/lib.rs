// SPDX-FileCopyrightText: Christian Amsüss <chrysn@fsfe.org>
// SPDX-License-Identifier: Apache-2.0 OR MIT
#![no_std]

use riot_wrappers::riot_main;
use riot_wrappers::println;
use riot_wrappers::ztimer::Clock;
use core::time::Duration;
use riot_wrappers::saul::{RegistryEntry, Class, SensorClass, Unit};

extern crate rust_riotmodules;

riot_main!(main);

const TEMPERATURE_THRESHOLD: i16 = 2400; // °C * 10^-2

fn main() {
    // Startup delay to ensure the terminal is connected
    Clock::sec().sleep_extended(Duration::from_secs(5));

    use switch_hal::OutputSwitch;
    let mut led0 = riot_wrappers::led::LED::<0>::new_checked().expect("Our board has an LED0");

    let temp_sensor = RegistryEntry::all()
        .filter(|e| matches!(e.type_(), Some(Class::Sensor(Some(SensorClass::Temp)))))
        .next()
        .expect("No temperature sensor present");

    println!("Found temperature device: {}", temp_sensor.name().unwrap_or("(unnamed)"));

    loop {
        let temperature = temp_sensor
            .read()
            .expect("Error reading temperature");

        println!("Read value: {:?}", temperature);

        assert!(matches!(temperature.unit(), Some(Unit::TempC)));
        assert_eq!(temperature.scale(), -2);

        if temperature.value()[0] > TEMPERATURE_THRESHOLD {
            led0.on().unwrap();
        } else {
            led0.off().unwrap();
        }

        Clock::msec().sleep_extended(Duration::from_millis(500));
    }
}
