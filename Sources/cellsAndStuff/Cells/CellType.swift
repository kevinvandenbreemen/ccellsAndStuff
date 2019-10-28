import Foundation
import cCellsAndStuff

public protocol CellType {

    var id: Int32 { get }

}

/**
 * Provider of CellType for your system
 */
public protocol CellTypes {

    func cellType(byID cellTypeID: Int32) -> CellType?

}

extension CellType {
    
    func setBehaviour(to cellTypeBehaviour: inout CellTypeBehaviour) {
        cCellsAndStuff.swift_cellTypes_setBehaviourForCellType(self.id, &cellTypeBehaviour)
    }

    public func setLogic(to logic: CellTypeLogic) {

        var behaviour: CellTypeBehaviour = CellTypeBehaviour(
            getOutputStrength: { (cellType, inputStrength, outgoingConnectionStrength) in 
                guard let logic = CellTypeLogicRegistry.getLogic(for: cellType) else {
                    return 0.0
                }

                return logic.computeOutputStrength(givenInput: inputStrength, givenOutputSum: outgoingConnectionStrength)
            },

            processIncomingConnections: { (cellType, size, cellIndex, incomingIndexes, incomingStrengths) in
                guard let logic = CellTypeLogicRegistry.getLogic(for: cellType), 
                    let incomingIndexes = incomingIndexes, let incomingStrengths = incomingStrengths else {
                    return
                }

                var incomingCellIndexes: [Int32] = []
                var incomingConnectionStrengths: [Double] = []

                for i in 0..<size {

                    let index = Int(i)

                    let indexVal = (incomingIndexes + index).pointee
                    let connectionStrength = (incomingStrengths + index).pointee
                    
                    incomingCellIndexes.append( indexVal )
                    incomingConnectionStrengths.append(connectionStrength)
                }

                logic.handleIncomingConnectionState(cellIndex: cellIndex, incomingIndexes: incomingCellIndexes, 
                    incomingStrengths: incomingConnectionStrengths
                )
            },

            processOutgoingConnections: { (cellType, size, cellIndex, outgoingIndexes, outgoingStrengths) in 
                guard let logic = CellTypeLogicRegistry.getLogic(for: cellType), 
                    let outgoingIndexes = outgoingIndexes, let outgoingStrengths = outgoingStrengths else {
                    return
                }

                var outgoingCellIndexes: [Int32] = []
                var outgoingConnectionStrengths: [Double] = []

                for i in 0..<size {

                    let index = Int(i)

                    let indexVal = (outgoingIndexes + index).pointee
                    let connectionStrength = (outgoingStrengths + index).pointee
                    
                    outgoingCellIndexes.append( indexVal )
                    outgoingConnectionStrengths.append(connectionStrength)
                }

                logic.handleOutgoingConnectionState(cellIndex: cellIndex, outgoingIndexes: outgoingCellIndexes, 
                    outgoingStrengths: outgoingConnectionStrengths
                )
            },

            calculateActivation: { cellType, weightedInputSum in

                guard let logic = CellTypeLogicRegistry.getLogic(for: cellType)  else {
                    return weightedInputSum
                }

                return logic.calculateActivation(weightedInputSum: weightedInputSum)
            }
        )

        self.setBehaviour(to: &behaviour)
        CellTypeLogicRegistry.registerCellTypeLogic(cellTypeID: self.id, logic: logic);

    }

}