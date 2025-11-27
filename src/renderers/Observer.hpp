#pragma once

class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Subject* subject) = 0;
};