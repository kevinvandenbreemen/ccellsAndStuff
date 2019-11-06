import cCellsAndStuff

class SDLTissueDisplayProvider: TissueDisplayProvider {

    private lazy var display: SDLTissueDisplay = SDLTissueDisplay()

    func getTissueDisplay() -> TissueDisplay {
        return display
    }

}


class SDLTissueDisplay: TissueDisplay {
    func display(tissue: TissueManager) {
        cCellsAndStuff.swift_tissue_display_tissue()
    }

}