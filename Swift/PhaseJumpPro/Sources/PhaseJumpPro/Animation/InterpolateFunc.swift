import Foundation

/// Function that returns a value between [start, end] based on progress
/// Progress value is usually 0-1.0, but can overflow for elastic animations
public typealias InterpolateFunc<T> = (_ progress: Double) -> T

// TODO: finish port

public enum InterpolateFuncs {
    /**
     @return Returns the standard func for interpolating between start and end values
     */
//    public func make<T>(start: T, end: T) -> InterpolateFunc<T> where T: Numeric {
//        { (progress: Double) in
//            start + (end - start) * progress
//        }
//    }
    
    /// @return Returns an interpolation func that applies an ease curve to the progress
    /// parameter
//    template <class T>
//    InterpolateFunc<T> Ease(InterpolateFunc<T> func, EaseFunc ease = EaseFuncs::linear) {
//        return [=](float progress) {
//            GUARDR(func && ease, T())
//            return func(ease(progress))
//        }
//    }
    
    /// @return Returns an interpolation func that interpolates in reverse (from end to start)
    public func reverse<T>(_func: @escaping InterpolateFunc<T>) -> InterpolateFunc<T> {
        {
            _func(1.0 - $0)
        }
    }
    
//    /// @return Returns a set binding func that takes a progress value, calls the interpolator,
//    /// and sets the value via the binding
//    template <class T>
//    SetBindingFunc<T> Binding(InterpolateFunc<T> func, SetBindingFunc<T> binding) {
//        return [=](float progress) {
//            GUARD(func && binding)
//            binding(func(progress))
//        }
//    }
}
