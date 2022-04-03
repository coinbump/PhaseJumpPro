import XCTest
@testable import PhaseJumpPro

final class WeakReferenceTests: XCTestCase {
    private class Foo: Equatable {
        let identifier = UUID().uuidString

        static func == (lhs: Foo, rhs: Foo) -> Bool {
            return lhs.identifier == rhs.identifier
        }
    }

    func test_weakReference_is_nilled() {
        let foo = Foo()
        let weak = WeakReference(value: foo)
        XCTAssertEqual(weak.value, foo)

        DispatchQueue.main.async {
            XCTAssertEqual(weak.value, nil)
        }
    }
}
