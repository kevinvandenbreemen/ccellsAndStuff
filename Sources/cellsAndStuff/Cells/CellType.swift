import Foundation

public protocol CellType {

    var id: Int32 { get }

}

/**
 * Provider of CellType for your system
 */
public protocol CellTypes {

    func cellType(byID cellTypeID: Int32) -> CellType?

}