#include <serialutils.h>

Serout sout;

Serout& Serout::warn() {
    Serial.print("[WARNING] ");
    return sout;
};

Serout& Serout::err() {
    Serial.print("[ERROR] ");
    return sout;
}

Serout& Serout::info() {
    Serial.print("[INFO] ");
    return sout;
}