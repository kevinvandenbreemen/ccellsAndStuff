/// Main entry point for rendering a tissue.
public protocol TissueDisplay {
    func display(tissue: TissueManager)
}

public protocol TissueDisplayProvider {

    func getTissueDisplay() -> TissueDisplay

}