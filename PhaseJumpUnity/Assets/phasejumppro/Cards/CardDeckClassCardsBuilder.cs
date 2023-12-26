using System;
using System.Collections.Generic;
using System.Linq;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/15/23
*/
namespace PJ
{
    /// <summary>
    /// Builds the cards for a card deck class from JSON
    /// </summary>
    public class CardDeckClassCardsBuilder
    {
        public void Build(SomeCardDeckClass _class, string jsonString)
        {
            _class.cardInfos.Clear();

            JObject root = JObject.Parse(jsonString);
            JArray cardList = (JArray)root["cards"];

            foreach (var _card in cardList)
            {
                var cardMap = (JObject)_card;
                if (null == cardMap) { continue; }

                var id = (string)cardMap["id"];
                if (null == id) { continue; }

                var card = new SomeCardDeckClass.CardInfo(id);

                var tags = (JObject)cardMap["tags"];
                foreach (var tag in tags.Properties())
                {
                    switch (tag.Value.Type)
                    {
                        case JTokenType.Integer:
                            card.tags[tag.Name] = (int)tag.Value;
                            break;
                        case JTokenType.Float:
                            card.tags[tag.Name] = (float)tag.Value;
                            break;
                        case JTokenType.String:
                            card.tags[tag.Name] = (string)tag.Value;
                            break;
                        case JTokenType.Boolean:
                            card.tags[tag.Name] = (bool)tag.Value;
                            break;
                        default:
                            break;
                    }
                }

                _class.cardInfos[card.id] = card;
            }
        }
    }
}
