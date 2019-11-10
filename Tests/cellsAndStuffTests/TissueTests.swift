import XCTest
import cellsAndStuff

class TissueTests: XCTestCase {

    static var allTests = [
        ("Send Stimulation Into the Network", testStimulateTheNetwork),
        ("Receive callbacks after stimulation", testGetCallbackAfterStimulatingTheNetwork),
        ("Feedforward stimulation", testFeedforwardStimulatingTheNetwork),
        ("Tissue State Callbacks", testReceiveTissueStateAfterNetworkStimulation),
        ("Get number of cells in the Tissue", testCountCellsInTissue),
        ("Set a cell's cell type", testSetCellTypeOfCell),
        ("Prevent setting a cell type to something un-supported by the tissue manager", testPreventsSettingCellTypeToAnUnsupportedCellType),
    ]

    func testSetCellTypeOfCell() throws {
        let tissueManager = TissueManager()
        TissueManager.resetTissue()

        let index: Int32 = 4

        //  Revert cell types to their previous
        defer {
            TissueManager.resetTissue()
        }

        guard var cell = tissueManager.cell(at: index) else {
            XCTFail("Could not get expected cell")
            return
        }
        
        XCTAssertEqual(cell.type.id, DefaultCellTypes.basic.id, "Unexpected default cell type ID")

        try tissueManager.setType(ofCellAtIndex: index, to: DefaultCellTypes.inhibitory)

        guard var updatedCell = tissueManager.cell(at: index) else {
            XCTFail("Could not get expected cell")
            return
        }
        XCTAssertEqual(updatedCell.type.id, DefaultCellTypes.inhibitory.id, "Cell's type was not updated")
        
    }

    func testPreventsSettingCellTypeToAnUnsupportedCellType() {
        let tissueManager = TissueManager(cellTypes: UnitTestCellTypes())
        TissueManager.resetTissue()

        let index: Int32 = 4

        //  Revert cell types to their previous
        defer {
            TissueManager.resetTissue()
        }

        XCTAssertThrowsError(try tissueManager.setType(ofCellAtIndex: index, to: DefaultCellTypes.inhibitory)) { error in 
            print(error)
        }

        XCTAssertThrowsError(try tissueManager.setType(ofCellAtIndex: index, to: DefaultCellTypes.basic)) { error in 
            print(error)
        }
    }

    func testStimulateTheNetwork() {
        
        let tissueManager = TissueManager()
        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 3, strength: 0.6)
        tissueManager.connectCell(from: 2, to: 3, strength: 0.99)
        
        var targets: [Int32] = [0, 1, 2]

        var strengths = [0.2, 0.9, 1.77722]

        tissueManager.stimulate(cellIndexes: &targets[0], strengths: &strengths[0], count: Int32(targets.count))

    }

    func testCountCellsInTissue() {
        let tissueManager = TissueManager()

        //  See also NUM_CELLS def in tissue.h
        XCTAssertEqual(100, tissueManager.numCells)
    }

    func testGetCallbackAfterStimulatingTheNetwork() {
        let tissueManager = TissueManager()
        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 3, strength: 0.6)
        tissueManager.connectCell(from: 2, to: 3, strength: 0.99)

        let cell = tissueManager.cell(at: 0)!

        let loggingLogic = LoggingCellTypeLogic()

        cell.type.setLogic(to: loggingLogic)

        var targets: [Int32] = [0, 1, 2]

        var strengths = [0.2, 0.9, 1.77722]

        tissueManager.stimulate(cellIndexes: &targets[0], strengths: &strengths[0], count: Int32(targets.count))

        XCTAssertFalse(loggingLogic.outputStrengthComputations.isEmpty, "System callback integration expected")
        XCTAssertFalse(loggingLogic.incomingConnectionStateCalls.isEmpty, "System callback integration expected")
        XCTAssertFalse(loggingLogic.outgoingConnectionStateCalls.isEmpty, "System callback integration expected")

    }

    func testFeedforwardStimulatingTheNetwork() {
        let tissueManager = TissueManager()

        TissueManager.resetTissue()

        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 4, strength: 0.5)
        tissueManager.connectCell(from: 3, to: 5, strength: 0.99)
        tissueManager.connectCell(from: 4, to: 5, strength: -0.98)

        let cell = tissueManager.cell(at: 0)!

        let loggingLogic = LoggingCellTypeLogic()

        cell.type.setLogic(to: loggingLogic)

        let targets = [0, 1, 2]
        let strengths = [0.2, 0.9, 1.77722]

        tissueManager.feedforwardStimulate(cellIndexes: targets, strengths: strengths)

        loggingLogic.logConnectionCalls()

        XCTAssertFalse(loggingLogic.incomingConnectionStateCalls.isEmpty, "System callback integration expected")
        XCTAssertFalse(loggingLogic.outgoingConnectionStateCalls.isEmpty, "System callback integration expected")

        //  Validate connection logic
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls.count, 3)
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[0].index, 5)
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[1].index, 3)
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[2].index, 4)

        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[0].incomingIndexes, [3, 4])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[1].incomingIndexes, [0])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[2].incomingIndexes, [1])

        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[0].incomingStrengths, [0.99, -0.98])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[1].incomingStrengths, [0.5])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[2].incomingStrengths, [0.5])

        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls.count, 4)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[0].index, 3)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[1].index, 4)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[2].index, 0)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[3].index, 1)

        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[0].outgoingStrengths, [0.99])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[1].outgoingStrengths, [-0.98])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[2].outgoingStrengths, [0.5])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[3].outgoingStrengths, [0.5])

        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[0].outgoingIndexes, [5])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[1].outgoingIndexes, [5])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[2].outgoingIndexes, [3])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[3].outgoingIndexes, [4])
    }

    func testReceiveTissueStateAfterNetworkStimulation() {
        let tissueManager = TissueManager()

        TissueManager.resetTissue()

        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 4, strength: 0.5)
        tissueManager.connectCell(from: 3, to: 5, strength: 0.99)
        tissueManager.connectCell(from: 4, to: 5, strength: -0.98)

        let cell = tissueManager.cell(at: 0)!

        let loggingLogic = LoggingCellTypeLogic()

        cell.type.setLogic(to: loggingLogic)

        let targets = [0, 1, 2]
        let strengths = [0.2, 0.9, 1.77722]

        var callbackReceived = false
        tissueManager.setTissueStateCallback { state in 
            callbackReceived = true

            print("Callback received:  Data:  \(state.outputIndexes), \(state.outputStrengths)")

        }

        tissueManager.feedforwardStimulate(cellIndexes: targets, strengths: strengths)

        XCTAssertTrue(callbackReceived, "System should have returned tissue state after stimulation")
    }

}