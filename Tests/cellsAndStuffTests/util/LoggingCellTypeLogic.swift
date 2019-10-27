import cellsAndStuff

class LoggingCellTypeLogic: CellTypeLogic {

    private(set) var outputStrengthComputations: [(input: Double, output: Double)] = []
    private(set) var incomingConnectionStateCalls: 
        [(index: Int32, incomingIndexes: [Int32], incomingStrengths: [Double])] = []
    private(set) var outgoingConnectionStateCalls: 
        [(index: Int32, outgoingIndexes: [Int32], outgoingStrengths: [Double])] = []

    func computeOutputStrength(givenInput input: Double, givenOutputSum output: Double) -> Double {

        outputStrengthComputations.append((input: input, output: output))

        return output
    }

    func handleIncomingConnectionState(cellIndex index: Int32, incomingIndexes: [Int32], incomingStrengths: [Double]) {
        print("Incoming state update for idx=\(index)\nIndxs:  \(incomingIndexes)\tstrs:\(incomingStrengths)")
        incomingConnectionStateCalls.append((index: index, incomingIndexes: incomingIndexes, incomingStrengths: incomingStrengths))
    }

    func handleOutgoingConnectionState(cellIndex index: Int32, outgoingIndexes: [Int32], outgoingStrengths: [Double]) {
        print("Outgoing state update for idx=\(index)\nIndxs:  \(outgoingIndexes)\tstrs:\(outgoingStrengths)")
        outgoingConnectionStateCalls.append((index: index, outgoingIndexes: outgoingIndexes, outgoingStrengths: outgoingStrengths))
    }

}