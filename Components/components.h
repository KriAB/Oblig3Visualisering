#ifndef COMPONENTS_H
#define COMPONENTS_H

enum compType{
    MESH,
    MATERIAL,
    RENDER,
    SOUND,
    TRANSFORM,
    COLLISION,
    INPUTCOMP
};
class Components
{
protected:
Components();
Components(compType mType);
public:

    virtual ~Components();
    //virtual void receive(int message);// = 0;

    unsigned int eID; //entity ID

    compType getCompType(){return type;}

private:
    //Enum som sier comptype
    compType type;


};

#endif // COMPONENTS_H
