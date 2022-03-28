#include "METUMaps.h"
#include "GraphExceptions.h"

#include <iostream>

void METUMaps::PrintNotInJourney() const
{
    std::cout << "Device is not in a journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeDestination() const
{
    std::cout << "Cannot change Destination during journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeStartingLoc() const
{
    std::cout << "Cannot change Starting Location during journey!" << std::endl;
}

void METUMaps::PrintAlreadyInJourney() const
{
    std::cout << "Device is already in a journey!" << std::endl;
}

void METUMaps::PrintJourneyIsAlreadFinished() const
{
    std::cout << "Journey is already finished!" << std::endl;
}

void METUMaps::PrintLocationNotFound() const
{
    std::cout << "One (or both) of the locations are not found in the maps!" << std::endl;
}

void METUMaps::PrintJourneyCompleted() const
{
    std::cout << "Journey Completed!" << std::endl;
}

void METUMaps::PrintCachedLocationFound(const std::string& location0,
                                        const std::string& location1) const
{
    std::cout << "Route between \""
              << location0 << "\" and \""
              << location1 << "\" is in cache, using that..."
              << std::endl;
}

void METUMaps::PrintCalculatingRoutes(const std::string& location0,
                                      const std::string& location1) const
{
    std::cout << "Calculating Route(s) between \""
              << location0 << "\" and \""
              << location1 << "\"..."
              << std::endl;
}

std::string METUMaps::GenerateKey(const std::string& location0,
                                  const std::string& location1)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    return location0 + "/" + location1;
}

METUMaps::METUMaps(int potentialPathCount,
                   const std::string& mapFilePath)
{
    map = Graph(mapFilePath);
    this->potentialPathCount = potentialPathCount;
    cachedPaths = KeyedHashTable(potentialPathCount * map.TotalVertexCount());
    inJourney = 0;
    /*int             potentialPathCount;
    bool                inJourney;
    std::string         startingLoc;
    std::string         currentLoc;
    std::string         destination;
    std::vector<int>    currentRoute; */
    // TODO
}

void METUMaps::SetDestination(const std::string& name)
{
    if(inJourney) 
    {
        PrintAlreadyInJourney();
    }
    else
    {
        destination = name;
    }
    // TODO
}

void METUMaps::SetStartingLocation(const std::string& name)
{
    if(inJourney) 
    {
        PrintAlreadyInJourney();
    }
    else
    {
        startingLoc = name;
    }
    // TODO
}

void METUMaps::StartJourney()
{
    std::vector<std::vector<int> > temp;
    int existingPathNum;
    if(inJourney) 
    {
        PrintAlreadyInJourney();
        return;
    }
    PrintCalculatingRoutes(startingLoc, destination);
    try{
    existingPathNum = map.MultipleShortPaths(temp, startingLoc, destination, potentialPathCount);}
    catch(VertexNotFoundException)
    {
        PrintLocationNotFound();
        return ;
    }
    currentLoc = startingLoc;
    std::string stra, end;
    std::vector<int> yol;
    stra = startingLoc;
    end = destination;
    int i;
    for(i = 0; i < existingPathNum; i++)
    {
        yol = temp[i];
        int j;
        std::string hashstr;
        int hashnum;
            while(yol.size()!=1)
            {
                stra = map.VertexName(yol[0]);
                hashstr = GenerateKey(stra, end);
                cachedPaths.Insert(hashstr, yol);
                yol.erase(yol.begin()); 
            }
    }
    currentRoute = temp[0];
    inJourney = 1;
    // TODO
}

void METUMaps::EndJourney()
{
    if(!inJourney) PrintJourneyIsAlreadFinished();
    else
    {
        cachedPaths.ClearTable();
        destination = "";
        currentLoc = "";
        startingLoc = "";
        inJourney = false;
    }
    // TODO
}

void METUMaps::UpdateLocation(const std::string& name)
{
    if(!inJourney) 
    {
        PrintNotInJourney();
        return;
    }
    if(name == destination)
    {
        PrintJourneyCompleted();
        return ;
    }
    std::string hashstr;
    std::vector<int> temp;
    bool formerPathExist = false;
    hashstr = GenerateKey(name, destination);
    formerPathExist = cachedPaths.Find(temp, hashstr);
    
    if(formerPathExist) 
    {
        PrintCachedLocationFound(name, destination);
        std::vector <int> newpath;
        int sizi = temp.size();
        for(int i = sizi-1; i >= 0; i--)
        {
            newpath.push_back(temp[i]);
        }
        currentRoute = temp;
        currentLoc = name;
    }
    else
    {
        std::vector<std::vector<int> > tempp;
        int existingPathNum;
        PrintCalculatingRoutes(name, destination);
        try{
        existingPathNum = map.MultipleShortPaths(tempp, name, destination, potentialPathCount);}
        catch(VertexNotFoundException)
        {
            PrintLocationNotFound();
            return ;
        }
        currentLoc = name;
        std::string stra, end;
        std::vector<int> yol;
        stra = name;
        end = destination;
        int i;
        for(i = 0; i < existingPathNum; i++)
        {
            yol = tempp[i];
            int j;
            std::string hashstr;
            int hashnum;
            while(yol.size()!=1)
            {
                stra = map.VertexName(yol[0]);
                hashstr = GenerateKey(stra, end);
                cachedPaths.Insert(hashstr, yol);
                yol.erase(yol.begin()); 
            }
        }
        currentRoute = tempp[0];
        inJourney = 1;
    }


    // TODO
}

void METUMaps::Display()
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    if(!inJourney)
    {
        // We are not in journey, this function
        // shouldn't be called
        PrintNotInJourney();
        return;
    }

    int timeLeft = map.TotalWeightInBetween(currentRoute);

    // Print last known location
    std::cout << "Journey         : " << startingLoc << "->"
              << destination << "\n";
    std::cout << "Current Location: " << currentLoc << "\n";
    std::cout << "Time Left       : " << timeLeft << " minutes\n";
    std::cout << "Current Route   : ";
    map.PrintPath(currentRoute, true);

    std::cout.flush();
}