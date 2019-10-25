import cCellsAndStuff

public class TissueManager {

    public func initializeDefaultTissue() {
        cCellsAndStuff.swift_tissue_initializeDefaultTissue()
    }

    public func connectCell(from: Int32, to: Int32, strength: Double) {
        let result = cCellsAndStuff.swift_cells_connectDirected(from, to, strength)
        if result != 0 {
            print("Failed to Connect cells - see log for details")
        }
    }

}