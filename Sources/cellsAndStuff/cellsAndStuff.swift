import cCellsAndStuff

public struct CellsAndStuffSystemAbout {
    
    public func versionNum() -> String {
        let majorVer = cCellsAndStuff.majorVersion()
        let minorVer = cCellsAndStuff.minorVersion()
        let revNum = cCellsAndStuff.revisionNum()
        return "\(majorVer).\(minorVer).\(revNum)"
    }

    public func getAbout() -> String {
        return "C Cells and Stuff Integration Lib:  v.\(versionNum())"

    }

}
