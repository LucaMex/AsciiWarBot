#include <sstream>
#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <thread>
#include "gridWar.h"


const int width {120};
const int height {40};
char grid[height][width];
auto rng = std::default_random_engine {std::random_device{}()};
std::uniform_int_distribution<> distrW(0, width); // define the range
std::uniform_int_distribution<> distrH(0, height); // define the range
int expansions{};
long rounds{};
char leader{};
int best{};

/////////////////////////////////////////////////////////////////////////////////

class State {

public:
	State(char c) : id{c}{
		conqueredRegionsIds.push_back(c);
	}	
	char id;
	std::vector<char> closeRegionsIds; //just for startup
	std::vector<char> conqueredRegionsIds;
};

/////////////////////////////////////////////////////////////////////////////////

class Region {

public:
	Region(char c) : id(c), stateId{c}{}

	char id;
	char stateId;		
	int indexes[height][width] = {}; //1 if the grid cell belongs to the region
	std::vector<char> closeRegionsIds;

	void addIndex(int i, int j){
		indexes[i][j] = 1;
	}
};

/////////////////////////////////////////////////////////////////////////////////

std::vector<Region> regionsVec;
std::vector<State> statesVec;

bool setChar(char c, int x, int y){
	
	if(grid[x][y]=='*'){
		grid[x][y] = c;
		State l{c};
		statesVec.push_back(l);
		Region r{c};
		regionsVec.push_back(r);
		return true;
	}
	else return false;

}

void remove(std::vector<char> &v)
{
	std::vector<char>::iterator itr = v.begin();
	std::unordered_set<char> s;

	for (auto curr = v.begin(); curr != v.end(); ++curr) {
		if (s.insert(*curr).second)
			*itr++ = *curr;
	}

	v.erase(itr, v.end());
}


void printStatesVector(){

	for(auto elem : statesVec){
		std::cout << "Id: "<< elem.id << std::endl<<"Conquered regions: ";
		for(auto regId : elem.conqueredRegionsIds){
			std::cout << regId << ",";
		}
		std::cout << std::endl << "Confining regions: ";
		for(auto regId : elem.closeRegionsIds){
			std::cout << regId << ",";
		} 
		std::cout << std::endl << std::endl;

	}

}


void printRegionsVector(){

	for(auto elem : regionsVec){
		std::cout << "Region Id: "<< toupper(elem.id) << " State Id: " << elem.stateId;
		
		std::cout << std::endl << "Confining regions: ";
		for(auto regId : elem.closeRegionsIds){
			std::cout << regId << ",";
		} 
		std::cout << std::endl << std::endl;

	}

}

void updateGrid(State attacker, Region defender){

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(defender.indexes[i][j] == 1){
				grid[i][j] = attacker.id;
			}
		}
	}

}


void printGrid(){
	std::cout<<std::endl;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){

		std::cout << grid[i][j];
		
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}

void printColoredGrid(char att, Region r){
	std::cout<<std::endl;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){

			if(r.indexes[i][j] == 1){
				std::cout << KYEL <<grid[i][j] << RST;
			}
			else if(grid[i][j] == r.stateId){
				std::cout << KRED << grid[i][j]<<RST;
			}
			else if(grid[i][j] == att){
				std::cout << KGRN<<grid[i][j]<<RST;
		
			}
			else if(grid[i][j]==FAVORITE){
				std::cout << KCYN<<grid[i][j]<<RST;
			}
			else if(islower(grid[i][j])){
				std::cout << KBLU<<grid[i][j]<<RST;
			}
			
			else {
				std::cout << grid[i][j];
			}
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
	
}
void printRegion(Region r){ //only after region extension
	std::cout<<std::endl;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(r.indexes[i][j] == 1){
				std::cout << grid[i][j];
			}
			else{
				std::cout << ".";
			}
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}

void Init(){

	//fill grid
	for(int j=0;j<height;j++){
		for(int i=0;i<width;i++){
			grid[j][i]='*';
		}	
	}

	for(int i=97;i<123;i++){
		if(!setChar(static_cast<char>(i),distrH(rng),distrW(rng))){ //ripeto se becco un punto già occupato
			i--;
		};
	}

	printStatesVector();
	printGrid();
	printRegion(regionsVec.at(0));
}

struct state_finder
{
    state_finder(char key) : key_(key)
    {}

    bool operator()(const State& o) const
    {
        return key_ == o.id;
    }

    const int key_;
};


void setRegionsNeightbours(){

	for(auto& elem : regionsVec){

		std::vector<State>::iterator it = find_if (statesVec.begin(), statesVec.end(), state_finder(elem.id)) ;

		if(it == statesVec.end()){
			std::cout << "Error! State "<< elem.id << "not found!" << std::endl;
    	}
		else{
    		for(auto i : it->closeRegionsIds){
    			elem.closeRegionsIds.push_back(i);
    		}
		}
	}
}

void setRegionsIndexes(){
	
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(!isupper(grid[i][j])){
				regionsVec.at(static_cast<int>(grid[i][j]-'a')).addIndex(i,j);
			}

		}
	}


}


void expand(State& c, bool finalize = false){
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(grid[i][j]==c.id){
				if (i-1 >= 0 ){
					if(grid[i-1][j]=='*'){
						grid[i-1][j]='~';
					}
					else if(finalize && grid[i-1][j]!=c.id && grid[i-1][j]!=toupper(c.id)){
						grid[i][j] = toupper(c.id);
						c.closeRegionsIds.push_back(tolower(grid[i-1][j]));
						//grid[i-1][j]='.';
					}
				}
				if (i+1 < height){ 
					if(grid[i+1][j]=='*'){
						grid[i+1][j]='~';
					}
					else if(finalize && grid[i+1][j]!=c.id && grid[i+1][j]!=toupper(c.id)){
						grid[i][j] = toupper(c.id);
						c.closeRegionsIds.push_back(tolower(grid[i+1][j]));

						//grid[i+1][j]='.';
					}
				}
				if (j-1 >= 0){
					if(grid[i][j-1]=='*'){
						grid[i][j-1]='~';
					}
					else if(finalize && grid[i][j-1]!=c.id && grid[i][j-1]!=toupper(c.id)){
						grid[i][j] = toupper(c.id);
						c.closeRegionsIds.push_back(tolower(grid[i][j-1]));

						//grid[i][j-1]='.';
					}
				}

				if (j+1 < width){
					if(grid[i][j+1]=='*'){
						grid[i][j+1]='~';
					}
					else if(finalize && grid[i][j+1]!=c.id && grid[i][j+1]!=toupper(c.id)){
						grid[i][j] = toupper(c.id);
						c.closeRegionsIds.push_back(tolower(grid[i][j+1]));

						//grid[i][j+1]='.';
					}
				}

			}
		}

	}	
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(grid[i][j]=='~'){
				grid[i][j]=c.id;
				expansions++;
			}
		}
	}
}

bool updateNeighbours(State * s){

	if(s->conqueredRegionsIds.size() == 0){
		//STATO ELIMINATO! TODO
		//std::cout << "STATO " << s->id << " ELIMINATO!" << std::endl;
		return false;
	}
	else{
		s->closeRegionsIds.clear();
		for(auto elem : s->conqueredRegionsIds){ //per ogni regione conquistata
			for(auto ee : regionsVec.at(elem-'a').closeRegionsIds){ //per ogni vicino di ogni regione conquistata
				if(find_if (s->conqueredRegionsIds.begin(), s->conqueredRegionsIds.end(), state_finder(ee))==s->conqueredRegionsIds.end()){ //se non trovo tra le regioni conquistate quel vicino
					//if(!find_if (statesVec.begin(), statesVec.end(), state_finder(ee->stateId))==statesVec.end())
					s->closeRegionsIds.push_back(ee);
				}
			}

		}
		remove(s->closeRegionsIds);
		//std::cout << "STATO " << s->id << " DIM" << s->conqueredRegionsIds.size() << std::endl;

		return true;
	}
	
}

void getNewLeader(){
	best = 0;
	for(auto c : statesVec){
		if(c.conqueredRegionsIds.size()>best){
			best = c.conqueredRegionsIds.size();
			leader = c.id;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void beginWar(){
	
	long prevTime = 0;
	char prevStateId{};
	bool erased = false;
	while(statesVec.size()>1){
		std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
		std::chrono::system_clock::duration dtn = tp.time_since_epoch();
		long sysTime = std::chrono::duration_cast<std::chrono::milliseconds>(dtn).count();
		if(sysTime-prevTime > TIMESTEP){
			rounds++;
			prevTime = sysTime;
			std::shuffle(std::begin(statesVec), std::end(statesVec), rng); //ATTACKER - shuffle the states vector to choose
			std::shuffle(std::begin(statesVec.at(0).closeRegionsIds), std::end(statesVec.at(0).closeRegionsIds), rng); //DEFENDER - shuffle the neighbours vector to choose
			State* attacker = &statesVec.at(0);
			Region* region = &regionsVec.at(statesVec.at(0).closeRegionsIds.at(0)-'a');
			std::cout << "Attacker: "<< attacker->id << ", defender: "<<region->stateId<< ", attacked region: "<< static_cast<char>(toupper(region->id)) << std::endl;
			//printRegion(*region);
			updateGrid(*attacker, *region); 
			
			//printStatesVector();

			//printGrid();

			///Find defender by region stateId
			std::vector<State>::iterator it = find_if (statesVec.begin(), statesVec.end(), state_finder(region->stateId)) ;
			if(it == statesVec.end()){
				std::cout << "Error! State "<< region->stateId << " not found!" << std::endl;
	    	}
	    	else{
	    	
				attacker->conqueredRegionsIds.push_back(region->id);
				printColoredGrid(attacker->id, *region);			
				prevStateId = region->stateId;
				region->stateId = attacker->id;
				it->conqueredRegionsIds.erase(std::remove(it->conqueredRegionsIds.begin(), it->conqueredRegionsIds.end(), region->id), it->conqueredRegionsIds.end());
				//printStatesVector();
				updateNeighbours(attacker);

				getNewLeader();

				if(!updateNeighbours(&(*it))){
					statesVec.erase(it);
					erased = true;
				}
				//printStatesVector();
				std::cout << "Day "<<rounds<<std::endl;
				std::cout << "[" <<KBOLD<< KGRN<<attacker->id << RST<<RST << "]"<<nameList[attacker->id-'a']<<" conquista la regione [" <<KBOLD<< KYEL << static_cast<char>(toupper(region->id)) <<RST<<RST << "] da [" <<KBOLD << KRED << prevStateId <<RST<< RST <<"]"<<nameList[prevStateId-'a']<<"."<<std::endl;
				std::cout<<"["<<KBOLD<<KGRN<<attacker->id<<RST<<RST <<"]" <<nameList[attacker->id-'a']<<" ora possiede " <<KBOLD<< attacker->conqueredRegionsIds.size()<< RST << " regioni."<<std::endl;
				if(erased) std::cout <<KBOLD<< "["<<KRED<<prevStateId<<RST<<"]"<<nameList[prevStateId-'a']<<" è stato eliminato!"<<RST<<std::endl;
				else 	std::cout<<"["<<KRED<<prevStateId<<RST <<"]"<<nameList[prevStateId-'a']<< " ora possiede " <<KBOLD<< it->conqueredRegionsIds.size()<<RST << " regioni."<<std::endl;
				std::cout << "Il leader è ["<< KBOLD << KMAG << leader << RST << RST <<"]"<<nameList[leader-'a']<< " con "<< KBOLD<<best<<RST<<" regioni."<<std::endl;
 				erased = false;
			}		
			//prevTime = time;
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
	
	Init();

	for(int i=0;i<width;i++){		
		std::shuffle(std::begin(statesVec), std::end(statesVec), rng);
		expansions = 0;
		for(auto& c:statesVec){
			expand(c);
		}
		if(expansions == 0) break;
		printGrid();
		std::this_thread::sleep_for(std::chrono::milliseconds(TIMESTEP/width));
	}

	printGrid();

	for(auto& c : statesVec){
		expand(c,true);
		remove(c.closeRegionsIds);

	}
	printStatesVector();
	printGrid();
	setRegionsNeightbours();
	setRegionsIndexes();
	//printRegionsVector();
	//printRegion(regionsVec.at('d'-'a'));
	beginWar();
	std::cout << "WAR ENDED! WINNER: "<< statesVec.at(0).id << std::endl;
	printGrid();
}


/*vector<myObj>::iterator it = std::find_if(...);

if(it == myObjList.end())
    // handle error in any way
else
    return *it;*/