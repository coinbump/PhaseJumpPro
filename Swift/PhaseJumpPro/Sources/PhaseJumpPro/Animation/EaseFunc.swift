import Foundation

/// Transforms a normalized progress value (0-1.0) to a new normalized progress value (0-1.0)
public typealias EaseFunc = @Sendable (Double) -> Double

public enum EaseFuncs {
    public static let linear: EaseFunc = {
        $0
    }
    public static let inSquared: EaseFunc = {
        pow($0, 2)
    }
    public static let inCubed: EaseFunc = {
        pow($0, 3)
    }
    public static let outSquared: EaseFunc = {
        1.0 - pow($0, 2)
    }
    public static let outCubed: EaseFunc = {
        1.0 - pow($0, 3)
    }
    
    // FUTURE: port from C++ as needed
    /*
    public static let outElastic: EaseFunc = {
    }
    public static let inBack: EaseFunc = {
    }
    public static let outBack: EaseFunc = {
    }
    public static let inOutBack: EaseFunc = {
    }
     */
} // namespace EaseFuncs

// FUTURE: port from C++ as needed
/*
const EaseFunc EaseFuncs::outElastic = [](float value) {
    float x = value;
    float c4 = (2.0f * (float)M_PI) / 3.0f;
    return x == 0 ? 0 : x == 1 ? 1 : powf(2, -15 * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f;
};

const EaseFunc EaseFuncs::inBack = [](float value) {
    float x = value;
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    return c3 * x * x * x - c1 * x * x;
};

const EaseFunc EaseFuncs::outBack = [](float value) {
    float x = value;
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    return 1.0f + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
};

const EaseFunc EaseFuncs::inOutBack = [](float value) {
    float x = value;
    float c1 = 1.70158f;
    float c2 = c1 * 1.525f;
    return x < 0.5f ? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f
                    : (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
};
*/
