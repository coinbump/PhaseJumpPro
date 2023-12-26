using System;

/*
 * RATING: 5 stars
 * Simple, with unit tests
 * CODE REVIEW: 1/11/22
 */
namespace PJ
{
    public abstract class SomeFactory : AnyFactory
    {
        public abstract object NewObject();
    }

    /// <summary>
    /// Creates type
    /// </summary>
    public class Factory<Type> : SomeFactory
    {
        private readonly Func<Type> allocator;

        public Factory(Func<Type> allocator)
        {
            this.allocator = allocator;
        }

        public Type New()
        {
            return allocator();
        }

        public override object NewObject()
        {
            return New();
        }
    }

    /// <summary>
    /// Creates type based on input
    /// </summary>
    public class InputFactory<Input, Type>
    {
        private readonly Func<Input, Type> allocator;

        public InputFactory(Func<Input, Type> allocator)
        {
            this.allocator = allocator;
        }

        public Type New(Input input)
        {
            return allocator(input);
        }
    }

    /// <summary>
    /// Convenience class
    /// </summary>
    public class FactoryNew<Type> : SomeFactory where Type : new()
    {
        public Type New()
        {
            return new Type();
        }

        public override object NewObject()
        {
            return New();
        }
    }
}
