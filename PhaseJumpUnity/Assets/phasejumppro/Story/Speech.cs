using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    [Serializable]
    public class SpeechText
    {
        public string text;
    }

    [Serializable]
    public class ActorModel
    {
        public string id;
        public string name;
        public string state;
    }

    /// <summary>
    /// An object that represents an element of speech by an actor (any entity †hat speaks)
    /// Speeches are usually displayed in a frame with optional UI such as the name of the actor speaking
    /// or an actor portrait
    /// </summary>
    [Serializable]
    public class Speech
    {
        /// <summary>
        /// The texts to display (may contain markup)
        /// </summary>
        public List<SpeechText> texts = new();

        /// <summary>
        /// (OPTIONAL) Information on the actor used to show art/animation along with the speech
        /// </summary>
        public ActorModel actorModel;
    }
}
