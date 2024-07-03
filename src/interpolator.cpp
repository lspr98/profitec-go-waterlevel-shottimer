#include <interpolator.h>
#include <serialutils.h>
#include <math.h>
#include <config.h>

Interpolator::Interpolator() : 
    _frameTime(FRAME_TIME_MS), _startValue(0), _endValue(0) {};

float Interpolator::sigmoid(float t) {
    return 1/(1 + exp(-(t-0.5)*12));
}

void Interpolator::setFromTo(float startValue, float endValue, float transitionTime) {
    _transitionTime = transitionTime;
    // Calculate number of steps
    _nSteps = ceil(_transitionTime/_frameTime);
    // Calculate step size such that _nSteps*_stepSize = 1
    _stepSize = 1.0f/_nSteps;
    _counter = _nSteps;
    _startValue = startValue;
    _endValue = endValue;
    _counter = 0;
}

bool Interpolator::next(float &nextValue) {
    // Check if animation is finished
    if(_counter == _nSteps) {
        // _nSteps reached, signal animation end
        nextValue = _endValue;
        return true;
    } else {
        // animation not finished, increase counter and update value
        _counter++;
        nextValue = _startValue + sigmoid((float)_counter*_stepSize)*(_endValue-_startValue);
        return false;
    }
}