//import Foundation
//
//public actor Timer: SomeUpdatable {
//    
//    // TODO: finish
//#if false
//    /// Timer duration
//    private var duration: Double;
//    
//    /// Time state
//    private var state = 0.0;
//    
//    private Runner runner;
//    
//    public typealias Func = (Self) -> Void
//    
//    /// (Optional). Called when this finishes
//    var onFinishFunc: Func
//    
//    init(_ duration: Double, runType: RunType) :
//    duration(duration),
//    runner(runType) {
//        runner.onFinishFunc = [this](Runner& runner) { this->OnFinish(); };
//        runner.onResetFunc = [this](Runner& runner) { this->OnReset(); };
//    }
//    
//    // Copy properties, not compose funcs (storing captured this)
//    init(This const& rhs) :
//    duration(rhs.duration),
//    runner(rhs.runner.runType) {
//        runner.onFinishFunc = [this](Runner& runner) { this->OnFinish(); };
//        runner.onResetFunc = [this](Runner& runner) { this->OnReset(); };
//    }
//    
//    float State() const {
//        return state;
//    }
//    
//    func SetState(float value) {
//        GUARD(state != value)
//        state = std::clamp(value, 0.0f, duration);
//        runner.SetIsFinished(state >= duration);
//    }
//    
//    func Stop() {
//        runner.runType = RunType::Once;
//        runner.SetIsFinished(true);
//    }
//    
//    func Reset() {
//        runner.Reset();
//    }
//    
//    func OnReset() {
//        SetState(0);
//    }
//    
//    float Progress() const {
//        return std::clamp(state / duration, 0.0f, 1.0f);
//    }
//    
//    func SetProgress(float progress) {
//        progress = std::clamp(progress, 0.0f, 1.0f);
//        SetState(progress * duration);
//    }
//    
//    func OnFinish() {
//        GUARD(onFinishFunc);
//        (onFinishFunc)(*this);
//    }
//    
//    // MARK: SomeUpdatable
//    
//    FinishType OnUpdate(TimeSlice time) override {
//        if (IsFinished() || duration <= 0) {
//            return FinishType::Finish;
//        }
//        
//        auto delta = TimeDeltaFor(time);
//        if (delta <= 0) {
//            return FinishType::Continue;
//        }
//        
//        // We can't break down large deltas here because events will get compressed
//        // Example: an event happens every 3 seconds, we get a 9 second delta,
//        // they should be 3 seconds apart but are 0 seconds apart instead
//        SetState(state + delta);
//        
//        return IsFinished() ? FinishType::Finish : FinishType::Continue;
//    }
//    
//    float TimeDeltaFor(TimeSlice time) {
//        return time.delta;
//    }
//    
//    /// Allows for cleanup of old updatables
//    bool IsFinished() const override {
//        return runner.IsFinished();
//    }
//    #endif
//};
