#pragma once
#include <vector>
#include <algorithm>

class Observer;

class Subject {
public:
    enum class ChangeType {
        VIEW,
        PROJECTION
    };

    virtual ~Subject() = default;
    
    void attach(Observer* observer) {
        observers.push_back(observer);
    }
    
    void detach(Observer* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    
    void notify();
    
    ChangeType getLastChangeType() const { return lastChangeType; }
    
protected:
    std::vector<Observer*> observers;
    ChangeType lastChangeType = ChangeType::VIEW;
};