// swift-tools-version:5.0

import PackageDescription

let package = Package(
    name: "SwiftCellsAndStuff",
    products: [
        .library(
            name: "SwiftCellsAndStuff",
            targets: ["SwiftCellsAndStuff"]
        ),
    ],
    dependencies: [],
    targets: [
        .target(name: "SwiftCellsAndStuff",
            path: "./Sources"
        ),
    ]
)