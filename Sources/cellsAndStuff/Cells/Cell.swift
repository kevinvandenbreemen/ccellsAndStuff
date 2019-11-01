import cCellsAndStuff

public struct Cell {

    /// Gets the cells that connect to this cell
    /// -   Returns: Raw pairings of cell index and strength of connection
    public var incomingConnections: [(index: Int32, strength: Double)]? {
        get {

            guard let connectionInfoPtr = cCellsAndStuff.swift_cells_connectedTo(self.index) else {
                return nil
            }

            return getConnectionTuples(connectionInfoPtr: connectionInfoPtr)
        }
    }

    /// Gets the cells from this cell, 
    /// - Returns: Raw pairings with cell index and the strength of the connection.
    public var outgoingConnections: [(index: Int32, strength: Double)]? {
        
        get {
            
            guard let connectionInfoPtr = cCellsAndStuff.swift_cells_getConnectedFrom(self.index) else {
                return nil
            }

            return getConnectionTuples(connectionInfoPtr: connectionInfoPtr)

        }
        
    }

    public let type: CellType
    public let index: Int32

    private func getConnectionTuples(connectionInfoPtr: UnsafeMutablePointer<CellConnectionInfo>) -> [(index: Int32, strength: Double)]? {
        let connectionInfo = connectionInfoPtr.pointee
            defer {
                connectionInfoPtr.deallocate()
            }

            guard 
                connectionInfo.numConnections > 0, 
                let destIndexes = connectionInfo.cellIndexes, 
                let connectionStrengths = connectionInfo.connectionStrengths 
            else {
                return nil
            }

            defer {
                connectionInfo.connectionStrengths?.deallocate() 
                connectionInfo.cellIndexes?.deallocate()
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