#pragma once

class Task
{

public:
    Task(int x, int y) : _x(x),
                         _y(y)
    {
    }
    void Excute()
    {
        _result = _x + _y;
    }
    int x(){return _x;}
    int y(){return _y;}
    int Result ()
    {
        return _result;
    }
    ~Task();

private:
    int _x;
    int _y;
    int _result;
};
