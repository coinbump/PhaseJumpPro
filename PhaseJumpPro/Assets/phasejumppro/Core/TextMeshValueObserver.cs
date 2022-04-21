using System;
using UnityEngine;
using TMPro;

/*
 * RATING: 5 stars
 * Tested and works
 * 4/19/22
 */
namespace PJ
{
    // <summary>
    // Listens to an observable value and updates the text based on that value
    // </summary>
    public class TextMeshValueObserver : MonoBehaviour, SomeListener
    {
        protected PublishedValue<string> stringValue = new PublishedValue<string>();
        protected TextMeshPro textMeshPro;

        public PublishedValue<string> StringValue
        {
            get => stringValue;
            set
            {
                if (stringValue == value) { return; }

                if (null != stringValue)
                {
                    stringValue.broadcaster.RemoveListener(this);
                }

                stringValue = value;

                if (null != stringValue)
                {
                    stringValue.broadcaster.AddListener(this);
                    UpdateText();
                }
            }
        }

        public TextMeshValueObserver()
        {
            stringValue.broadcaster.AddListener(this);
        }

        protected virtual void Awake()
        {
            textMeshPro = GetComponent<TextMeshPro>();
            if (null == textMeshPro)
            {
                Debug.Log("Error. ObserveTextBuilder requires a text mesh");
                return;
            }

            UpdateText();
        }

        public void OnListen(Event theEvent)
        {
            var stringChangeEvent = theEvent as PublishedValue<string>.EventValueChange;
            if (null != stringChangeEvent && stringChangeEvent.value == stringValue)
            {
                UpdateText();
            }
        }

        protected void UpdateText()
        {
            if (null == textMeshPro) { return; }

            textMeshPro.text = stringValue.Value;
        }
    }
}
