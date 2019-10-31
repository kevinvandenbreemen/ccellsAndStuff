import XCTest
import cellsAndStuff

class CellTests: XCTestCase {

    static var allTests = [
        ("Get Cell from the Network", testGetCellFromNetwork),
        ("Get index of Cell", testCellHasCellIndex),
        ("Get connections from a Cell", testGetCellsConnectedFromCell),
        ("Get connections when none defined", testGetCellsConnectedFromCellWhenNoneConnected),
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

    func testGetCellsConnectedFromCell() {
        let tissueManager = TissueManager()
        TissueManager.resetTissue()

        tissueManager.connectCell(from: 0, to: 1, strength: 0.4)
        tissueManager.connectCell(from: 0, to: 2, strength: 0.5)

        guard let cell = tissueManager.cell(at: 0) else {
            XCTFail("Could not get cell from tissue")
            return
        }

        guard let outgoingConnections = cell.outgoingConnections else {
            XCTFail("Expected cell to have connections")
            return
        }

        XCTAssertEqual(2, outgoingConnections.count, "Expect cell at index 0 to have 2 outgoing connections")
    }

    func testGetCellsConnectedFromCellWhenNoneConnected() {
        let tissueManager = TissueManager()
        TissueManager.resetTissue()

        guard let cell = tissueManager.cell(at: 0) else {
            XCTFail("Could not get cell from tissue")
            return
        }

        XCTAssertNil(cell.outgoingConnections, "Should be no out-going connections since none were defined")
    }

}