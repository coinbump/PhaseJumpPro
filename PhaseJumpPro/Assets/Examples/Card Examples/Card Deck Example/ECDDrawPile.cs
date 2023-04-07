using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PJ;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using System.Linq;

/// <summary>
/// Build a deck with the standard 52-card deck and draw 3 cards each time you click on the draw pile
/// </summary>
public class ECDDrawPile : WorldComponent, IPointerClickHandler
{
    /// <summary>
    /// The JSON file containing the definition of the deck (use the standard deck)
    /// </summary>
    public TextAsset cardDeckText;

    /// <summary>
    /// The card object that will be created when dealing a card
    /// </summary>
    public GameObject cardObject;

    /// <summary>
    /// The deck of cards (model)
    /// </summary>
    protected CardDeck<Card> cardDeck;

    /// <summary>
    /// zIndex of the next card to be dealt
    /// </summary>
    protected float zIndex;

    public class Card : SomeCard
    {
        public Card(string id)
        {
            this.id = id;
        }
    }

    public class CardDeckClass : CardDeckClass<Card>
    {
        public CardDeckClass() : base("example.cardDeck")
        {
        }

        // TODO: fix this broken example
        // public override ECDDrawPile.Card New(string id)
        // {
        //     return new ECDDrawPile.Card(id);
        // }
    }

    protected override void Awake()
    {
        base.Awake();

        // Parse the deck definition into the deck class, then build a deck model from the class
        var jsonString = cardDeckText.ToString();
        var builder = new CardDeckClassCardsBuilder();
        var _class = new CardDeckClass();
        builder.Build(_class, jsonString);

        var cardDeck = new CardDeck<Card>(_class);
        cardDeck.Build(card => true);
        cardDeck.Shuffle(new UnityRandom());

        this.cardDeck = cardDeck;
    }

    protected override void Start()
    {
        base.Start();
        DrawCards();
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        DrawCards();
    }

    protected void DrawCards()
    {
        if (cardDeck.cards.Count == 0)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            if (cardDeck.cards.Count == 0)
            {
                break;
            }

            var topCard = cardDeck.cards[0];
            Deal(topCard);
            cardDeck.cards.RemoveAt(0);
        }
    }

    protected void Deal(Card card)
    {
        var cardObject = Instantiate(this.cardObject, transform.position, Quaternion.identity);

        if (cardObject.TryGetComponent(out ECDCard cardComponent))
        {
            var suite = "";
            var id = card.id;
            var idElements = id.Split('.').ToList();
            if (idElements.Count > 0)
            {
                suite = idElements[0];
            }

            Color color = Color.black;
            switch (suite)
            {
                case "heart":
                case "diamond":
                    color = Color.red;
                    break;
                default:
                    break;
            }

            cardComponent.suiteTextMesh.text = suite;
            cardComponent.suiteTextMesh.color = color;
            cardComponent.valueTextMesh.color = color;

            var value = card.tags.Value<int>("value").value;

            switch (value)
            {
                case 1:
                    cardComponent.valueTextMesh.text = "A";
                    break;
                case 11:
                    cardComponent.valueTextMesh.text = "J";
                    break;
                case 12:
                    cardComponent.valueTextMesh.text = "Q";
                    break;
                case 13:
                    cardComponent.valueTextMesh.text = "K";
                    break;
                default:
                    cardComponent.valueTextMesh.text = value.ToString();
                    break;
            }

            var screenWorldSize = Utils.ScreenWorldSize(Camera.main);
            float inset = 2.0f;
            var xPos = Random.value * (screenWorldSize.x - inset * 2.0f);
            var yPos = Random.value * (screenWorldSize.y - inset * 2.0f);

            var xDest = -(screenWorldSize.x / 2.0f - inset) + xPos;
            var yDest = (screenWorldSize.y / 2.0f - inset) * Vector2.up.y + yPos * Vector2.down.y;

            var animationDuration = 0.3f;

            var positionAnimator = new Animator<Vector3>(
                new Vector3Interpolator(cardObject.transform.position, new Vector3(xDest, yDest, transform.position.z + zIndex), new EaseOutSquared()),
                animationDuration,
                new SetBinding<Vector3>(value => cardObject.transform.position = value)
            );

            cardComponent.updatables.Add(positionAnimator);

            var rotateAnimator = new Animator<Vector3>(
                new Vector3Interpolator(cardObject.transform.eulerAngles, new Vector3(0, 0, new UnityRandom().VaryFloat(0, 20.0f)), new EaseOutSquared()),
                animationDuration,
                new SetBinding<Vector3>(value => cardObject.transform.eulerAngles = value)
            );

            cardComponent.updatables.Add(rotateAnimator);
            zIndex += Vector3.back.z * .001f;
        }
    }
}
