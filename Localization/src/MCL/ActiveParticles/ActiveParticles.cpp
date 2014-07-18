/**
* 
* @File ActiveParticles.cpp
* @Author Alex Rich and John Allard. Summer 2014.
* @Info Operations on active particles and particle generations.
*
**/

#include "ActiveParticles.h"

#define PI 3.1415926

namespace MCL
{

    ActiveParticles::ActiveParticles() : generation(0), defaultDistributionSize(3000), gd(0.2), dtheta(15) {srand(time(0));}

    Perspective Scatter(Perspective p, float maxtranslation, int prob=32);
    
    Perspective ActiveParticles::MakeGuess()
    {
        // First Determine Location
        float avgx = 0.0;
        float avgy = 0.0;
        float avgz = 0.0;

        int totalPs = this->NumParticles();

        float maxx = 0.0;
        float maxy = 0.0;
        float maxz = 0.0;
        float minx = 1000.0;
        float miny = 1000.0;
        float minz = 1000.0;

        for (int i = 0; i < totalPs; i++)
        {
            Particle p(this->pList[i]);
            float x = p.GetPerspective(0);
            float y = p.GetPerspective(1);
            float z = p.GetPerspective(2);
            float w = p.GetWeight();
            avgx += w * x;
            avgy += w * y;
            avgz += w * z;

            maxx = x > maxx ? x : maxx;
            minx = x < minx ? x : minx;
            maxy = y > maxy ? y : maxy;
            miny = y < miny ? y : miny;
            maxz = z > maxz ? z : maxz;
            minz = z < minz ? z : minz;
        }
        string t = "\t";

        // cout << totalPs << t << this->GetAvgWeight() << t << avgx << "\t" << avgy << "\t" << avgz << endl;

        avgx = avgx / (this->GetAvgWeight() * (float) totalPs);
        avgy = avgy / (this->GetAvgWeight() * (float) totalPs);
        avgz = avgz / (this->GetAvgWeight() * (float) totalPs);

        float dx = (maxx - avgx) > (avgx - minx) ? (maxx - avgx) : (avgx - minx);
        float dy = (maxy - avgy) > (avgy - miny) ? (maxy - avgy) : (avgy - miny);
        float dz = (maxz - avgz) > (avgz - minz) ? (maxz - avgz) : (avgz - minz);

        float dist = sqrt(dx * dx + dy * dy + dz * dz) / 3;

        Particle myP(Perspective(avgx, avgy, avgz, 0.0, 0.0, 0.0));

        double avgdx = 0;
        double avgdy = 0;
        double avgdz = 0;

        int inRange = 0;

        for (int i = 0; i < totalPs; i++)
        {
            if (myP.Distance(pList[i]) < dist)
            {
                float w = myP.GetWeight();
                avgdx = w * pList[i].GetPerspective(3);
                avgdy = w * pList[i].GetPerspective(4);
                avgdz = w * pList[i].GetPerspective(5);
                inRange++;
            }
        }

        this->guessQualityHistory.push_back((float) inRange / (float) totalPs);

        avgdx = avgdx / (this->GetAvgWeight() * totalPs);
        avgdy = avgdy / (this->GetAvgWeight() * totalPs);
        avgdz = avgdz / (this->GetAvgWeight() * totalPs);
        

        Perspective guess(avgx, avgy, avgz, avgdx, avgdy, avgdz);
        SnapToGrid(&guess);
        this->guessHistory.push_back(guess);

        cout << "ParticleListSize:" << pList.size() << endl;

        return guess;
    }

    float ActiveParticles::ComputeAvgWeight()
    {
        double avg = 0;

        float totalPs = (float) this->pList.size();

        for (int i = 0; i < totalPs; i++)
        {
            avg += this->pList[i].GetWeight() / totalPs;
        }
        this->weightHistory.push_back(avg);
        return avg;
    }

    int ActiveParticles::GenerateDistribution()
    {
        return GenerateDistribution(6000);
    }

    int ActiveParticles::GenerateDistribution(int wantedSize)
    {
        // Randomly generate a distribution based on 
        // the weights of all elements in pList
        float totalWeight = this->GetAvgWeight() * (float) this->NumParticles();
        this->distribution.clear();

        for (int i = 0; i < this->pList.size(); i++)
        {
            // cout << this->pList[i].GetWeight() << "\t" << wantedSize << "\t" << totalWeight << endl;
            int num = (this->pList[i].GetWeight() * wantedSize) / totalWeight;
            for (; num > -1; num--)
                this->distribution.push_back(this->pList[i].GetPerspective());
        }
        return 0;
    }

    void ActiveParticles::GenerateParticles()
    {
        GenerateParticles(pList.size());
    }

    void ActiveParticles::GenerateParticles(int amount)
    {
        this->pList.clear();
        // uniform_int_distribution<int> dist(0, this->distribution.size() - 1);
        stringstream ss;
        ss << "Distribution Size : " << this->distribution.size();
        DebugIO(ss.str());

        for (int i = 0; i < 0.9*amount; i++)
        {
            int rndIdx = rand() % this->distribution.size();
            Perspective P = Scatter(this->distribution[rndIdx], this->gd*2, 32);
            // cout << this->distribution[rndIdx].ToString() << "->" << P.ToString();
            SnapToGrid(&P);
            // cout << "->" << P.ToString() << endl;
            if (!masterMap.count(P))
            {
                i--;
                continue;
            }
            pList.push_back(Particle(P));
        }

        for (int i = 0; i < 0.1*amount; i++)
        {
            int rndIdx = rand() % perspectives.size();
            Perspective P = perspectives[rndIdx];
            if (!masterMap.count(P))
            {
                i--;
                continue;
            }
            pList.push_back(Particle(P));
        }
    }

    Perspective ActiveParticles::AnalyzeList()
    {
        this->ComputeAvgWeight();
        this->generation++;
        return this->MakeGuess();
    }

    // Rounds to second decimal place
    float round(float x)
    {
        return (float) ((int) (x*100))/100.0;
    }

    float ActiveParticles::GetAngle(float x, float y)
    {
        float angle = atan2(y, x) * 180.0 / PI;
        float mul = angle / dtheta;
        int intmul = (int) (mul + 0.5);
        return round(dtheta * intmul);
    }

    void ActiveParticles::SnapToGrid(Perspective * p)
    {
        Perspective refP = perspectives[0];

        float normalx = p->x - refP.x;
        float normaly = p->y - refP.y;

        float mulx = normalx / this->gd;
        float muly = normaly / this->gd;
        int nearestMulx = floor(mulx + 0.5);
        int nearestMuly = floor(muly + 0.5);

        p->x = nearestMulx * this->gd + refP.x;
        p->y = nearestMuly * this->gd + refP.y;

        float angle = GetAngle(p->dx, p->dy);

        p->dx = round(cos(angle * PI / 180.0));
        p->dy = round(sin(angle * PI / 180.0));
    }

    Perspective ActiveParticles::Scatter(Perspective p, float maxtranslation, int prob)
    {
        if (prob < 2)
            return p;

        // uniform_int_distribution<int> dist(1, 100);

        int rnd = rand() % 100; // dist(time(0));
        vector<float> v = p.ToVector();

        // cout << "Scattering with prob: " << rnd << "/" << prob << endl;
        if (rnd < prob)
        {
            if (rnd % 2 == 0)
            { // turn!
                cout << "U";
                float curangle = p.GetAngle();
                curangle += (float) (rand() % 100 - 50) / 25; //(dist(default_random_engine) - 50) / 80.0;
                v[3] = round(cos(curangle * PI / 180.0));
                v[4] = round(sin(curangle * PI / 180.0));
            }
            else
            { // translate!
                cout << "R";
                v[0] += (float) (rand() % 100 - 50) * maxtranslation / 50.0;
                v[1] += (float) (rand() % 100 - 50) * maxtranslation / 50.0;
            }
        }
        return Scatter(Perspective(v), maxtranslation, prob/2);
    }

    int ActiveParticles::Move(float travel, float turn)
    {
        vector<Particle> newPList;
        for (int i = 0; i < pList.size(); i++)
        {
            Particle myP = pList[i];
            vector<float> myV = myP.GetPerspective().ToVector();
            
            // Turn
            float origAngle = GetAngle(myV[3], myV[4]);
            float newangle = origAngle + turn;

            myV[3] = round(cos(newangle * PI / 180.0));
            myV[4] = round(sin(newangle * PI / 180.0));

            // Translate along axis.
            float mag = (float)sqrt(myV[3] * myV[3] + myV[4] * myV[4]);
            myV[3] /= mag;
            myV[4] /= mag;

            myV[0] += myV[3] * travel;
            myV[1] += myV[4] * travel;

            Perspective P = Perspective(myV);

            SnapToGrid(&P);

            if (find(perspectives.begin(), perspectives.end(), P) != perspectives.end())
                newPList.push_back(Particle(Perspective(myV)));
        }
        return 0;
    }

    bool ActiveParticles::GetConstants(string dirName)
    {
        string fn = "../../Data/InputData/" + dirName + "/InputInfo.txt";
        // Get Input Data
        ifstream file(fn.c_str());
        if ( !file.is_open() )
        {
            stringstream ss;
            ss << "InputInfo.txt Not Found for model " << dirName << "! Looked in " << fn;;
            ErrorIO(ss.str());
            return false;
        }

        string str;

        // grid density
        getline( file, str );
        this->gd = atof(str.c_str());

        // Angle difference
        getline( file, str );
        this->dtheta = atof(str.c_str());
        return true;
    }

    vector<Particle> ActiveParticles::GetParticleList()
    { return this->pList; }

    void ActiveParticles::GetParticleList(vector<Particle> * v)
    { v = &(this->pList); }

    void ActiveParticles::SetParticleList(vector<Particle> pl)
    { this->pList = pl; }

    vector<float> ActiveParticles::GetWeightHistory()
    { return this->weightHistory; }

    vector<Perspective> ActiveParticles::GetGuessHistory()
    { return this->guessHistory; }

    vector<Perspective> ActiveParticles::GetDistribution()
    { return this->distribution; }
    
    void ActiveParticles::SetDistribution(vector<Perspective> dist)
    { this->distribution = dist; }

    Perspective ActiveParticles::GetGuess()
    { return this->guessHistory.back(); }
    
    float ActiveParticles::GetAvgWeight()
    { 
        // cout << this->weightHistory.back() << endl; 
        return this->weightHistory.back(); }
    
    int ActiveParticles::GetGeneration()
    { return this->generation; }

    int ActiveParticles::NumParticles()
    { return this->pList.size(); }

    void ActiveParticles::generatePoints()
    {
        ofstream pListFile;
        pListFile.open("ParticleLists.txt");

        vector<float> g = GetGuess().ToVector();
        for (int i = 0; i < g.length(); i++)
            pListFile << g[i] << " ";

        for (int i = 0; i < NumParticles(); i++)
        {
            Particle p = pList[i];
            vector<float> pv = p.ToVector();
            for (int j = 0; j < pv.length(); j++)
                pListFile << "\n" << pv[j] << " ";
            pListFile << p.GetWeight();
        }

        pListFile.close();

        return true;
    }
}