import cCellsAndStuff

public struct Cell {

    public var outgoingConnections: [(index: Int32, strength: Double)]? {
        
        get {
            
            guard let connectionInfoPtr = cCellsAndStuff.swift_cells_getConnectedFrom(self.index) else {
                print("No connection pointer")
                return nil
            }

            let connectionInfo = connectionInfoPtr.pointee

            guard 
                connectionInfo.numConnections > 0, 
                let destIndexes = connectionInfo.cellIndexes, 
                let connectionStrengths = connectionInfo.connectionStrengths 
            else {
                return nil
            }

            var connectionsList: [(index: Int32, strength: Double)] = []
            for i in 0..<connectionInfo.numConnections {
                let index = (destIndexes + Int(i)).pointee
                let strength = (connectionStrengths + Int(i)).pointee
                connectionsList.append( (index: index, strength: strength) )
            }

            return connectionsList

        }
        
    }

    public let type: CellType
    public let index: Int32
    
}