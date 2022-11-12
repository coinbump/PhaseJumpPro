//
//  HelloSwiftUI.swift
//  Kaiju
//
//  Created by Jeremy Vineyard on 11/10/22.
//

import SwiftUI

/// Processes files when dropped onto it
/// FUTURE: support folders
struct FilesProcessorView: View {
    let startFilesProcessor: (Int) -> Void

    init(startFilesProcessor: @escaping (Int) -> Void) {
        self.startFilesProcessor = startFilesProcessor
    }

    @State private var isDragOver = false

    var body: some View {
        VStack {
            Text("Drag Files Inside")

            ZStack {
                Rectangle()
                    .stroke(lineWidth: 3.0)
                    .foregroundColor(isDragOver ? Color.green : Color.gray)
                    .onDrop(of: [.fileURL], isTargeted: $isDragOver) { providers in
                        let urlProviders = providers.filter { $0.canLoadObject(ofClass: URL.self) }
                        ObjCBridge.shared().startFilesProcessor(withFilePathsCount: urlProviders.count)

                        urlProviders.forEach { provider in
                            let _ = provider.loadObject(ofClass: URL.self) { object, error in
                                if let url = object {
                                    let path = url.path
                                    print("path: \(path)")
                                    ObjCBridge.shared().provideFilePath(path)
                                    ObjCBridge.shared().processNextFile()
                                }
                            }
                        }

                        return !urlProviders.isEmpty
                    }

                Image(systemName: "folder")
            }
        }
        .padding()
    }
}
