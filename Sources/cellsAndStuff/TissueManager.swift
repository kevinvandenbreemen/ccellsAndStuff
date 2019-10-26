import cCellsAndStuff

public class TissueManager {

    private let cellTypes: CellTypes
    
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

        return Cell(type: cellType)

    }

    public func stimulate(cellIndexes indexes: UnsafeMutablePointer<Int32>, strengths: UnsafeMutablePointer<Double>, count: Int32) {

        print("This method still to be implemented.  Data input appears to be:")

        for i in 0..<count {
            print("target \((indexes+Int(i)).pointee) -> str=\((strengths+Int(i)).pointee)")
        }

        //cCellsAndStuff.swift_cells_stimulate(indexes, strengths, count)
        
    }

}