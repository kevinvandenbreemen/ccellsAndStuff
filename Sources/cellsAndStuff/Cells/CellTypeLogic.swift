import cCellsAndStuff

class CellTypeLogicRegistry {

    private static var cellTypeToLogic: [Int32: CellTypeLogic] = [:]

    static func registerCellTypeLogic(cellTypeID: Int32, logic: CellTypeLogic) {
        CellTypeLogicRegistry.cellTypeToLogic[cellTypeID] = logic
    }

    static func getLogic(for cellTypeID: Int32) -> CellTypeLogic? {
        return CellTypeLogicRegistry.cellTypeToLogic[cellTypeID]
    }

}

/**
 * Logic you can add for cell types
 */
public protocol CellTypeLogic {

    func computeOutputStrength(givenInput input: Double, givenOutputSum output: Double) -> Double
    func handleIncomingConnectionState(cellIndex index: Int32, incomingIndexes: [Int32], incomingStrengths: [Double])
    func handleOutgoingConnectionState(cellIndex index: Int32, outgoingIndexes: [Int32], outgoingStrengths: [Double])
    func calculateActivation(weightedInputSum: Double) -> Double

    /// Logic for a cell.  Generally you should put most of your logic for cells in this method and then regularly call
    /// TissueManager.executeCellBehaviours() to drive cell logic throughout your network!
    func executeCellLogic(cellIndex index: Int32)

}