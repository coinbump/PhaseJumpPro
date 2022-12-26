using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/17/22
 */
namespace PJ
{
    /// <summary>
    /// Used to encode true, false, or none (no value) in Unity property editor
    /// </summary>
    public enum OptionalBoolType
    {
        None, False, True
    }

    /// <summary>
    /// Allows us to switch our game objects between kinematic and non kinematic
    /// Useful for testing scenarios
    /// </summary>
    // FUTURE: add 3D support if needed
    public class KinematicSwitcher : WorldComponent
    {
        public enum WorldType
        {
            // Objects are moved directly, with no physics
            Kinematic,

            // Objects have mass, and are moved by forces
            Physics
        }

        public WorldType worldType = WorldType.Kinematic;

        public float gravityScale = 1.0f;
        public OptionalBoolType freezeRotation = OptionalBoolType.None;

        protected override void Start()
        {
            var objects = FindObjectsOfType<GameObject>();
            foreach (GameObject gameObject in objects)
            {
                var rigidbody2D = gameObject.GetComponent<Rigidbody2D>();

                if (null != rigidbody2D)
                {
                    var node2D = gameObject.GetComponent<GoNode2D>();
                    switch (worldType)
                    {
                        case WorldType.Kinematic:
                            rigidbody2D.bodyType = RigidbodyType2D.Kinematic;
                            rigidbody2D.useFullKinematicContacts = true;
                            break;
                        case WorldType.Physics:
                            rigidbody2D.bodyType = RigidbodyType2D.Dynamic;

                            if (null != node2D && node2D.HasTypeTag("static"))
                            {
                                rigidbody2D.bodyType = RigidbodyType2D.Static;
                            }

                            rigidbody2D.gravityScale = gravityScale;

                            switch (freezeRotation)
                            {
                                case OptionalBoolType.None:
                                    break;
                                case OptionalBoolType.False:
                                    rigidbody2D.freezeRotation = false;
                                    break;
                                case OptionalBoolType.True:
                                    rigidbody2D.freezeRotation = true;
                                    break;
                            }
                            break;
                    }
                }
            }
        }

        protected override void Update()
        {
        }
    }
}
