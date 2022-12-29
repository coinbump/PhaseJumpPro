using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 12/29/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Class objects are useful for defining modular behavior based on object type
    /// We might want to have properties that are defined dynamically (during runtime),
    /// but also shared by multiple objects of the same type
    /// </summary>
    public class Class
    {
        public string id;

        /// <summary>
        /// Optional title, for browsing
        /// </summary>
        public string title;

        /// <summary>
        /// Optional description, for browsing
        /// </summary>
        public string description;

        /// <summary>
        /// Set of string tags that define type properties
        /// </summary>
        public TypeTagsSet typeTags = new TypeTagsSet();

        /// <summary>
        /// Tag metadata
        /// </summary>
        public Tags tags = new Tags();

        public Class(string id)
        {
            this.id = id;
        }

        public Class(string id, TypeTagsSet typeTags)
        {
            this.id = id;
            this.typeTags = typeTags;
        }
    }
}
