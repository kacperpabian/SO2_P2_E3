#include <ncurses.h>

class Station
{
    public:
        int stationIndex;
        bool occupied = false;
        int positionX;
        int positionY;
    
    Station (int stationIndex, int positionX, int positionY)
    {
        this->stationIndex = stationIndex;
        this->positionX = positionX;
        this->positionY = positionY;
    }

    int getStationIndex()
    {
        return stationIndex;
    }
    int getPositionX()
    {
        return positionX;
    }
    int getPositionY()
    {
        return positionY;
    }
    bool getOccupied()
    {
        return occupied;
    }
    void setPositionX(int newPositionX)
    {
        positionX = newPositionX;
    }
    void setPositionY(int newPositionY)
    {
        positionY = newPositionY;
    }
    void setOccupied(bool newOccupied)
    {
        occupied = newOccupied;
    }
    void drawStation()
    {
        refresh();
        mvprintw(getPositionY(), getPositionX(), "ooo");
        mvprintw(getPositionY()+1, getPositionX()+1, "4");
        mvprintw(getPositionY()+2, getPositionX(), "_|_");
    }
};