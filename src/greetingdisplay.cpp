#include <greetingdisplay.h>
#include <graphics.h>
#include <display.h>
#include <config.h>
#include <Roboto_Light20pt7b.h>
#include <pico/rand.h>

// Array of coffee-related greeting messages
const char* GreetingDisplay::greetings[] = {
    "You are my\nspecialty",
    "Brew-tiful\nmorning",
    "Stay\ngrounded",
    "Life happens,\ncoffee helps",
    "Espresso\nyourself",
    "Better latte\nthan never",
    "A latte love\nfor you",
    "Rise and\ngrind",
    "Sip happens",
    "Love at\nfirst sip",
    "You mocha\nme happy",
    "Depresso?\nHave espresso",
    "Bean there,\nbrewed that",
    "Coffee is\nalways good",
    "Caffeine &\nkindness"
};

const uint8_t GreetingDisplay::numGreetings = sizeof(greetings) / sizeof(greetings[0]);

GreetingDisplay::GreetingDisplay() :
    _xCenter(COUNTER_CENTER_X), _yCenter(COUNTER_CENTER_Y) {
    _hidden = true;
    _currentGreeting = 0;
}

void GreetingDisplay::drawGreeting(uint16_t color) {
    // Get the greeting text
    const char* text = greetings[_currentGreeting];
    
    // Parse the text to find newline and split into two lines
    const char* line1 = text;
    const char* line2 = nullptr;
    int line1_len = 0;
    
    // Find newline character
    for(int i = 0; text[i] != '\0'; i++) {
        if(text[i] == '\n') {
            line1_len = i;
            line2 = &text[i + 1];
            break;
        }
    }
    
    // If no newline found, it's a single line
    if(line2 == nullptr) {
        line1_len = 0;
        while(text[line1_len] != '\0') line1_len++;
    }
    
    // Calculate actual text width using glyph information
    auto calculateTextWidth = [](const char* str, int len) -> int {
        int width = 0;
        for(int i = 0; i < len; i++) {
            unsigned char c = str[i];
            if(c >= Roboto_Light20pt7b.first && c <= Roboto_Light20pt7b.last) {
                uint8_t glyphIndex = c - Roboto_Light20pt7b.first;
                width += Roboto_Light20pt7bGlyphs[glyphIndex].xAdvance;
            }
        }
        return width;
    };
    
    const int lineHeight = 32;
    
    if(line2 != nullptr) {
        // Two lines - draw them vertically centered
        int line2_len = 0;
        while(line2[line2_len] != '\0') line2_len++;
        
        int line1Width = calculateTextWidth(line1, line1_len);
        int line2Width = calculateTextWidth(line2, line2_len);
        
        // Draw first line above center
        int x1 = _xCenter - (line1Width / 2);
        int y1 = _yCenter - (lineHeight / 2);
        
        for(int i = 0; i < line1_len; i++) {
            unsigned char c = line1[i];
            Graphics::drawChar(x1, y1, c, color, 
                &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            if(c >= Roboto_Light20pt7b.first && c <= Roboto_Light20pt7b.last) {
                uint8_t glyphIndex = c - Roboto_Light20pt7b.first;
                x1 += Roboto_Light20pt7bGlyphs[glyphIndex].xAdvance;
            }
        }
        
        // Draw second line below center
        int x2 = _xCenter - (line2Width / 2);
        int y2 = _yCenter + (lineHeight / 2);
        
        for(int i = 0; i < line2_len; i++) {
            unsigned char c = line2[i];
            Graphics::drawChar(x2, y2, c, color, 
                &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            if(c >= Roboto_Light20pt7b.first && c <= Roboto_Light20pt7b.last) {
                uint8_t glyphIndex = c - Roboto_Light20pt7b.first;
                x2 += Roboto_Light20pt7bGlyphs[glyphIndex].xAdvance;
            }
        }
    } else {
        // Single line - draw centered
        int lineWidth = calculateTextWidth(line1, line1_len);
        int x = _xCenter - (lineWidth / 2);
        int y = _yCenter;
        
        for(int i = 0; i < line1_len; i++) {
            unsigned char c = line1[i];
            Graphics::drawChar(x, y, c, color, 
                &Roboto_Light20pt7b, Roboto_Light20pt7bGlyphs, Roboto_Light20pt7bBitmaps);
            if(c >= Roboto_Light20pt7b.first && c <= Roboto_Light20pt7b.last) {
                uint8_t glyphIndex = c - Roboto_Light20pt7b.first;
                x += Roboto_Light20pt7bGlyphs[glyphIndex].xAdvance;
            }
        }
    }
    
    _hidden = false;
}

void GreetingDisplay::showRandom(uint16_t color) {
    // Hide previous greeting if shown
    if(!_hidden) {
        hide();
    }
    
    // Select a random greeting
    _currentGreeting = get_rand_32() % numGreetings;
    
    // Draw the greeting
    drawGreeting(color);
}

void GreetingDisplay::hide() {
    if(_hidden) return;
    
    // Redraw in black to hide
    drawGreeting(BLACK);
    _hidden = true;
}

