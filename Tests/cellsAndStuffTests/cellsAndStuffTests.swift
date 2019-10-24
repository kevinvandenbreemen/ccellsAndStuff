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

    func testCreateTissue() {
        let sut = TissueManager()
        sut.initializeDefaultTissue()
    }

    func testConnectTissueCells() {
        let sut = TissueManager()

        sut.connectCell(from: 0, to: 10, strength: 3.2)
        sut.connectCell(from: 1, to: 10, strength: 3.2)
        sut.connectCell(from: 2, to: 10, strength: 3.2)
        sut.connectCell(from: 3, to: 10, strength: 3.2)
        sut.connectCell(from: 10, to: 2, strength: 3.2)
    }

    static var allTests = [
        ("Get C Versioning", testCCodebaseVersion),
        ("Initialize Tissue", testCreateTissue),
        ("Connect Cells", testConnectTissueCells),
    ]
}
