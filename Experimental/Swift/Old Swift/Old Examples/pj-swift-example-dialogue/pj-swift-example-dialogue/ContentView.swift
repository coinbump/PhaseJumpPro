//
//  ContentView.swift
//  pj-swift-example-dialogue
//
//  Created by Jeremy Vineyard on 4/2/22.
//

import Combine
import SwiftUI

/*
 An example of loading a graph for interactive dialogue choices
 */
struct ContentView: View {
    @StateObject var viewModel = ContentViewModel()

    var body: some View {
        VStack(spacing: 12) {
            Text("Dialogue")
                .bold()
                .frame(maxWidth: .infinity)
                .background(Color.gray)

            HStack(spacing: 16) {
                Spacer()
                    .frame(width: 2)

                if viewModel.canGoBack {
                    Button(action: { viewModel.goBack() }) {
                        Image(systemName: "arrow.backward")
                    }
                }
                Text(viewModel.text)
                    .multilineTextAlignment(.leading)
                Spacer()
            }

            Text("Choices")
                .bold()
                .frame(maxWidth: .infinity)
                .background(Color.gray)
            List(viewModel.choices, id:\.identifier) { choice in
                HStack {
                    Text(choice.text)
                    Spacer()
                }
                .contentShape(Rectangle())
                .onTapGesture {
                    viewModel.select(choice: choice)
                }
            }
            .listStyle(.grouped)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
