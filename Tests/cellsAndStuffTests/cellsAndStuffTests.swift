import XCTest
@testable import cellsAndStuff

final class cellsAndStuffTests: XCTestCase {
    func testCCodebaseVersion() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct
        // results.
        XCTAssertEqual(cellsAndStuff().text, "Hello, World!")
    }

    static var allTests = [
        ("Get C Versioning", testCCodebaseVersion),
    ]
}
