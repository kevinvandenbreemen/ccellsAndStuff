import XCTest
import cellsAndStuff

class TissueTests: XCTestCase {

    static var allTests = [
        ("Send Stimulation Into the Network", testStimulateTheNetwork),
        ("Receive callbacks after stimulation", testGetCallbackAfterStimulatingTheNetwork),
        ("Feedforward stimulation", testFeedforwardStimulatingTheNetwork),
    ]

    func testStimulateTheNetwork() {
        
        let tissueManager = TissueManager()
        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 3, strength: 0.6)
        tissueManager.connectCell(from: 2, to: 3, strength: 0.99)
        
        var targets: [Int32] = [0, 1, 2]

        var strengths = [0.2, 0.9, 1.77722]

        tissueManager.stimulate(cellIndexes: &targets[0], strengths: &strengths[0], count: Int32(targets.count))

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

        var targets: [Int32] = [0, 1, 2]

        var strengths = [0.2, 0.9, 1.77722]

        tissueManager.feedforwardStimulate(cellIndexes: &targets[0], strengths: &strengths[0], count: Int32(targets.count))

        XCTAssertFalse(loggingLogic.incomingConnectionStateCalls.isEmpty, "System callback integration expected")
        XCTAssertFalse(loggingLogic.outgoingConnectionStateCalls.isEmpty, "System callback integration expected")

        //  Validate connection logic
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls.count, 3)
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[0].index, 3)
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[1].index, 4)
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[2].index, 5)

        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[0].incomingIndexes, [0])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[1].incomingIndexes, [1])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[2].incomingIndexes, [3, 4])

        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[0].incomingStrengths, [0.5])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[1].incomingStrengths, [0.5])
        XCTAssertEqual(loggingLogic.incomingConnectionStateCalls[2].incomingStrengths, [0.99, -0.98])

        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls.count, 3)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[0].index, 0)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[1].index, 1)
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[2].index, 3)

        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[0].outgoingStrengths, [0.5])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[1].outgoingStrengths, [0.5])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[2].outgoingStrengths, [0.99])

        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[0].outgoingIndexes, [3])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[1].outgoingIndexes, [4])
        XCTAssertEqual(loggingLogic.outgoingConnectionStateCalls[2].outgoingIndexes, [5])
    }

}