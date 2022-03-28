#include "Graph.h"
#include "GraphExceptions.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Literally do nothing here
// default constructors of the std::vector is enough
Graph::Graph()
{}

Graph::Graph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[3];

    std::ifstream mapFile(filePath.c_str());
    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly three tokens (Meaning it is an edge)
        else if(i == 3)
        {
            int weight = std::atoi(tokens[0].c_str());
            if(!ConnectVertices(tokens[1], tokens[2], weight))
            {
                std::cerr << "Duplicate edge on "
                          << tokens[0] << "-"
                          << tokens[1] << std::endl;
            }
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void Graph::InsertVertex(const std::string& vertexName)
{
    struct GraphVertex newVertex, i;
    newVertex.edgeCount = 0;
    newVertex.name = vertexName;
    int vertlistsize = vertexList.size();
    for (int j = 0; j < vertlistsize; j++)
    {
        i = vertexList[j];
        if (i.name == vertexName) throw DuplicateVertexNameException();
    }
    vertexList.push_back(newVertex);
    // TODO
}

bool Graph::ConnectVertices(const std::string& fromVertexName,
                            const std::string& toVertexName,
                            int weight)
{

    if(fromVertexName == toVertexName) return false;
    struct GraphEdge newEdge;
    int vertsize, edgesize, fromID = -1, toID = -1;
    int i;
    edgesize = edgeList.size();
    vertsize = vertexList.size();
    for (i = 0; i < vertsize; i++)
    {
        if (fromVertexName == vertexList[i].name)
        {
            fromID = i;
        }
        else if(toVertexName == vertexList[i].name)
        {
            toID = i;
        }
    } 
    if(fromID == -1 || toID == -1)  
    {
        throw VertexNotFoundException();
    }
    if((vertexList[fromID].edgeCount >= MAX_EDGE_PER_VERTEX) || (vertexList[toID].edgeCount >= MAX_EDGE_PER_VERTEX))
    {
        throw TooManyEdgeOnVertexExecption();
    }
    for(i = 0; i < edgesize; i++)
    {
        if(((edgeList[i].vertexId0 == fromID) && (edgeList[i].vertexId1 == toID)) || ((edgeList[i].vertexId0 == toID) && (edgeList[i].vertexId1 == fromID)))
        {
            return false;
        }
    }
    newEdge.masked = false;
    newEdge.vertexId0 = fromID;
    newEdge.vertexId1 = toID;
    newEdge.weight = weight;
    edgeList.push_back(newEdge);
    vertexList[fromID].edgeCount++;
    vertexList[toID].edgeCount++;
    vertexList[fromID].edgeIds[vertexList[fromID].edgeCount-1] = edgesize;
    vertexList[toID].edgeIds[vertexList[toID].edgeCount-1] = edgesize;
    return true;
    // TODO
}

bool Graph::ShortestPath(std::vector<int>& orderedVertexIdList,
                         const std::string& from,
                         const std::string& to) const
{
    int i, fromID = -1, toID = -1;
    int vertsize, edgesize;
    vertsize = vertexList.size();
    edgesize = edgeList.size();
    for(i = 0; i < vertsize; i++)
    {
        if(from == vertexList[i].name)
            fromID = i;
        else if(to == vertexList[i].name)
            toID = i;
    }
    if((toID == -1) || (fromID == -1)) throw VertexNotFoundException();
    std::priority_queue<DistanceVertexIdPair> pq, temppq;
    int disArr[10000];
    int prevArr[10000];
    int lockArr[10000];
    for (i = 0; i < vertsize; i++)
    {
        struct DistanceVertexIdPair temp(0, 0);
        temp.vId = i;
        if (i == fromID) temp.currentWeight = 0;
        else temp.currentWeight = LARGE_NUMBER;
        prevArr[i] = -1;
        disArr[i] = temp.currentWeight;
        pq.push(temp);
    }
    int tempTo = -1, tempFrom = -1;
    while(!pq.empty())
    {
        struct DistanceVertexIdPair v = pq.top();
        pq.pop();
        if(v.currentWeight == LARGE_NUMBER) break;
        for (i = 0; i < edgesize; i++)
        {
            if(((edgeList[i].vertexId0 == v.vId) || (edgeList[i].vertexId1 == v.vId)) && (edgeList[i].masked == false)) 
            {
                tempTo = (edgeList[i].vertexId0 == v.vId) ? (edgeList[i].vertexId1) : (edgeList[i].vertexId0);
                tempFrom = v.vId;
                int new_dist;
                new_dist = edgeList[i].weight + disArr[tempFrom];
                if(new_dist < disArr[tempTo])
                {
                    disArr[tempTo] = new_dist;
                    prevArr[tempTo] = tempFrom;
                    struct DistanceVertexIdPair apo(-1,-1);
                    apo.currentWeight = new_dist;
                    apo.vId = tempTo;
                    pq.push(apo);
                }
            }
        }
    }
    std::vector<int> tempVector;
    if(prevArr[toID] == -1) return false;
    tempVector.push_back(toID);
    int yol = prevArr[toID];
    if(prevArr[toID] == fromID)
    {
        orderedVertexIdList.push_back(fromID);
        orderedVertexIdList.push_back(toID);
        return true;
    }
    do
    {
        tempVector.push_back(yol);
        yol = prevArr[yol];
    } while (yol != fromID);
    tempVector.push_back(fromID);
    for(i = tempVector.size()-1; i >= 0; i--)
    {
        orderedVertexIdList.push_back(tempVector[i]);
    }
    return true;
    // TODO
}
int Graph::findvertexID(const std::string& name)
{
    for(int i = 0; i < vertexList.size(); i++)
    {
        if(vertexList[i].name == name)
        {
            return i;
        }
    }
    throw VertexNotFoundException();
}

void Graph::MaskTheHighestEdge(std::vector<int> orderedVertexIdList, const std::string& from,const std::string& to)
{
    int v1id, v2id;
    int MaxEdge = -1;
    int TempEdgeId = -1;
    int MaxValue = -1;
    int v1p=0, v2p=1;
    GraphVertex v1, v2;
    if(orderedVertexIdList.size() == 2)
    {
        v1 = vertexList[findvertexID(from)];
        v2 = vertexList[findvertexID(to)];
        int j = 0, k = 0, apo;
        for(j = 0; j < v2.edgeCount; j++)
        {
            for(k = 0; k < v1.edgeCount; k++)
            {
                if(v1.edgeIds[k] == v2.edgeIds[j])
                {
                    apo = v1.edgeIds[k];
                    edgeList[apo].masked = true;
                    return;
                }
            }
        }
        return;
    }
    do 
    {
        v1 = vertexList[orderedVertexIdList[v1p++]];
        v2 = vertexList[orderedVertexIdList[v2p++]];
        int j = 0, k = 0;
        for(j = 0; j < v2.edgeCount; j++)
        {
            for(k = 0; k < v1.edgeCount; k++)
            {
                if(v1.edgeIds[k] == v2.edgeIds[j])
                {
                    TempEdgeId = v1.edgeIds[k];
                }
            }
        }
        if(edgeList[TempEdgeId].weight > MaxValue)
        {
            MaxValue = edgeList[TempEdgeId].weight;
            MaxEdge = TempEdgeId;
        }
    }   while(v2.name != to);
    edgeList[MaxEdge].masked = true;
}

int Graph::MultipleShortPaths(std::vector<std::vector<int> >& orderedVertexIdList,
                              const std::string& from,
                              const std::string& to,
                              int numberOfShortestPaths)
{
    bool pathExist = 0;
    int i;
    std::vector<int> temp;
    UnMaskAllEdges();
    pathExist = ShortestPath(temp, from, to);
    if(!pathExist) return 0;
    orderedVertexIdList.push_back(temp);
    MaskTheHighestEdge(temp ,from, to);
    for (i = 1; i < numberOfShortestPaths; i++)
    {
        std::vector<int> temppath;
        while(!temppath.empty()) temppath.pop_back();
        pathExist = ShortestPath(temppath, from, to);
        if(!pathExist) break;
        orderedVertexIdList.push_back(temppath);
        MaskTheHighestEdge(temppath, from, to);
    }
    UnMaskAllEdges();
    return i;
    // TODO
}

void Graph::MaskEdges(const std::vector<StringPair>& vertexNames)
{
    int fromID = -1, toID = -1, i, j, flag1 = 0, flag2 = 0;
    for(j = 0; j < vertexNames.size(); j++)
    {
        flag1 = 1, flag2 = 1;
        for (i = 0; i < edgeList.size(); i++)
        {
            if((vertexList[edgeList[i].vertexId0].name == vertexNames[j].s0) || (vertexList[edgeList[i].vertexId1].name == vertexNames[j].s0))
                flag1 = 0;
            if((vertexList[edgeList[i].vertexId0].name == vertexNames[j].s1) || (vertexList[edgeList[i].vertexId1].name == vertexNames[j].s1))
                flag2 = 0;
            if ((vertexList[edgeList[i].vertexId0].name == vertexNames[j].s0) && (vertexList[edgeList[i].vertexId1].name == vertexNames[j].s1))
            {
                edgeList[i].masked = true;
            }
            else if((vertexList[edgeList[i].vertexId1].name == vertexNames[j].s0) && (vertexList[edgeList[i].vertexId0].name == vertexNames[j].s1))
            {
                edgeList[i].masked = true;
            }
        } 
        if((flag1 == 1) || (flag2 == 1)) throw VertexNotFoundException();
    }
    // TODO
}

void Graph::UnMaskEdges(const std::vector<StringPair>& vertexNames)
{
    int fromID = -1, toID = -1, i, j, flag1 = 0, flag2 = 0;
    for(j = 0; j < vertexNames.size(); j++)
    {
        flag1 = 1, flag2 = 1;
        for (i = 0; i < edgeList.size(); i++)
        {
            if((vertexList[edgeList[i].vertexId0].name == vertexNames[j].s0) || (vertexList[edgeList[i].vertexId1].name == vertexNames[j].s0))
                flag1 = 0;
            if((vertexList[edgeList[i].vertexId0].name == vertexNames[j].s1) || (vertexList[edgeList[i].vertexId1].name == vertexNames[j].s1))
                flag2 = 0;
            if ((vertexList[edgeList[i].vertexId0].name == vertexNames[j].s0) && (vertexList[edgeList[i].vertexId1].name == vertexNames[j].s1))
            {
                edgeList[i].masked = false;
            }
            else if((vertexList[edgeList[i].vertexId1].name == vertexNames[j].s0) && (vertexList[edgeList[i].vertexId0].name == vertexNames[j].s1))
            {
                edgeList[i].masked = false;
            }
        } 
        if((flag1 == 1) || (flag2 == 1)) throw VertexNotFoundException();
    }
    // TODO
}

void Graph::UnMaskAllEdges()
{
    int i;
    for(i = 0; i < edgeList.size(); i++)
    {
        edgeList[i].masked = false;
    }
    // TODO
}

void Graph::MaskVertexEdges(const std::string& name)
{
    int id = -1;
    for(int i = 0; i < vertexList.size(); i++)
    {
        if(name == vertexList[i].name)
            id = i;
    }
    if(id == -1) throw VertexNotFoundException();
    for(int i = 0; i < edgeList.size(); i++)
    {
        if((edgeList[i].vertexId0 == id) || (edgeList[i].vertexId1 == id))
            edgeList[i].masked = true;
    }
    // TODO
}

void Graph::UnMaskVertexEdges(const std::string& name)
{
    int id = -1;
    for(int i = 0; i < vertexList.size(); i++)
    {
        if(name == vertexList[i].name)
            id = i;
    }
    if(id == -1) throw VertexNotFoundException();
    for(int i = 0; i < edgeList.size(); i++)
    {
        if((edgeList[i].vertexId0 == id) || (edgeList[i].vertexId1 == id))
            edgeList[i].masked = false;
    }
    // TODO
}

void Graph::ModifyEdge(const std::string& vName0,
                       const std::string& vName1,
                       float newWeight)
{
    int vertsize, edgesize, fromID = -1, toID = -1;
    int i;
    edgesize = edgeList.size();
    vertsize = vertexList.size();
    for (i = 0; i < vertsize; i++)
    {
        if (vName0 == vertexList[i].name)
        {
            fromID = i;
        }
        else if(vName1 == vertexList[i].name)
        {
            toID = i;
        }
    } 
    if(fromID == -1 || toID == -1)  
    {
        throw VertexNotFoundException();
    }
    for(i = 0; i < edgesize; i++)
    {
        if(((edgeList[i].vertexId0 == fromID) && (edgeList[i].vertexId1 == toID)) || ((edgeList[i].vertexId0 == toID) && (edgeList[i].vertexId1 == fromID)))
        {
            edgeList[i].weight = newWeight;
        }
    }
    // TODO
}

void Graph::ModifyEdge(int vId0, int vId1,
                       float newWeight)
{
    if(vId0>=vertexList.size() || vId1 >=vertexList.size()-1) return;
    ModifyEdge(vertexList[vId0].name, vertexList[vId1].name, newWeight);
    // TODO
}

void Graph::PrintAll() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(int j = 0; j < v.edgeCount; j++)
        {
            int edgeId = v.edgeIds[j];
            const GraphEdge& edge = edgeList[edgeId];
            // Skip printing this edge if it is masked
            if(edge.masked)
                continue;

            // List the all vertex names and weight
            std::cout << "-" << std::setfill('-')
                             << std::setw(2) << edge.weight
                             << "-> ";
            int neigVertexId = (static_cast<int>(i) == edge.vertexId0)
                                 ? edge.vertexId1
                                 : edge.vertexId0;
            std::cout << vertexList[neigVertexId].name << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void Graph::PrintPath(const std::vector<int>& orderedVertexIdList,
                      bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    for(size_t i = 0; i < orderedVertexIdList.size(); i++)
    {
        int vertexId = orderedVertexIdList[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == orderedVertexIdList.size() - 1) break;
        int nextVertexId = orderedVertexIdList[i + 1];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        // Find the edge between these two vertices
        int edgeId = INVALID_INDEX;
        if(vertexId     < static_cast<int>(vertexList.size()) &&
           nextVertexId < static_cast<int>(vertexList.size()))
        {
            // Check all of the edges of vertex
            // and try to find
            const GraphVertex& fromVert = vertexList[vertexId];
            for(int i = 0; i < fromVert.edgeCount; i++)
            {
                int eId = fromVert.edgeIds[i];
                // Since the graph is not directional
                // check the both ends
                if((edgeList[eId].vertexId0 == vertexId &&
                    edgeList[eId].vertexId1 == nextVertexId)
                ||
                   (edgeList[eId].vertexId0 == nextVertexId &&
                    edgeList[eId].vertexId1 == vertexId))
                {
                    edgeId = eId;
                    break;
                }
            }
        }
        if(edgeId != INVALID_INDEX)
        {
            const GraphEdge& edge = edgeList[edgeId];
            std::cout << "-" << std::setfill('-')
                      << std::setw(2)
                      << edge.weight << "->";
        }
        else
        {
            std::cout << "-##-> ";
        }
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

int Graph::TotalVertexCount() const
{
    return vertexList.size();
    // TODO
}

int Graph::TotalEdgeCount() const
{
    return edgeList.size();
    // TODO
}

std::string Graph::VertexName(int vertexId) const
{
    if(vertexId >= vertexList.size()) return "";
    return vertexList[vertexId].name;
    // TODO
}

int Graph::TotalWeightInBetween(std::vector<int>& orderedVertexIdList)
{
    int v1id, v2id;
    int MaxEdge = -1;
    int TempEdgeId = -1;
    int MaxValue = -1;
    int v1p=0, v2p=1;
    std::string from, to;
    from = vertexList[orderedVertexIdList[0]].name;
    to = vertexList[orderedVertexIdList[orderedVertexIdList.size()-1]].name;
    GraphVertex v1, v2;
    int ans = 0;
    if(orderedVertexIdList.size() == 2)
    {
        v1 = vertexList[findvertexID(from)];
        v2 = vertexList[findvertexID(to)];
        int j = 0, k = 0, apo;
        for(j = 0; j < v2.edgeCount; j++)
        {
            for(k = 0; k < v1.edgeCount; k++)
            {
                if(v1.edgeIds[k] == v2.edgeIds[j])
                {
                    apo = v1.edgeIds[k];
                    return edgeList[apo].weight;
                }
            }
        }
        return edgeList[apo].weight;
    }
    do 
    {
        v1 = vertexList[orderedVertexIdList[v1p++]];
        v2 = vertexList[orderedVertexIdList[v2p++]];
        int j = 0, k = 0;
        for(j = 0; j < v2.edgeCount; j++)
        {
            for(k = 0; k < v1.edgeCount; k++)
            {
                if(v1.edgeIds[k] == v2.edgeIds[j])
                {
                    TempEdgeId = v1.edgeIds[k];
                    ans+=edgeList[TempEdgeId].weight;
                }
            }
        }
        if(edgeList[TempEdgeId].weight > MaxValue)
        {
            MaxValue = edgeList[TempEdgeId].weight;
            MaxEdge = TempEdgeId;
        }
    }   while(v2.name != to);
    return ans;
    // TODO
}