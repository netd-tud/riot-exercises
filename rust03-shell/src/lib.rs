// SPDX-FileCopyrightText: Christian Amsüss <chrysn@fsfe.org>
// SPDX-License-Identifier: Apache-2.0 OR MIT
#![no_std]

use riot_wrappers::riot_main;

use core::fmt::Write;

extern crate rust_riotmodules;

riot_main!(main);

fn main() {
    use riot_wrappers::shell::CommandList;
    riot_wrappers::shell::new()
        .run_forever();
}


riot_wrappers::static_command!(static_echo, "echo", "Echo message", echo);

pub fn echo<'a>(w: &mut impl Write, args: impl IntoIterator<Item=&'a str>) {
    let mut args = args.into_iter();
    let commandname = args.next().expect("argv is always present");

    let (Some(message), None) = (args.next(), args.next()) else {
        writeln!(w, "Usage: {commandname} <message>").unwrap();
        writeln!(w, r#"Note: to echo multiple words wrap the message in ""."#).unwrap();
        return;
    };

    writeln!(w, "{}", message).unwrap();
}
