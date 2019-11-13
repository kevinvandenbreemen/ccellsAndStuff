import cellsAndStuff

class LoggingCellTypeLogic: CellTypeLogic {

    private(set) var outputStrengthComputations: [(input: Double, output: Double)] = []
    private(set) var incomingConnectionStateCalls: 
        [(index: Int32, incomingIndexes: [Int32], incomingStrengths: [Double])] = []
    private(set) var outgoingConnectionStateCalls: 
        [(index: Int32, outgoingIndexes: [Int32], outgoingStrengths: [Double])] = []
    private(set) var cellLogicCalls: [Int32] = []

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

    func calculateActivation(weightedInputSum: Double) -> Double {
        return weightedInputSum
    }

    func executeCellLogic(cellIndex index: Int32) {
        cellLogicCalls.append(index)
    }

    func logConnectionCalls() {

        var incomingConnectionsDebug = ""
        incomingConnectionStateCalls.forEach{ call in 
            incomingConnectionsDebug += "\(call)\n"
        }

        var outgoingConnectionsDebug = ""
        outgoingConnectionStateCalls.forEach{ call in 
            outgoingConnectionsDebug += "\(call)\n"
        }

        print(
"""
Logging Cell Logic Data Acquired:
outputStrengthComputations:\t\(outputStrengthComputations)
incomingConnectionStateCalls:\n\(incomingConnectionsDebug)
outgoingConnectionStateCalls:\n\(outgoingConnectionsDebug)
"""
        )

        // var ret = "Logging Cell Logic Data Acquired:\n"
        // ret += "outputStrengthComputations:  \(outputStrengthComputations)\n"

    }

}