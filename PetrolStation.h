#include "vector"
#include "Station.h"
#include <condition_variable>



class PetrolStation 
{
    public:
        int petrol;
        int stationsNumber;
        int carsNumber = 0;
        std::mutex mtx;
        std::mutex mtx2;
        std::condition_variable cv;
        std::vector <Station> stations;
        bool checkFree = true;
        

    PetrolStation (int petrol, int stationsNumber)
    {
        this->petrol = petrol;
        this->stationsNumber = stationsNumber;
        
        initStations();
    }

    int getPetrol()
    {
        return petrol;
    }

    int getStationsNumber()
    {
        return stationsNumber;
    }

    int setCarsNumber(int number)
    {
        carsNumber = number;
    }

    int getCarsNumber()
    {
        return carsNumber;
    }

    void setPetrol(int newPetrol)
    {
        petrol = newPetrol;
    }

    void initStations()
    {
        int statx=50, staty=1;
        for (int i = 0; i<getStationsNumber(); i++)
        {
            Station station(i, statx, staty);
            stations.push_back(station);
            statx += 5;
            staty += 5;
        }
    }

    void drawPetrolChamber()
    {
        refresh();
        mvprintw(7, 70, "___________________");
        mvprintw(8, 70, "|                 |");
        mvprintw(9, 70, "|       GAS       |");
        mvprintw(10, 70, "|                 |");
        mvprintw(11, 70, "|                 |");
        mvprintw(12, 70, "___________________");
    }

    void drawPetrolPoints()
    {
        std::unique_lock<std::mutex> lock2(mtx2);
        refresh();
        mvprintw(6, 78, "%d", getPetrol());
        lock2.unlock();
    }

    void tankPetrolSuperCar(PetrolStation *petrolStation)
    {
        int posy = 8, posx = 120, prevy, prevx;
        bool inStation = false;

        if(getPetrol() < 200)
        {

        std::unique_lock<std::mutex> lock2(mtx2);
        refresh();
        mvprintw(posy, posx, "    __   __________ ");
        mvprintw(posy+1, posx, "  _//]| |          |");
        mvprintw(posy+2, posx, " |____|-|__________|");
        mvprintw(posy+3, posx, "    O      O     O  ");
        lock2.unlock();


        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);

        while(!inStation)
        {
            
            usleep(20000);
            prevx = posx;
            posx --;
            std::unique_lock<std::mutex> lock2(mtx2);
            refresh();
            mvprintw(posy, prevx, "                    ");
            mvprintw(posy+1, prevx, "                    ");
            mvprintw(posy+2, prevx, "                    ");
            mvprintw(posy+3, prevx, "                    ");
            mvprintw(posy, posx, "    __   __________ ");
            mvprintw(posy+1, posx, "  _//]| |          |");
            mvprintw(posy+2, posx, " |____|-|__________|");
            mvprintw(posy+3, posx, "    O      O     O  ");
            refresh();
            lock2.unlock();
            if(posx == 92)
            {
                inStation = true;
                while(petrolStation->getPetrol() != 400)
                {
                    petrolStation->setPetrol(petrolStation->getPetrol()+1);
                    petrolStation->drawPetrolPoints();
                    usleep(20000);
                }
                std::unique_lock<std::mutex> lock2(mtx2);
                mvprintw(posy, posx, "                    ");
                mvprintw(posy+1, posx, "                    ");
                mvprintw(posy+2, posx, "                    ");
                mvprintw(posy+3, posx, "                    ");
                lock2.unlock();
            }
            
            
        }
        cv.notify_one();

    }
        }

        
};