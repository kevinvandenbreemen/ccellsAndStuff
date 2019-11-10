import cellsAndStuff

enum UnitTestCellType: Int32, CellType {
    case astroCyte = 5
    case elephant = 6
    case basic = 1

    public var id: Int32 { get {
            return self.rawValue
        }
    }
}

class UnitTestCellTypes: CellTypes {

    public init() {
        
    }
    

    public func cellType(byID cellTypeID: Int32) -> CellType? {
        guard let cellType = UnitTestCellType(rawValue: cellTypeID) else {
            return nil
        }

        return cellType
    }

}