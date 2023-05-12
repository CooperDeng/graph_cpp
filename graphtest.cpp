#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int MAXV = 23133;

class illegal_exception {
    public:
    std::string error;
    illegal_exception();
};

illegal_exception::illegal_exception() {
    error = "illegal argument\n";
}

class Vertex {
    public:
    double weight;
    int next;
    int pointing_from;
    Vertex();
    Vertex(int input_next, double input_weight);
};  

Vertex::Vertex() {
    //indicating empty
    weight = 0.0;
    pointing_from = 0;
    next = 0;
}

Vertex::Vertex(int input_next, double input_weight){
    next = input_next;
    weight = input_weight;
    //note that we're setting pointing_from to zero, because we don't use 
    //this value when we're doing standard insertting
    pointing_from = 0;
}

class Graph {
    private:
    //I want the index to start from one, to prevent any confusion.
    std::vector <Vertex*> V[MAXV+1];

    //used to show if a vertex exist
    bool VS[MAXV+1];

    //only insertion and deletion changes the size
    int size;    

    public:

    Graph();
    ~Graph();

    //inserting a vertex (note that it must comes with an edge and a weight)
    // from a, to b, with weight of weight
    void insert (int a, int b, double w, bool if_load);
    
    //printing the vertex a and all stuff adjacent bruh
    void print(int a);
    
    //deleting a vertex that has existed
    void delete_vertex(int a);
    //returning the size of the graph
    void size_return();

    //used to print size for mst
    void mst_size();
    
    //find the mst rooted at vertex a
    //maximum spanning tree
    void mst(int a);
};

//class below is inspired by youtube tutorial carried out by HackerRank
//youtube link: https://www.youtube.com/watch?v=t0Cq6tVNRBA
class Maxheap {
    private:
    int size;
    std::vector <Vertex*> VH;
    int GetleftChildIndex(int parentIndex) { return 2*parentIndex +1;};
    int getRightChildIndex(int parentIndex) { return 2*parentIndex +1;};
    int getParentIndex(int childIndex) {return (childIndex -1)/2;};

    bool hasLeftChild(int index) {return GetleftChildIndex(index)<size;};
    bool hasRightChild(int index) {return getRightChildIndex(index)<size;};
    bool hasParent(int index) {return getParentIndex(index)>=0;};

    Vertex* leftChild(int index){return VH[GetleftChildIndex(index)];};
    Vertex* rightChild(int index){ return VH[GetleftChildIndex(index)];};
    Vertex* parent(int index){return VH[getParentIndex(index)];};

    void swap(int indexOne, int indexTwo) {
        Vertex* temp = VH[indexOne];
        VH[indexOne] = VH[indexTwo];
        VH[indexTwo] = temp;
    };

    public:
    
    
    void add(Vertex* new_vertex);
    void heapifyUp();
    void heapifydown();
    void remove_root();
    void print_heap();
    bool heap_empty();
    Vertex* return_root();
    Maxheap();
    ~Maxheap();
}; 

Maxheap::Maxheap(){
    size = 0;
}

Vertex* Maxheap::return_root() {
    return VH[0];
}

void Maxheap::add(Vertex* new_vertex){
    VH.push_back(new_vertex);
    size++;
    heapifyUp();
}

void Maxheap::heapifyUp(){
    int index = size-1;
    while(hasParent(index) && parent(index)->weight<VH[index]->weight){
        swap(getParentIndex(index), index);
        index = getParentIndex(index);
    }
}

void Maxheap::heapifydown(){
    int index = 0;
    while(hasLeftChild(index)){
        int smallerChildIndex = GetleftChildIndex(index);
        if(hasRightChild(index) && rightChild(index)->weight>leftChild(index)->weight){
            smallerChildIndex = getRightChildIndex(index);
        }
        if(VH[index]->weight>VH[smallerChildIndex]->weight){
            break;
        }
        else{
            swap(index, smallerChildIndex);
        }
        index = smallerChildIndex;
    }
}

void Maxheap::remove_root(){
    VH[0] = VH.back();
    VH.pop_back();
    size--;
    heapifydown();
}

void Maxheap::print_heap(){
    for (int i = 0; i < VH.size(); i++) {
        std::cout<< VH[i]->weight << " ";
    }

    std::cout<< "\n";
}

bool Maxheap::heap_empty(){
    return VH.empty();
}

Maxheap::~Maxheap(){
    for (int i = 0; i < VH.size(); i++){
        VH[i] = NULL;
    }
}

Graph::Graph(){
    size = 0;
    
    //this for loop doens't really do anything
    for (int i = 0; i < MAXV+1; i++) {
        for (int k = 0; k < V[i].size(); k++){
            V[i][k] = NULL;
        }
    }

    for (int j = 0; j <MAXV+1; j++) {
        VS[j] = false;
    }
}

Graph::~Graph(){
    size = 0;
    for (int i = 0; i < MAXV+1; i++) {
        for (int k = 0; k < V[i].size(); k++){
            //do stuff
            delete V[i][k];
            V[i][k] = NULL;

        }
    }
}

void Graph::insert(int a, int b, double w, bool if_load) {
    
    //catching illegal exceptions
    try {
        if (a < 1 || a > MAXV || b < 1 || b > MAXV || w < 0 || w > 1){
            throw illegal_exception();
        }

        else if (a == b) {
            throw illegal_exception();
        }
    }

    catch(illegal_exception exception) {
        std::cout << exception.error;
        return;
    }

    //if a does not exist
    if (VS[a]!=true) {
        Vertex *tempPtr = new Vertex(b, w);
        Vertex *new_vertex = tempPtr;
        new_vertex->pointing_from = a;
        V[a].push_back(new_vertex);
        tempPtr = NULL;
        VS[a] = true;
        size++;
/*
        //making b aware of a
        //this is just a virtual vertex of some sort
        Vertex *temp = new Vertex(a, w);
        V[b].push_back(temp);
*/      
        //if b does not exist
        if (VS[b] != true) {
            VS[b] = true;
            size++;
        }

        if (!if_load) {
            std::cout << "success\n";
        }
    }

    //a exist
    else {
        //we first traverse through the vector to find duplication
        bool duplication = false;

        for(int i = 0; i < V[a].size(); i++) {
            if (V[a][i]!=NULL) {
                if (V[a][i]->next == b) {
                    duplication = true;
                    break;
                }
            }
        }

        if (duplication) {
            std::cout << "failure\n";
        }

        //not duplicated, we do normal pushback
        else {
            Vertex *tempPtr = new Vertex(b, w);
            Vertex *new_vertex = tempPtr;
            new_vertex->pointing_from = a;
            V[a].push_back(new_vertex);
            tempPtr = NULL;
            if (VS[b] != true) {
                VS[b] = true;
                size++;
            }
/*
            //making b aware of a
            //this is just a virtual vertex of some sort
            Vertex *temp = new Vertex(a, w);
            V[b].push_back(temp);
*/
            if (!if_load) {
                std::cout << "success\n";
            }
        }

    }
    return;
}

// print as if "b c d ..."
void Graph::print(int a) {
    //catching illegal exceptions
    try {
        if (a < 1 || a > MAXV){
            throw illegal_exception();
        }
    }

    catch(illegal_exception exception) {
        std::cout << exception.error;
        return;
    }

    //if a does not exist
    if (V[a].empty()) {
        std::cout << "\n";
        return;
    }

    else {
        // we simply traverse through the vector to print
        for (int i = 0; i < V[a].size(); i++) {
            if (V[a][i] != NULL) {
                if (true) {
                    std::cout << V[a][i]->next << " ";
                }
            }
        }
        std::cout <<"\n";

    }

    return;

}

void Graph::delete_vertex(int a) {

    //catching illegal exceptions
    try {
        if (a < 1 || a > MAXV){
            throw illegal_exception();
        }
    }

    catch(illegal_exception exception) {
        std::cout << exception.error;
        return;
    }

    //first we start by checking if the vertex exists
    if (VS[a] == false){
        std::cout <<"failure\n";
        return;
    }

    //the vertex exists
    else {
        // STANDS FOR ASSOCIATED BRUH
        std::vector <int> ass;

        for (int i = 0; i < V[a].size(); i++) {
            //pushing back associated values
            //we have ensured that there's no duplication
            ass.push_back(V[a][i]->next);
        }

        //deleting the vertex itself
        //just to be more explicit, preventing mem leak
        for (int bruh = 0; bruh < V[a].size(); bruh++) {
            delete V[a][bruh];
            V[a][bruh] == NULL;
        }

        V[a].clear();
        VS[a] = false; 

        std::cout << "success\n";


        //basically looping through every single living being on earth
        for (int k = 0; k < MAXV; k++) {
            for (int j = 0; j < V[k].size(); j++ ){
                if (V[k][j]->next == a) {
                    delete V[k][j];
                    V[k][j] = NULL;
                    V[k].erase(V[k].begin() + j);
                    break;
                }
            }
        }
/*
        for (int k = 0; k < ass.size(); k++) {
            for (int j = 0; j < V[ass[k]].size(); j++) {
                if (V[ass[k]][j]->next == a) {
                    delete V[ass[k]][j];
                    V[ass[k]][j] = NULL;
                    V[ass[k]].erase(V[k].begin() + j);
                    std::cout << "deleted " << ass[k] << " " << j;
                    break;
                }
            }
        }*/
    }


    size--;
    return;
}

void Graph::size_return() {
    std::cout << "number of vertices is " << size << "\n";
    return;
}

void Graph::mst_size() {
    std::cout << size << "\n";
}

void Graph::mst(int a) {
    //catching illegal exceptions
    try {
        if (a < 1 || a > MAXV){
            throw illegal_exception();
        }
    }

    catch(illegal_exception exception) {
        std::cout << exception.error;
        return;
    }

    if (VS[a] != true) {
        std::cout << "failure\n";
    }

    //empty vertex
    else if (V[a].empty()){ 
        //bruh
        std::cout << "1\n";    
        return;
    }

    //non-empty existing vertex
    //using prim's algorithm
    else {
        //new Maxheap & graph for mst
        Maxheap MH;        
        Graph mstg;

        //visited stors all vertices that are visited
        std::vector <int> visited;
        
        //used for the application of prim's algorithm during while loop
        std::vector <Vertex*> mstp;

        //this vector stores stuff to be put in mst
        std::vector <Vertex*> mst_V;

        //marking vertex a as visited, preventing loop
        visited.push_back(a);
        
        
        //adding a's edges into both MH and mst_V
        for (int k = 0; k < V[a].size(); k++) {
            MH.add(V[a][k]);
            mstp.push_back(V[a][k]);
        }

        if (MH.heap_empty()) {
            std::cout << "failure\n";
            return;
        }

        //for debugging the heap
//        MH.print_heap();
        int traverse = a;
        int while_cout = 1;
        while (!(MH.heap_empty())) {
            Vertex* new_vertex = new Vertex();
            delete new_vertex;
            new_vertex = NULL;

            //whatever we do, we're going for the highest-cost value.
            //found the highest cost option
            Vertex* root = MH.return_root();
            MH.remove_root();

            int index_found = 0;
            //trying to find the corresponding vertex in mstp
            //also updating the index_found variable
            for (int i = 0; i < mstp.size(); i++) {
                if (mstp[i]->next == root->next && mstp[i]->weight == root->weight) {
                    new_vertex = root;
                    index_found = i;
                    break;
                }
            }


            //we check if the next specified is a visited value
            //if so, we just straight out end this cycle
            bool should_break = false;
            for (int i = 0; i < visited.size(); i++) {
                if (root->next == visited[i]) {
                    //dies
                    mstp.erase(mstp.begin() + index_found);
                    should_break = true;
                    break;
                }
            }

            if (should_break) {
                continue;
            }
            //the next specified is not a visited value
            //update traverse, mstp, mst_V
            traverse = new_vertex->next;
            mst_V.push_back(new_vertex);
            mstp.erase(mstp.begin() + index_found);
            //making traverse as visited
            visited.push_back(traverse);


            //this shouldn't be the case
            if (traverse == a) {
                //do nothing
            }

            else {
                //if traverse isn't pointing to a, which should be the case
                //we add new edges into MH, and we add them into mstp.
                for (int k = 0; k < V[traverse].size(); k++) {
                    MH.add(V[traverse][k]);
                    mstp.push_back(V[traverse][k]);
                }
            }



        }
        //this is the outside of while loop
        for (int i = 0; i < mst_V.size(); i ++) {
            mstg.insert(mst_V[i]->pointing_from, mst_V[i]->next, mst_V[i]->weight, true);
        }

        //lastly, we print out the size
        mstg.mst_size();
        
    }
    //outside of else
    //don't really need a return but i've put it here
    return;
}

int main(){
    
    std::string user_input = "";

    Graph g;

    while(true) {
        // looking for commands
        for (int i = 0; i <231333; i++){
            //do nothing
        }
        std::getline(std::cin, user_input);
        int found = user_input.find(" ");
        std::string input_c = user_input.substr(0, found);
        std::string rest = user_input.substr(found+1);

        

        if (input_c == "load") {
            
            std::ifstream input("paperCollabWeighted.txt");
            int num1, num2 = 0;
            double num3 = 0.0;
            while (input >> num1 >> num2 >> num3) {

                g.insert(num1, num2, num3, true);

            }

            std::cout <<"success load\n";
        }

        else if (input_c == "i") {    
            int x = 0;
            int y = 0;
            double z = 0.0;   
            std::string r_x = rest.substr(0, rest.find(" "));
            std::string r_y = rest.substr(rest.find(" ")+1);
            std::string r_z = r_y.substr(r_y.find(" ")+1);
            r_y = r_y.substr(0, r_y.find(" "));

            x = stoi(r_x); 
            y = stoi(r_y); 
            z = stod(r_z);

            g.insert(x,y,z,false);
        }

        else if (input_c == "p") {
            g.print(std::stoi(rest));
        }

        else if (input_c == "d") {
            g.delete_vertex(stoi(rest));
        }

        else if (input_c == "mst") {

            g.mst(stoi(rest));
        }

        else if (input_c == "exit") {
            break;
        }
        
        else if (rest == "size"){
            g.size_return();
        }

    }
    return 0;
}