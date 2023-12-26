/*
 * RATING: 5 stars
 * Simple base class
 * CODE REVIEW: 12/20/22
 * PORTED TO: C++
 */
namespace PJ
{
    public class Core : Base
    {
        protected Class _class;
        protected string classId;   // If class is not yet assigned

        public Class Class
        {
            get => _class;
            set
            {
                _class = value;
                if (null != _class)
                {
                    classId = _class.id;
                }
            }
        }

        /// <summary>
        /// If we have a class object, return its id, otherwise use the id we have
        /// </summary>
        public string ClassId
        {
            get => _class != null ? _class.id : classId;
            set => classId = value;
        }

        /// <summary>
        /// Custom properties
        /// </summary>
        public Tags tags = new Tags();
    }
}
