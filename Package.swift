// swift-tools-version:4.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "ttytalk",
    products: [
        // Products define the executables and libraries produced by a package, and make them visible to other packages.
        .library(
            name: "ttytalk",
            targets: ["ttytalk"]),
    ],
    dependencies: [
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages which this package depends on.
        .target(
            name: "ttycall",
            dependencies: []),
        .target(
            name: "ttytalk",
            dependencies: ["ttycall"]),
        .testTarget(
            name: "ttytalkTests",
            dependencies: ["ttytalk"]),
    ]
)
