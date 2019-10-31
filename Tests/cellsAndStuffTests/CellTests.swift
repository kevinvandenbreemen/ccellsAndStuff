import XCTest
import cellsAndStuff

class CellTests: XCTestCase {

    static var allTests = [
        ("Get Cell from the Network", testGetCellFromNetwork),
        ("Get index of Cell", testCellHasCellIndex),
    ]

    func testGetCellFromNetwork() {
        let tissueManager = TissueManager()

        guard let cell = tissueManager.cell(at: 0) else {
            XCTFail("Could not get cell from tissue")
            return
        }

        XCTAssertEqual(cell.type.id, DefaultCellTypes.basic.rawValue)

    }

    func testCellHasCellIndex() {
        let tissueManager = TissueManager()

        guard let cell = tissueManager.cell(at: 2) else {
            XCTFail("Could not get cell from tissue")
            return
        }

        XCTAssertEqual(2, cell.index, "Cell in tissue should have index")
    }

}