using UnityEngine;
using System;
using PJ;

namespace PJ
{
    /// <summary>
    /// A place where cards can be placed and dragged to
    /// </summary>
    // TODO: work in progress
    public abstract class SomeCardPlace : SomeDropTarget
    {
        public override bool CanAcceptDrag(DragItems items)
        {
            bool itemsContainsCard = false;

            foreach (var dragHandler in items.dragHandlers)
            {
                if (!dragHandler.TryGetComponent(out SomeCardHandler cardHandler)) { continue; }

                var card = cardHandler.SomeCard;
                if (null == card) { continue; }

                itemsContainsCard = true;

                if (!CanAcceptCard(card)) { return false; }
            }

            return itemsContainsCard;
        }

        protected abstract bool CanAcceptCard(SomeCard card);
    }

    /// <summary>
    /// Used to attach a card to a game object
    /// </summary>
    public interface SomeCardHandler
    {
        public SomeCard SomeCard { get; set; }
    }
}
