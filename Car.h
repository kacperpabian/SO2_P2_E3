#include <ncurses.h>
#include "vector"
#include <thread>
#include <mutex>

std::mutex mtx3;
std::mutex mtx4;
std::condition_variable cv2;

class Car 
{
    public:
        int fuel;
        int type;
        int capacity;
        int positionX;
        int positionY;
        

    Car (int fuel, int type, int capacity, int positionX, int positionY)
    {
        this->fuel = fuel;
        this->type = type;
        this->capacity = capacity;
        this->positionX = positionX;
        this->positionY = positionY;
    }
    Car()
    {

    }

    void setPositionX(int newPositionX)
    {
        positionX = newPositionX;
    }

    void setPositionY(int newPositionY)
    {
        positionY = newPositionY;
    }

    int getPositionX()
    {
        return positionX;
    }
    int getPositionY()
    {
        return positionY;
    }

    int getCapacity()
    {
        return capacity;
    }

    void setFuel(int newFuel)
    {
        fuel = newFuel;
    }

    int getFuel()
    {
        return fuel;
    }

    void carAll(PetrolStation *PetrolStation)
    {
        int index;
        drawCar(PetrolStation);
        if(getPositionX() == 45)
            index = 0;
        else if(getPositionX() == 50)
            index = 1;
        else if(getPositionX() == 55)
            index = 2;
        else if(getPositionX() == 60)
            index = 3;
        else if(getPositionX() == 65)
            index = 4;
        tankCar(PetrolStation, index);
    }
    void drawCar(PetrolStation *petrolStation)
    {
        
        int lockedNumber = 0;
        bool stationed = false;
        int indexStation;
        int prevX1, prevY1;
        
        
        petrolStation->setCarsNumber(petrolStation->getCarsNumber() + 1);

        std::unique_lock<std::mutex> locker(mtx3);

        refresh();
        mvprintw(getPositionY(), getPositionX(), "OOOOO");
        mvprintw(getPositionY()+1, getPositionX(), "OOOOO");
        mvprintw(getPositionY()+2, getPositionX()+1, "o o");


        if(petrolStation->getCarsNumber() >=6)
        {
            cv2.wait(locker);
        }
        
        
        for(int i = 0; i< petrolStation->stations.size(); i++ )
        {
            if(petrolStation->stations[i].getOccupied() == true)
            {
                lockedNumber++;
            }
        }
        if(lockedNumber==petrolStation->stations.size())
        {
            petrolStation->checkFree = false;
        }
        else
        {
            petrolStation->checkFree = true;
        }

        if(petrolStation->checkFree == false)
        {
            cv2.wait(locker);
        }

        if(petrolStation->getPetrol() < 200)
        {
            std::unique_lock<std::mutex> locker2(petrolStation->mtx);
            petrolStation->cv.wait(locker2);
        }


        while(!stationed)
        {
            
            usleep(20000);
            prevX1 = getPositionX();
            prevY1 = getPositionY();
            setPositionX(getPositionX()+1);
            mvprintw(prevY1, prevX1, " ");
            mvprintw(prevY1+1, prevX1, " ");
            mvprintw(prevY1+2, prevX1+1, " ");
            mvprintw(getPositionY(), getPositionX(), "OOOOO");
            mvprintw(getPositionY()+1, getPositionX(), "OOOOO");
            mvprintw(getPositionY()+2, getPositionX()+1, "o o");
            refresh();
            
            if(getPositionX() == 45 && getPositionY() == 1)
            {
                stationed = true;
                petrolStation->stations[0].setOccupied(true);
                break;
                
            }
                
            else if(getPositionX() == 50 && getPositionY() == 6)
            {
                stationed = true;
                petrolStation->stations[1].setOccupied(true);
                break;
            }
            else if(getPositionX() == 55 && getPositionY() == 11)
            {
                stationed = true;
                petrolStation->stations[2].setOccupied(true);
                break;
            }
            else if(getPositionX() == 60 && getPositionY() == 16)
            {
                stationed = true;
                petrolStation->stations[3].setOccupied(true);
                break;
            }
            else if(getPositionX() == 65 && getPositionY() == 21)
            {
                stationed = true;
                petrolStation->stations[4].setOccupied(true);
                break;
            }

            repositionCar(petrolStation, 40, 6, prevX1, prevY1, 0);
            repositionCar(petrolStation, 45, 11, prevX1, prevY1, 1);
            repositionCar(petrolStation, 50, 16, prevX1, prevY1, 2);
            repositionCar(petrolStation, 55, 21, prevX1, prevY1, 3);

            

        }
        if(petrolStation->checkFree == true)
        {
            mtx3.unlock();
        }
        
        
    }


    void repositionCar(PetrolStation *petrolStation, int posX, 
    int posY, int prevX1, int prevY1, int stationIndex)
    {
        if(getPositionX() == posX && petrolStation->stations[stationIndex].getOccupied() == true)
            {
                bool nextTrack = false;
                while(!nextTrack)
                {
                    usleep(30000);
                    prevY1 = getPositionY();
                    setPositionY(getPositionY() + 1);
                    // std::unique_lock<std::mutex> locker(mtx3);
                    mvprintw(prevY1, prevX1, "      ");;
                    mvprintw(getPositionY(), getPositionX(), "OOOOO");
                    mvprintw(getPositionY()+1, getPositionX(), "OOOOO");
                    mvprintw(getPositionY()+2, getPositionX()+1, "o o");
                    refresh();
                    // locker.unlock();
                    if(getPositionY() == posY)
                        nextTrack = true;
                }
            }
    }

    void tankCar(PetrolStation *petrolStation, int index)
    {
        
        if(petrolStation->checkFree == false)
        {
            std::unique_lock<std::mutex> lock(mtx3);
        }
            
        while(getFuel()<getCapacity())
        {
            usleep(200000);
            setFuel(getFuel()+1);
            petrolStation->setPetrol(petrolStation->getPetrol()-1);
            petrolStation->drawPetrolPoints();
        }
        removeCar(petrolStation);
        petrolStation->stations[index].setOccupied(false);

        cv2.notify_one();

        bool occupied = false;
        for(int i = 0; i< petrolStation->stations.size(); i++ )
        {
            if(petrolStation->stations[i].getOccupied() == true)
            {
                occupied = true;
            }
        }
        if(!occupied)
        {
            petrolStation->cv.notify_one();
        }
        
    
    }

    void removeCar(PetrolStation *petrolStation)
    {
        petrolStation->setCarsNumber(petrolStation->getCarsNumber() - 1);
        mvprintw(getPositionY(), getPositionX(), "     ");
        mvprintw(getPositionY()+1, getPositionX(), "     ");
        mvprintw(getPositionY()+2, getPositionX()+1, "   ");
        refresh();
    }
};