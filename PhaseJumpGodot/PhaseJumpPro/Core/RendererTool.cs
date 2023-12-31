using Godot;

/*
 * RATING: 5 stars
 * Simple wrapper class
 * CODE REVIEW: 12/20/22
 */
namespace PJ
{
    /// <summary>
    /// Allows us to write code that will work with multiple types of renderers
    /// without knowing which one is attached to the object
    /// </summary>
    public class RendererTool
    {
        public MeshMaterialType meshMaterialType = MeshMaterialType.Shared;

        protected Sprite2D spriteRenderer;
        // FUTURE: support text, mesh

        public RendererTool(Node node)
        {
            if (node is Sprite2D sprite2D)
            {
                spriteRenderer = sprite2D;
            }
        }

        public Optional<Vector3> WorldSize
        {
            get
            {
                if (null != spriteRenderer)
                {
                    var textureSize = spriteRenderer.Texture.GetSize();
                    return new(new(textureSize.X, textureSize.Y, 0));
                }

                return null;
            }
        }

        public Node Renderer
        {
            get
            {
                if (null != spriteRenderer)
                {
                    return spriteRenderer;
                }
                return null;
            }
        }

        public Color32 Color32
        {
            get => new(Color);
            set
            {
                Color = value;
            }
        }

        public Color Color
        {
            get
            {
                if (null != spriteRenderer)
                {
                    return new(spriteRenderer.Modulate);
                }
                return new Color(1.0f, 1.0f, 1.0f, 1.0f);
            }
            set
            {
                if (null != spriteRenderer)
                {
                    spriteRenderer.Modulate = value;
                }
            }
        }

        public float Alpha
        {
            get => ((float)Color32.a) / 255.0f;
            set
            {
                Color = new Color(Color.R, Color.G, Color.B, (byte)(value * 255.0f));
            }
        }
    }
}
