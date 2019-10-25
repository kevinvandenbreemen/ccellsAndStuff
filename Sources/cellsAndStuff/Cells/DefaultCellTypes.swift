public enum DefaultCellTypes: Int32, CellType {

    case basic = 1
    case inhibitory = 2

    public var id: Int32 { get {
            return self.rawValue
        }
    }

}

public class DefaultCellTypesProvider : CellTypes {
    
    public init() {
        
    }
    

    public func cellType(byID cellTypeID: Int32) -> CellType? {
        guard let cellType = DefaultCellTypes(rawValue: cellTypeID) else {
            return nil
        }

        return cellType
    }

}