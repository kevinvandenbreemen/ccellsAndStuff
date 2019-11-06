/// Main entry point for rendering a tissue.
public protocol TissueDisplay {
    func display(tissue: TissueManager)
}

public protocol TissueDisplayProvider {

    func getTissueDisplay() -> TissueDisplay

}

public class TissueDisplayProviders {

    public static let provider: TissueDisplayProvider = SDLTissueDisplayProvider()

    private init(){}

}