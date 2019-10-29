#ifndef INPUT_H
#define INPUT_H

//This class just defines the keys we want to use for now
//You have to set the state of this table manually!
class Input
{
public:
    Input();

    bool W{false};
    bool A{false};
    bool S{false};
    bool D{false};
    bool UP{false};     //Arrow keys
    bool DOWN{false};
    bool LEFT{false};
    bool RIGHT{false};
    bool Q{false};
    bool E{false};
    bool C{false};
    bool O{false};
    bool LSHIFT{false};
    bool SPACE{false};
    bool LMB{false};
    bool RMB{false};
    bool MMB{false};
    float MWHEEL{0.f};
    int MOUSEX{0};
    int MOUSEY{0};
};


#endif // INPUT_H
