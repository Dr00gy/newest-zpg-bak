#include "Subject.hpp"
#include "Observer.hpp"

void Subject::notify() {
    for (auto* observer : observers) {
        observer->update(this);
    }
}