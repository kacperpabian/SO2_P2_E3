#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <mutex>
#include "PetrolStation.h"
#include "Car.h"

void makeCar(int carChar, 
PetrolStation *petrolStation);

void tankStation(PetrolStation *petrolStation);

int main()
{
    std::mutex * mtx;
    int carChar;
    PetrolStation *petrolStation = new PetrolStation(400, 5);
    std::vector<std::thread> threads;
    initscr();
 	noecho();
 	curs_set(FALSE);
    petrolStation->drawPetrolPoints();
    petrolStation->drawPetrolChamber();
    refresh();
    for (int i = 0; i < petrolStation->getStationsNumber(); i++)
    {
        refresh();
        petrolStation->stations[i].drawStation();
        refresh();
    }

    for (int i = 0; i<25;i++)
    {
        carChar = 97 + i;
        threads.push_back(std::thread(makeCar, carChar, petrolStation));
        if(i==24)
        {
            threads.push_back(std::thread(tankStation, petrolStation));
        }
        
    }
    for (auto& thread : threads) // for (int i = 0; i < 10; i++)
        thread.join(); 

    
    getch();
	endwin();
}

void makeCar(int carChar, 
PetrolStation *petrolStation)
{
    bool exists = false;
    while(!exists)
    {
        if (getch() == carChar)
    {
        Car car (5, 1, 60, 1, 1);
        car.carAll(petrolStation);
        exists = true;
    }
    }
    
}

void tankStation(PetrolStation *petrolStation)
{
    while(true)
    {
        petrolStation->tankPetrolSuperCar(petrolStation);
        
    }
   
}
