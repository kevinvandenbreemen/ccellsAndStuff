import cCellsAndStuff

public enum TissueManagementError: Error {

    /// Indicates that caller tried to set cell type to something unsupported by tissue manager.
    /// 
    /// Includes associated values as follows:
    ///
    /// Supported CellTypes implementation
    ///
    /// Actual CellTypes implementation passed (see setType() method)
    case unsupportedCellType(String, String)
}

public class TissueManager {

    public var numCells: Int32 {
        get { 
            cCellsAndStuff.swift_tissue_getNumCells() 
        }
    }

    private static var tissueStateCallback: (_ tissueState: CurrentTissueState) -> Void = {tissueState in 
    }
    private static var tissueStateCallbackFromC: TissueStateCallback = TissueStateCallback(
        onStateUpdate: {(tissueState) in 
            guard let tissueState = tissueState else {
                return
            }

            let state = tissueState.pointee
            guard let rawIndexes = state.outputIndices, let rawStrengths = state.outputStrengths else {
                return
            }

            var outputIndexes: [Int32] = []
            var outputStrengths: [Double] = []
            for i in 0..<state.outputCount {
                outputIndexes.append( (rawIndexes + Int(i)).pointee )
                outputStrengths.append( (rawStrengths + Int(i)).pointee )
            }

            TissueManager.tissueStateCallback(CurrentTissueState(outputIndexes: outputIndexes, outputStrengths: outputStrengths))
        }
    )

    private let cellTypes: CellTypes

    public static func resetTissue() {
        cCellsAndStuff.swift_tissue_resetAll();
    }
    
    public init(cellTypes: CellTypes = DefaultCellTypesProvider()) {
        self.cellTypes = cellTypes
    }
    

    public func initializeDefaultTissue() {
        cCellsAndStuff.swift_tissue_initializeDefaultTissue()
    }

    public func connectCell(from: Int32, to: Int32, strength: Double) {
        let result = cCellsAndStuff.swift_cells_connectDirected(from, to, strength)
        if result != 0 {
            print("Failed to Connect cells - see log for details")
        }
    }

    public func cell(at index: Int32) -> Cell? {

        let cellTypeRaw = cCellsAndStuff.swift_tissue_getCellType(index)
        guard let cellType = cellTypes.cellType(byID: cellTypeRaw) else {
            return nil
        }

        return Cell(type: cellType, index: index)

    }

    public func setType(ofCellAtIndex cellIndex: Int32, to cellType: CellType) throws {
        guard let proposedCellType = cellTypes.cellType(byID: cellType.id) else {
            throw TissueManagementError.unsupportedCellType(String(describing: type(of: self.cellTypes)), String(describing: type(of: cellType)))
        }
        
        cCellsAndStuff.swift_tissue_setCellType(cellIndex, cellType.id)
    }

    public func stimulate(cellIndexes indexes: UnsafeMutablePointer<Int32>, strengths: UnsafeMutablePointer<Double>, count: Int32) {

        cCellsAndStuff.swift_cells_stimulate(indexes, strengths, count)
        
    }

    public func feedforwardStimulate(cellIndexes indexes: [Int], strengths: [Double]) {

        var intArray: [Int32] = []
        for i in indexes {
            intArray.append(Int32(i))
        }

        var strArray: [Double] = strengths

        cCellsAndStuff.swift_cells_matrix_feedfoward_stim(&intArray[0], &strArray[0], Int32(indexes.count))
    }

    public func setTissueStateCallback(_ callback: @escaping (_ tissueState: CurrentTissueState) -> Void) {
        TissueManager.tissueStateCallback = callback
        cCellsAndStuff.swift_tissue_setOnStateUpdate(&TissueManager.tissueStateCallbackFromC)
    }

}