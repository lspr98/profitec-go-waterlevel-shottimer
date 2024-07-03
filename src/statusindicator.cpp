#include <statusindicator.h>
#include <display.h>
#include <graphics.h>
#include <serialutils.h>
#include <Roboto_Light20pt7b.h>
#include <config.h>


StatusIndicator::StatusIndicator() : 
    _xCenter(STATUS_CENTER_X), _yCenter(STATUS_CENTER_Y) {

    _hidden = true;

};

void StatusIndicator::drawSymbol(uint16_t color) {
    switch (_state) {
        case StatusIndicatorState::TIMER:
            // Draw 's' to signal seconds
            // Note: symbol is 16px wide
            Graphics::drawChar(_xCenter-(16/2), _yCenter, 's', color, &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            break;
        case StatusIndicatorState::LEVEL:
            // Draw '%' to signal percentage
            // Note: symbol is 25px wide
            Graphics::drawChar(_xCenter-(25/2), _yCenter, '%', color, &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            break;
        case StatusIndicatorState::CLEAN:
            // Draw '!' to signal percentage
            // Note: symbol is 3px wide
            Graphics::drawChar(_xCenter-(3/2), _yCenter, '!', color, &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            break;
        case StatusIndicatorState::ERROR:
            // Draw '?' to signal error
            // Note: symbol is 14px wide
            Graphics::drawChar(_xCenter-(14/2), _yCenter, '?', color, &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            break;
        default:
            sout.err() << "StatusIndicator::hide encountered unknown state " <= _state;
            break;
    }

    _hidden = false;
}

void StatusIndicator::hide() {
    // Nothing to do if hidden already
    if(_hidden) return;

    // Draw current status symbol in black to remove it
    drawSymbol(BLACK);
    _hidden = true;
}

void StatusIndicator::showStatus(StatusIndicatorState newStatus, uint16_t color) {
    // Remove old symbol if not hidden
    if(!_hidden) hide();

    // Update state and draw new symbol
    _state = newStatus;
    drawSymbol(color);
    _hidden = false;
}