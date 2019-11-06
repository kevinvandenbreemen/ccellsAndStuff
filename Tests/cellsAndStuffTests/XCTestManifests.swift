import XCTest

#if !canImport(ObjectiveC)
public func allTests() -> [XCTestCaseEntry] {
    return [
        testCase(cellsAndStuffTests.allTests),
        testCase(CellTests.allTests),
        testCase(TissueTests.allTests),
        testCase(TissueDisplayTests.allTests),
    ]
}
#endif
