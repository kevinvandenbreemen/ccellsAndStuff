import XCTest
import cellsAndStuff

class CellTests: XCTestCase {

    static var allTests = [
        ("Get Cell from the Network", testGetCellFromNetwork),
    ]

    func testGetCellFromNetwork() {
        let tissueManager = TissueManager()

        guard let cell = tissueManager.cell(at: 0) else {
            XCTFail("Could not get cell from tissue")
            return
        }

        XCTAssertEqual(cell.type.id, DefaultCellTypes.basic.rawValue)

    }

}