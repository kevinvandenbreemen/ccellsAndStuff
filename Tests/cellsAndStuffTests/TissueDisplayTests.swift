import XCTest
import cellsAndStuff

class TissueDisplayTests: XCTestCase {

    static var allTests = [
        ("Get tissue display", testGetBuildTissueDisplay),
        ("Display a Tissue", testDisplayTissue),
    ]

    func testGetBuildTissueDisplay() {
        TissueDisplayProviders.provider.getTissueDisplay()
    }

    func testDisplayTissue() {
        let tissueManager = TissueManager()
        TissueDisplayProviders.provider.getTissueDisplay().display(tissue: tissueManager)
    }

}