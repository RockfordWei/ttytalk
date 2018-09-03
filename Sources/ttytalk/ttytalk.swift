import ttycall

/// simple tty communication interface
public class TTY {

  /// possible errors in the serial conversation
  public enum Exception: Int, Error {
    case unknown = 0
    case invalidParameter = -1
    case openReadFailure = -2
    case openWriteFailure = -3
    case receiverFailure = -4
    case sendingFailure = -5
    case cannotWait = -6
    case deviceFault = -7
    case bufferFault = -8
  }

  private let dev: String
  public init(_ device: String) {
    dev = device
  }

  /// call a device
  /// - parameter device: tty device path, such as /dev/ttyxxx...
  /// - parameter toSend: bytes to send
  /// - parameter size: buffer size to receive
  /// - returns: bytes array
  /// - throws: Exception
  public func call(_ toSend: [UInt8], size: Int) throws -> [UInt8] {

    var buf = UnsafeMutablePointer<UInt8>.allocate(capacity: size)
    defer { buf.deallocate() }
    let ret = toSend.withUnsafeBufferPointer { buffer -> Int in
      guard let pointer = buffer.baseAddress else { return Exception.bufferFault.rawValue }
      return Int(tty_session(self.dev, pointer, Int32(toSend.count), buf, Int32(size)))
    }
    guard ret > 0 else {
      throw Exception(rawValue: ret) ?? Exception.unknown
    }
    let buffer = UnsafeMutableBufferPointer<UInt8>(start: buf, count: ret)
    let array = Array(buffer)
    return array
  }

  /// call a device in text mode
  /// - parameter device: tty device path, such as /dev/ttyxxx...
  /// - parameter command: command text to send
  /// - parameter size: buffer size to receive, 1K by default.
  /// - returns: text returns
  /// - throws: Exception
  public func callText(_ command: String, size: Int = 1024) throws -> String {
    let toSend:[UInt8] = command.utf8.map { $0 }
    let bytes = try self.call(toSend, size: size)
    return bytes.withUnsafeBufferPointer { buffer -> String in
      guard let pointer = buffer.baseAddress else { return "" }
      return String(cString: pointer)
    }
  }
}
