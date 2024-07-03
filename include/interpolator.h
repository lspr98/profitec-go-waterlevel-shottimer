#ifndef _INTERPOLATOR_H
#define _INTERPOLATOR_H

/**
 * 
 * Interpolator class performs a sigmoid interpolation for a given range to
 * create an ease-in and ease-out effect for animations
 * 
 */
class Interpolator {
private:
    // transitionTime:  total duration for animation
    // frameTime:       time per step
    // stepSize:        step increment to reach _endValue after _nSteps
    // startValue:      desired initial value
    // endValue:        desired final value
    // nSteps:          number of steps required to reach final value
    // counter:         internal step counter
    float _transitionTime, _frameTime, _stepSize, _startValue, _endValue;
    int _nSteps, _counter;

    /**
     * Scaled sigmoid function to map t \in \mathbb{R} to interval (0, 1)
     */
    float sigmoid(float t);

public:
    Interpolator();

    /**
     * Resets the interpolator to a new interval [startValue, endValue]
     * with given transitionTime.
     */
    void setFromTo(float startValue, float endValue, float transitionTime);

    /**
     * Steps the interpolator. The next value is written into nextValue.
     * Returns true if _nSteps steps are reached to signal end of animation, false otherwise.
     */
    bool next(float &nextValue);
};

#endif