//
//  SwiftBridge.swift
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/10/22.
//

import Foundation
import AppKit
import SwiftUI

@objc class SwiftBridge: NSObject {
    @objc class func makeFilesProcessorViewController() -> NSViewController {
        let result = NSHostingController(
            rootView: FilesProcessorView() {
                ObjCBridge.shared().startFilesProcessor(withFilePathsCount: $0)
            }
        )
//        result.sizingOptions = .standardBounds
        return result
    }
}
