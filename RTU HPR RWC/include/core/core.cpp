#ifndef STATE_H
#define STATE_H

class State {
    public:
        virtual ~State() {}

        virtual void start();
};

#endif // STATE_H