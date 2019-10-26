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
        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 3, strength: 0.6)
        tissueManager.connectCell(from: 2, to: 3, strength: 0.99)

        let cell = tissueManager.cell(at: 0)!

        let loggingLogic = LoggingCellTypeLogic()

        cell.type.setLogic(to: loggingLogic)

        var targets: [Int32] = [0, 1, 2]

        var strengths = [0.2, 0.9, 1.77722]

        tissueManager.feedforwardStimulate(cellIndexes: &targets[0], strengths: &strengths[0], count: Int32(targets.count))

        XCTAssertFalse(loggingLogic.outputStrengthComputations.isEmpty, "System callback integration expected")
        XCTAssertFalse(loggingLogic.incomingConnectionStateCalls.isEmpty, "System callback integration expected")
        XCTAssertFalse(loggingLogic.outgoingConnectionStateCalls.isEmpty, "System callback integration expected")

    }

}