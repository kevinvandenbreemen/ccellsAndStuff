import cCellsAndStuff

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

    public func stimulate(cellIndexes indexes: UnsafeMutablePointer<Int32>, strengths: UnsafeMutablePointer<Double>, count: Int32) {

        cCellsAndStuff.swift_cells_stimulate(indexes, strengths, count)
        
    }

    public func feedforwardStimulate(cellIndexes indexes: UnsafeMutablePointer<Int32>, strengths: UnsafeMutablePointer<Double>, count: Int32) {
        cCellsAndStuff.swift_cells_matrix_feedfoward_stim(indexes, strengths, count)
    }

    public func setTissueStateCallback(_ callback: @escaping (_ tissueState: CurrentTissueState) -> Void) {
        TissueManager.tissueStateCallback = callback
        cCellsAndStuff.swift_tissue_setOnStateUpdate(&TissueManager.tissueStateCallbackFromC)
    }

}