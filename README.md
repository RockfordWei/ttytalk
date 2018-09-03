# ttytalk

A simple tty device access interface in Swift 4.1


## Quick Start

Add `.package("https://github.com/RockfordWei/ttytalk.git", from: "0.0.0")` to your `Package.swift`

``` swift

import ttytalk

// initialize a device
let tty = TTY("/dev/cu.usbmodem1411")

// this call will block thread until a buffer fully return.
let info = try tty.callText("AT\r\n")

// info should be `OK` if it was a regular modem
print(info)
```

## Binary Traffic

Similiar to the `callText` function, there is also an interface for binary traffics:

``` swift
let input:[UInt8] = ...
let output:[UInt8] = try tty.call(input)
```
