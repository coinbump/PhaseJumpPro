using System;
using UnityEngine;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    public enum PointerInputButton
    {
        Left,
        Right,
        Middle,

        // A custom button
        Extended
    }

    public struct PointerInputButtonModel
    {
        public PointerInputButton button;

        /// <summary>
        /// (OPTIONAL) id for non-standard button
        /// </summary>
        public String buttonId;

        public PointerInputButtonModel(PointerInputButton buttonModel, String buttonId)
        {
            this.button = buttonModel;
            this.buttonId = buttonId;
        }
    }

    public class SomePointerUIEvent : SomeUIEvent
    {
        public PointerInputButtonModel buttonModel;

        public PointerInputButton Button => buttonModel.button;
    }
}
