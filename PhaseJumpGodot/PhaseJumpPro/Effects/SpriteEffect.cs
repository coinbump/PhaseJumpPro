// using Godot;
// using System.Collections;

// /*
//  * RATING: 5 stars
//  * Tested and works
//  * CODE REVIEW: 12/26/22
//  */
// namespace PJ
// {
//     /// <summary>
//     /// Change the sprite
//     /// </summary>
//     public partial class SpriteEffect : SomeEffect
//     {
//         public Sprite offSprite;
//         public Sprite onSprite;

//         protected Sprite defaultSprite;

//         public SpriteRenderer SpriteRenderer
//         {
//             get
//             {
//                 if (TryGetComponent(out SpriteRenderer spriteRenderer))
//                 {
//                     if (!defaultSprite)
//                     {
//                         defaultSprite = spriteRenderer.sprite;
//                     }
//                     return spriteRenderer;
//                 }

//                 return null;
//             }
//         }

//         public override void UpdateEffectProperties()
//         {
//             base.UpdateEffectProperties();

//             var spriteRenderer = SpriteRenderer;
//             if (!spriteRenderer) { return; }

//             switch (IsOn)
//             {
//                 case true:
//                     if (onSprite)
//                     {
//                         spriteRenderer.sprite = onSprite;
//                     }
//                     break;
//                 default:
//                     var sprite = offSprite != null ? offSprite : defaultSprite;
//                     if (sprite)
//                     {
//                         spriteRenderer.sprite = sprite;
//                     }
//                     break;
//             }
//         }
//     }
// }
