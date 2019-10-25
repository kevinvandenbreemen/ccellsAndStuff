public enum DefaultCellTypes: Int32, CellType {

    case basic = 1
    case inhibitory = 2

    public var id: Int32 { get {
            return self.rawValue
        }
    }

}