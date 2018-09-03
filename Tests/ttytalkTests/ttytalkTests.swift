import XCTest
@testable import ttytalk

final class ttytalkTests: XCTestCase {
  static var allTests = [
    ("testExample", testExample),
    ]
  func testExample() {
    let tty = TTY("/dev/cu.usbmodem1411")
    do {
      let info = try tty.callText("info\r\n")
      print(info)
    } catch let err {
      XCTFail(err.localizedDescription)
    }
  }


}

