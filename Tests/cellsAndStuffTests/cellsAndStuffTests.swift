import XCTest
@testable import cellsAndStuff

final class cellsAndStuffTests: XCTestCase {
    func testCCodebaseVersion() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct
        // results.

        let sut = CellsAndStuffSystemAbout()
        let version = sut.versionNum()

        XCTAssertEqual(CellsAndStuffSystemAbout().getAbout(), "C Cells and Stuff Integration Lib:  v.\(version)")
    }

    static var allTests = [
        ("Get C Versioning", testCCodebaseVersion),
    ]
}
