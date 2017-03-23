//
//  main.cpp
//  VertexCover
//
//  Created by X on 15/5/6.
//  Copyright (c) 2015 X. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;

void vertexCover();
vector<int> getMinimal(vector<vector<int> > neighbors, vector<int> cover);
vector<int> getCross(vector<vector<int> > neighbors, vector<int> cover, int k);
int coverSize(vector<int> cover);
int maxRem(vector<vector<int> > neighbors, vector<int> cover);


int main(int argc, const char * argv[]) {
    int round;
    cin >> round;
    
    while (round > 0) {
        vertexCover();
        round --;
    }
    
    return 0;
}

void vertexCover()
{
    //Initialise and Read Data
    int v, e, k;
    cin >> v >> e >> k;
    
    vector<vector<short> > adjacency;
    vector<short> row(v, (short)0);
    
    for (int i=0; i<v; i++) {
        adjacency.push_back(row);
    }
    
    int tmp1 = 0, tmp2 = 0;
    for (int i=0; i<e; i++) {
        cin >> tmp1 >> tmp2;
        tmp1--;
        tmp2--;
        adjacency[tmp1][tmp2] = (short)1;
        adjacency[tmp2][tmp1] = (short)1;
    }
    
    
    //Review
    /*
    cout << "\n\nv=" << v << ", e=" << e << ", k=" << k << "\n";
    
    cout << "\n\nThe matrix is:";
    for (int i=0; i<v; i++) {
        cout << "\n" << (i+1) << "\t:";
        for (int j=0; j<v; j++) {
            cout << adjacency[i][j]<< " ";
        }
    }
    cout << "\n---Read Complete---\n\n";
    */
    
    
    //Algorithm
    //Remember: in the algorithm the vertex starts with the number 0, not 1. This is different from the origional data.
    
    //Build Neighbors
    vector<vector<int> > neighbors;
    for(int i=0; i<v; i++)
    {
        vector<int> neighbor;
        for(int j=0; j<v; j++)
            if(adjacency[i][j]==1)
                neighbor.push_back(j);
        neighbors.push_back(neighbor);
    }
    
    //Find Vertex-Covers
    bool found = false;
    int min = v + 1, s = min;
    vector<vector<int> > covers; // stores all found minimal vertex-covers.
    vector<int> coverAll(v, 1); // a vertex cover which contains all vertices
    
    for(int i=0; i<v; i++)
    {
        if(found) break;

        vector<int> cover = coverAll;
        cover[i] = 0;
        cover = getMinimal(neighbors, cover); //"cover" becomes a minimal vertex cover.
        s = coverSize(cover);
        if(s < min)
            min = s;
        if(s <= k)
        {
            cout << s << endl;
            for(int j=0; j<v; j++)
                if(cover[j] == 1)
                    cout << j+1 << " ";
            cout << endl;
            
            covers.push_back(cover);
            found = true;
            break;
        }
        
        for(int j=0; j<v-k; j++)
            cover = getCross(neighbors, cover, j);
        s = coverSize(cover);
        if(s < min)
            min = s;
        covers.push_back(cover);
        if(s <= k)
        {
            cout << s << endl;
            for(int j=0; j<v; j++)
                if(cover[j] == 1)
                    cout << j+1 << " ";
            cout << endl;
            
            found = true;
            break;
        }
    }
    
    if (s > k)
    {
        for(int p=0; p<covers.size(); p++)
        {
            if(found) break;
            for(int q=p+1; q<covers.size(); q++)
            {
                if(found) break;
                vector<int> cover = coverAll;
                for(int r=0; r<cover.size(); r++)
                    if(covers[p][r]==0 && covers[q][r]==0)
                        cover[r] = 0;
                cover = getMinimal(neighbors,cover);
                s = coverSize(cover);
                if(s < min) min=s;
                if(s <= k)
                {
                    cout << s << endl;
                    for(int j=0; j<v; j++)
                        if(cover[j] == 1)
                            cout << j+1 << " ";
                    cout << endl;
                    
                    found = true;
                    break;
                }
                
                
                for(int j=0; j<k; j++)
                    cover = getCross(neighbors,cover,j);
                s = coverSize(cover);
                if(s < min)
                    min=s;
                
                if(s <= k)
                {
                    cout << s << endl;
                    for(int j=0; j<v; j++)
                        if(cover[j] == 1)
                            cout << j+1 << " ";
                    cout << endl;
                    
                    found = true;
                    break;
                }
            }
        }
    }
    
    if (s > k) {
        cout << "-1" << endl;
    }
}


bool removable(vector<int> neighbor, vector<int> cover)
{
    bool check = true;
    for(int i=0; i<neighbor.size(); i++)
        if(cover[neighbor[i]]==0)
        {
            check=false;
            break;
        }
    return check;
}

int maxRem(vector<vector<int> > neighbors, vector<int> cover)
{
    int r = -1, max = -1;
    for(int i=0; i<cover.size(); i++)
    {
        if(cover[i]==1 && removable(neighbors[i],cover)==true)
        {
            vector<int> temp_cover = cover;
            temp_cover[i] = 0;
            int sum = 0;
            for(int j=0; j<temp_cover.size(); j++)
                if(temp_cover[j]==1 && removable(neighbors[j], temp_cover)==true)
                    sum++;
            if(sum > max)
            {
                if(r == -1)
                {
                    max = sum;
                    r = i;
                }
                else if(neighbors[r].size()>=neighbors[i].size())
                {
                    max = sum;
                    r = i;
                }
            }
        }
    }
    return r;
}

vector<int> getMinimal(vector<vector<int> > neighbors, vector<int> cover)
{
    vector<int> temp_cover = cover;
    int r = 0;
    while(r != -1)
    {
        r= maxRem(neighbors, temp_cover);
        if(r!=-1) temp_cover[r] = 0;
    }
    return temp_cover;
}

vector<int> getCross(vector<vector<int> > neighbors, vector<int> cover, int k)
{
    int count = 0;
    vector<int> temp_cover = cover;
    for(int i=0; i<temp_cover.size(); i++)
    {
        if(temp_cover[i] == 1)
        {
            int sum = 0, index = 0;
            for(int j=0; j<neighbors[i].size(); j++)
                if(temp_cover[neighbors[i][j]] == 0)
                {
                    index=j;
                    sum++;
                }
            if(sum==1 && cover[neighbors[i][index]]==0)
            {
                temp_cover[neighbors[i][index]] = 1;
                temp_cover[i] = 0;
                temp_cover = getMinimal(neighbors, temp_cover);
                count ++;
            }
            if(count > k)
                break;
        }
    }
    return temp_cover;
}

int coverSize(vector<int> cover)
{
    int count = 0;
    for(int i=0; i<cover.size(); i++)
        if(cover[i]==1)
            count++;
    return count;
}




