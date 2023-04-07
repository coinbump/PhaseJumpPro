using UnityEngine;
using System;
using System.Collections.Generic;
using TMPro;
using UnityEngine.InputSystem;

namespace PJ
{
    public class SpeechRunner : WorldComponent, SomeGoStateListener<SpeechRunner.StateType>
    {
        public enum StateType { Hidden, Show, Reveal, Revealed, Hide }

        /// <summary>
        /// Where we put the speech text
        /// </summary>
        public TextMeshPro speechTextMesh;

        /// <summary>
        /// Root of the combined speech UI so we can hide/show everything at once
        /// This can include a text mesh, background frame, name tag mesh, name tag frame, continue UI, etc.
        /// </summary>
        public GameObject speechRoot;

        /// <summary>
        /// (OPTIONAL) Where we put the actor's name text
        /// </summary>
        public TextMeshPro actorNameTextMesh;

        /// <summary>
        /// (OPTIONAL) Shows the continue UI (usually a down-arrow or triangle)
        /// </summary>
        public GameObject continueUI;

        /// <summary>
        /// If true, first speech is started in Start
        /// </summary>
        public bool autoStart = false;

        [SerializeField]
        protected List<Speech> speeches = new();

        protected List<Speech> flatSpeeches = new();
        protected int flatSpeechIndex;

        protected GoCore<StateType> core;

        public string ActorName
        {
            get
            {
                var speech = ActiveSpeech;
                if (null == speech || null == speech.actorModel) { return ""; }

                return speech.actorModel.name;
            }
        }

        public Speech ActiveSpeech
        {
            get
            {
                if (flatSpeechIndex < 0 || flatSpeechIndex >= flatSpeeches.Count)
                {
                    return null;
                }
                return flatSpeeches[flatSpeechIndex];
            }
        }

        public GameObject SpeechRoot => speechRoot != null ? speechRoot : speechTextMesh != null ? speechTextMesh.gameObject : null;

        public bool IsFinalSpeech => flatSpeeches.Count > 0 && flatSpeechIndex == flatSpeeches.Count - 1;

        protected RevealTextEffect revealTextEffect;

        public SpeechRunner()
        {
            core = new(this);
        }

        protected override void Awake()
        {
            base.Awake();

            ConfigureSpeechTextMesh();
            OnSpeechesChange();
            OnSpeechChange();
            UpdateForState();
        }

        protected override void Start()
        {
            base.Start();

            if (!autoStart) { return; }
            Go();
        }

        protected virtual void OnSpeechesChange()
        {
            var flatSpeeches = new List<Speech>();

            // Flatten the speeches into a single list to make logic easier
            foreach (var speech in speeches)
            {
                foreach (var text in speech.texts)
                {
                    var flatSpeech = new Speech();
                    flatSpeech.texts.Add(text);
                    flatSpeech.actorModel = speech.actorModel;
                    flatSpeeches.Add(flatSpeech);
                }
            }

            this.flatSpeeches = flatSpeeches;
        }

        protected virtual void ConfigureSpeechTextMesh()
        {
            if (!speechTextMesh) { return; }

            // Add a reveal text component if you want to configure it yourself or we'll use the default
            revealTextEffect = speechTextMesh.GetComponent<RevealTextEffect>();
            if (null == revealTextEffect)
            {
                revealTextEffect = speechTextMesh.gameObject.AddComponent<RevealTextEffect>();
            }
        }

        public virtual void Go()
        {
            var speechRoot = SpeechRoot;
            if (!speechRoot) { return; }

            ShowSpeech();
        }

        public virtual void ShowSpeech()
        {
            core.State = StateType.Reveal;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            switch (core.State)
            {
                case StateType.Reveal:
                    if (revealTextEffect.IsFinished)
                    {
                        core.State = StateType.Revealed;
                    }
                    break;
            }
        }

        protected virtual void OnSpeechChange()
        {
            UpdateSpeechText();
            UpdateActorName();
            UpdateContinueUI();
        }

        protected virtual void UpdateSpeechText()
        {
            if (!speechTextMesh) { return; }
            speechTextMesh.text = ActiveSpeech.texts[0].text;
        }

        protected virtual void UpdateActorName()
        {
            if (!actorNameTextMesh) { return; }
            actorNameTextMesh.text = ActorName;
        }

        protected virtual void UpdateContinueUI()
        {
            if (!continueUI) { return; }

            switch (core.State)
            {
                case StateType.Revealed:
                    continueUI.SetActive(true);
                    break;
                default:
                    continueUI.SetActive(false);
                    break;
            }
        }

        /// <summary>
        /// If the text is being revealed, reveal it. Otherwise continue to the next speech
        /// </summary>
        public virtual void OnInputContinueSpeech(InputValue inputValue)
        {
            switch (core.State)
            {
                case StateType.Show:
                case StateType.Reveal:
                    core.State = StateType.Revealed;
                    break;
                case StateType.Revealed:
                    GotoNext();
                    break;
                case StateType.Hide:
                case StateType.Hidden:
                    break;
            }
        }

        public virtual void GotoNext()
        {
            var isFinalSpeech = IsFinalSpeech;
            flatSpeechIndex++;

            if (isFinalSpeech)
            {
                core.State = StateType.Hidden;
            }
            else
            {
                OnSpeechChange();
                core.State = StateType.Reveal;
            }
        }

        public void OnStateChange(GoStateMachine<StateType> inStateMachine)
        {
            UpdateForState();
        }

        protected virtual void UpdateForState()
        {
            var speechRoot = SpeechRoot;

            switch (core.State)
            {
                case StateType.Reveal:
                    revealTextEffect.Reset();
                    revealTextEffect.IsOn = true;
                    break;
                case StateType.Revealed:
                    revealTextEffect.Finish();
                    break;
                default:
                    break;
            }

            if (speechRoot)
            {
                speechRoot.SetActive(core.State != StateType.Hidden);
            }

            UpdateContinueUI();
        }

        public void OnStateFinish(GoStateMachine<StateType> inStateMachine)
        {
        }
    }
}
