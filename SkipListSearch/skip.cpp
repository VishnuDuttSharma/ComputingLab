/**
 *	@file skip.cpp
 *	@author Vishnu Dutt Sharma 12EC35018
 *
 *	@param None
 *	@return None, Prints number of seraches and makes graph
 *
 *	@brief Implements Skip List and performs some search operations and makes graphs
 *
 *		This module reads three files. It makes lists from those files and the performs
 *      some search opearations on them and makes graphs for them
 */
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sys/time.h>
#include <limits.h>

using namespace std;
/*
 * \enum KEYS for helping in getting attributes from the roll number
 */
enum KEYS {ROLL, DEPT, YEAR, DEG};



/**
 * 	\def SKIP_MAX_HEIGHT Maximum height in a Skip List
 */
#define SKIP_MAX_HEIGHT 4

/**
 * @brief traceFile File for helping in tracing
 * @brief totalCount Global variable for counting number of elements
 * @brief currLevel Current layer level in the list
 */
ofstream traceFile;
int totalCount, currLevel;
/**
 * \struct node
 *
 *	@brief Structure for nodes of the skip list
 *		key: key for the list and node
 *		height: height/size of the node array
 *		value: string that represents the value stored at the node
 *		direct: array of node that is used to store address of next same level nodes
 */
typedef struct node{
    int key;
    int height;
    char value[256];
    struct node **direct;
}node;

/**
 * \struct SkipList
 *
 *	@brief Structure for SkipList
 *		key: key for the list and the nodes
 *		height: height/size of the node array
 *		next: adsress of the firsr node that holds the contents of the list
 */
typedef struct SkipList{
    int key;
    int height;
    struct node *next;
}SkipList;


/**
 * Arrays for helping in search operations, used for storing start and stop node after a search operation
 */
node *listing1[2];
node *listing2[2];

/**
 *	\fn int convert(int roll, KEYS basis)
 *
 *	@param roll Roll number
 *	@param basis Attrubute to be returned, determined by enum KEYS
 *
 *	@return Attrbute extracted from the roll number
 *
 *	@brief Function to return an attrbute from the roll number
 */
int convert(int roll, KEYS basis)
{
    switch( basis ){
        case ROLL:
        roll = roll % 100;
        break;
        case DEPT:
        roll = (roll % 10000) / 100;
        break;
        case YEAR:
        roll = (roll % 100000) / 10000;
        break;
        case DEG:
        roll = roll / 100000;
        break;
    }

    return roll;
}

/**
 *	\fn void print_list(SkipList *myList)
 *
 *	@param myList Address of the skip list to be printed
 *
 *	@return Void
 *
 *	@brief Function to print the skip list
 */
void print_list(SkipList *myList)
{
    // Temporary node for iteration
    node *point;

    point = myList->next;

    // Traverse through the list and print contents at each point
    while(point->direct[0]->key != INT_MAX){
        point = point->direct[0];
        cout<<"Key: "<<point->key<<", Value: "<<point->value<<", Height: "<<point->height<<endl;

    }

}

/**
 *	\fn SkipList *init_list()
 *
 *	@param None
 *
 *	@return list Pointer to the newly created Skip List
 *
 *	@brief Function to initialize a skip list
 *
 *		Function initializes a skip list. Last element has the value equal to \def INT_MAX
 */
SkipList *init_list()
{
    int i;

    // Initailize and Assign Space
    SkipList *list = (SkipList *)malloc( sizeof(SkipList) );
    node *next = (node *)malloc( sizeof(node ));

    // Attach node
    list->next = next;

    // Assign initial values
    list->key = INT_MAX;
    next->key = INT_MAX;
    sprintf(next->value, "%s", "Start");

    // Assign space to the array
    next->direct = (node **)malloc( sizeof(node *) * SKIP_MAX_HEIGHT);

    // Put elements in the array
    for(i = 0; i < SKIP_MAX_HEIGHT; i++){
        next->direct[i] = list->next;
    }

    // Assign height to the list
    next->height = SKIP_MAX_HEIGHT;

    return list;
}



/**
 *  void inserth(SkipList *myList, int key, char * val, int h)
 *
 *  @param myList Skip List where insertion is to be performed
 *  @param key Key of the element to be inserted
 *  @param val Value of the lement to be inserted
 *  @param h Height of the element to be inserted
 *
 *	@return Void
 *
 *	@brief Function to insert an element/node in the list with given height
 *	Insert   a   node   into   the   skip   list   with   the   key , value and height
 */
void inserth(SkipList *myList, int key, char * val, int h)
{
    node *point, *var, *pArray[SKIP_MAX_HEIGHT];// Iterator Nodes and nod earrays

    // Temporary node
    var = (node *)malloc( sizeof(node));

 	// Starting the list
    point = myList->next;

    // Limit the height
    if(h > SKIP_MAX_HEIGHT)
        h = SKIP_MAX_HEIGHT;


    for(int i = SKIP_MAX_HEIGHT-1; i >= 0; i--){
        while( point->direct[i]->key < key){
        	// Kep going while key of iterator value is lesser
            point = point->direct[i];
        }
        pArray[i] = point;
    }
    point = point->direct[0];

    // Assign values
    var->key = key;
    sprintf(var->value, "%s", val);
    var->height = h;
    var->direct = (node **)malloc( sizeof(node) * h);

/*
    if(point->key == key){
        for(int i = 0; i < h; i++){
            var->direct[i] = pArray[i]->direct[i];
        }
            pArray[i] = var;
        return ;
    }
*/


    // Copy value of temporary variable and add to the list
    for(int i = 0; i < h; i++){
        var->direct[i] = pArray[i]->direct[i];


        if(point->key == key){
            pArray[i] = var;
        }
        else{
            pArray[i]->direct[i] = var;

        }

    }

    //print_list(myList);
    //cout<<"Inserting: Key: "<<var->key<<" Value: "<<var->value<<" Height: "<<var->height<<"\n \t Current: "<<var<<" Next:"<<var->direct[1]<<endl;
}

/**
 *  void delete(SkipList *myList, int key)
 *
 *  @param myList Skip List fom which element is to be deleted
 *  @param key Key of the element to be deleted
 *
 *	return Void
 *
 *	@brief	Delete the node from the SkipList  with the given key
 */
void deletep(SkipList *myList, int key)
{
    int i; // Iterator
    node *point; // Iterator node

    // Start the list
    point = myList->next;

    for(i = SKIP_MAX_HEIGHT-1; i >= 0; i--){

        while( (point->direct[i]->key != key)){
            point = point->direct[i];

            // Break if end of list is reached
            if (point->key == INT_MAX)
                break;

            // If key matches, detach its pointer and attach it to nect element
            if(point->direct[i]->key == key){
                point->direct[i] = point->direct[i]->direct[i];
            }
        }
    }
}

/**
*  node *find(node *start, node *stop,KEYS basis, int key, int *count)
*
*   @param start Starting node for search operation
*   @param key Key of the elemnt to be searched
*
*   @return node pointer for the element after the last element to fulfill the criteria otherwise the element just after it
*
*   @brief Function to search for a key (on the basis of attribute) in some part of list
*       First it moves up till it gets a proper height for traversal, then it traverses horizontal till getting a sammler key
*       When a key if coming before next node, it moves down
*/
node *find(node *start, int key)
{
    int i = 0; // Height variable

    node *pointer = start; // Starting the list

    // Assigning last obtained height
    if(start->height >= currLevel)
        i = currLevel ;


    // Move up till you get max height for traversal to the key
    while((i < pointer->height-1) && (pointer->key < key)) {
        traceFile<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->key<<":l"<<i+1<<" [color=\"0.002 0.999 0.999\"]; \n";
        //cout<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->key<<":l"<<i+1<<" [color=\"0.002 0.999 0.999\"]; \n";
        i++;
        totalCount++;
    }


    while( pointer->key < key){
        // Loop till iterator key becomes greater than search key
        while( (pointer->direct[i]->key <= key)  ){

            // Plot and move forward
            traceFile<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->direct[i]->key<<":l"<<i<<" [color=\"0.002 0.999 0.999\"]; \n";

            pointer = pointer->direct[i];
            // Increase the number of steps
            totalCount++;

            // Loop till max height is reached
            while((i < pointer->height-1) && (pointer->key < key)) {
                traceFile<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->key<<":l"<<i+1<<" [color=\"0.002 0.999 0.999\"]; \n";
                //cout<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->key<<":l"<<i+1<<" [color=\"0.002 0.999 0.999\"]; \n";
                totalCount++;
                i++;
            }


        }

        // Return if equal key or greater key value iso btained
        if( pointer->key >= key ){
            totalCount++;
            return pointer;
        }

        // Layer 0 iterations
        else if(i == 0)
        {
            traceFile<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->direct[i]->key<<":l"<<i<<" [color=\"0.002 0.999 0.999\"]; \n";

            pointer = pointer->direct[i];

            currLevel = i;

            totalCount++;

            return pointer;
        }

        // Move down
        if(i > 0){
            traceFile<<"\tnode"<<pointer->key<<":l"<<i<<" -> node"<<pointer->key<<":l"<<i-1<<" [color=\"0.002 0.999 0.999\"]; \n";

            totalCount++;

            i--;
        }
    }
    currLevel = i;

    return pointer;


}


/**
*  \fn int findBy(node *start, node **nodeArr, KEYS basis, int key)
*  @param start Starting node for searching
*  @nodeArr Node array to store the first and node satisfying the criteria
*  @param basis Attribute for searching (Degree, Department etc)
*  @param key Key of the elemnt to be searched
*
*  @return Number of elemets satisfying the criteria
*
*   @brief Function to find in the SkipList the elements having the key provided for a given attributs
*/
int findBy(node *start, node **nodeArr, KEYS basis, int key)
{
    int i, step=0;
    int baseKey = 0;


    node *point, *subPoint;//, *pArray[SKIP_MAX_HEIGHT];

     // Starting point
    point = start;

    for(i = basis; i >= 0; i--){

        // Horizontal traversing while the iterator key is smaller than the key
        while( ( convert( point->direct[i]->key, basis ) < key)){
            point = point->direct[i];
            //cout<<point->key<<"-> "<<point->direct[i]->key<<","<<i<<endl;

            // Break if end of list is reached
            if (point->key == INT_MAX){
                break;
            }


        }
        step++;

        // If key is equal/matched
        if( convert( point->direct[i]->key, basis ) == key){
            //cout<<"Found: Key: "<<point->direct[i]->key<<" Value: "<<point->direct[i]->value<<" Steps: "<<step<<endl;

            nodeArr[0] = point->direct[basis];

            // Find the last element satisfying the criteria
            subPoint = point->direct[basis];
            while(( convert( subPoint->key, basis ) == key) ){


                //cout<<"Going in "<< subPoint->key <<endl;

                if(  subPoint->direct[0]->key  ==  INT_MAX)
                break;
                subPoint = subPoint->direct[0];

                step++;
            }

            //cout<<"Found: Key: "<<subPoint->direct[0]->key<<" Value: "<<subPoint->direct[0]->value<<" Steps: "<<step<<endl;
            nodeArr[1] = subPoint;

            return step-1;

        }
    }
    //cout<<"Not Found: Key: "<<point->direct[0]->key<<" Value: "<<point->direct[0]->value<<" Steps: "<<step<<endl;
    // If not found, return same key
    nodeArr[1] = point->direct[0];
    nodeArr[0] = point->direct[0];
    //return point->direct[0];
    return step;
}

/**
 *  int traverse_level(node *start, node *stop, int layer, string fileName)
 *
 *  @param start starting node for traversal
 *  @param stop end node for traversal
 *  @param layer Layer number for horizontal traversal
 *  @param fileName Name of the file where output is to be written
 *
 *  @return count of the elements traversed
 *
 *  @brief Function to traverse the list and write all elements in a file
 */
int traverse_level(node *start, node *stop, int layer, ofstream& outfile)
{
    int count = 0;
    //ofstream outfile;
    //outfile.open( fileName.c_str() );

    for ( node *pointer = start; pointer != stop; pointer = pointer->direct[layer]){
        outfile<<pointer->key<<" "<<pointer->value<<" "<<pointer->height<<" \n";
        //cout<<pointer->key<<" "<<pointer->value<<" "<<pointer->height<<" \n";

        count++;
    }

    //outfile.close();

    return count;
}

/**
 *  void display(SkipList *myList, string fileName)
 *
 *	@param myList List to be displayed
 *  @param filename Name of the file where the output is to be written
 *
 *	@return Void, makes a .dot file for making the graph
 *
 *	@brief Output the skip list as a GraphViz DOT document file
 */
void display(SkipList *myList, string fileName)
{
    int i = 1, j;

    int *arr = (int *)malloc(sizeof(int));
    node *point;

    // Start of the list
    point = myList->next;

    ofstream outfile;
    outfile.open( fileName.c_str() );

    // Basic attributes
    outfile<<"strict digraph skiplist {\n\trankdir = LR;\nnode [shape=record, style = filled];";

    // Traverse through the list and write all nodes
    while(point->direct[0]->key != INT_MAX){
        point = point->direct[0];

        arr = (int *)realloc(arr, sizeof(int) * i);
        arr[i-1] = point->key;

        outfile<<"node"<<point->key<<" [label=\"";
        for( j = (point->height)-1; j >= 0; j-- ){
            outfile<<"<l"<<j<<"> level"<<j<<" | ";
        }
        outfile<<point->key;
        outfile<<"\", fillcolor = \"deepskyblue\"];\n";

        //cout<<"Key: "<<point->key<<", Value: "<<point->value<<", Height: "<<point->height<<endl;

        i++;
    }

    //outfile<<"null [label=\"NULL\", shape=plaintext];\n";

    // Read the searc traces
    ifstream traceFile;
    traceFile.open("traces.txt");
    string buff;
    while ( getline(traceFile, buff) ) {
        outfile<<buff;
    }
    traceFile.close();


    i = 1;

    point = myList->next;
    // Traverse through the list and write all linkages
    while(point->direct[0]->key != INT_MAX){
        point = point->direct[0];

        for( j = point->height-1; j >= 0; j-- ){

            outfile<<"node"<<point->key<<":l"<<j<<" -> ";
            if(point->direct[j]->key == INT_MAX)
            outfile<<"null;\n";
            else
            outfile<<"node"<<point->direct[j]->key<<":l"<<j<<";\n";
        }

    }

    // End of file
    outfile<<"\n}\n";

    outfile.close();
}

/**
 *  void skiplist_free(SkipList *list)
 *
 *	@param myList List to manipulated
 *
 *	@return Void
 *
 *	@brief Frees the memory by deleting the list
 */
void skiplist_free(SkipList *list)
{
    node *curr_node = list->next->direct[0];
    while(curr_node != list->next) {
        node *next_node = curr_node->direct[0];
        free(curr_node->direct);
        free(curr_node);
        curr_node = next_node;
    }

    //free(list);
}


/**
 *  \fn SkipList *list_maker(node *start, node *stop)
 *
 *	@param start Starting node for the last
 *  @param stop Last node for the list
 *
 *  @return Pointer to the list created
 *
 *	@brief Creates a list with elements from main list with given starting and last node
 */
SkipList *list_maker(node *start, node *stop)
{
    SkipList *myList = init_list();

    node *temp;
    temp = start;

    // Copy elements and attach
    while(temp != stop){
        inserth(myList, temp->key, temp->value, temp->height);
        temp = temp->direct[0];
    }

    return myList;
}

/**
 *  \fn void reader1( SkipList *myList, string filename)
 *
 *	@param myList List to be manipulated/written to
 *	@param filename Name of the file to be read from
 *
 *	@return Void
 *
 *	@brief Makes the skip list by reading from a file and inserting values in the list
 */
void reader1(SkipList *myList, string filename)
{
    int num,key, height;// number of elements, key, height, max height
    ifstream myfile;

    string buff;// For reading from the file
    char *token, name[256];// For tokenozation and stroing value for the node


    cout<<"Reading from "<<filename<<endl;

     // Open the file
    myfile.open( filename.c_str() );

    // Tokenize and read key and value for each line till the end of file
    while(getline(myfile, buff)){

        token = strtok(const_cast<char*>(buff.c_str()), ",");
        key = atoi(token);

        token = strtok(NULL, ",");
        if(token[0] == '\"')
        {
            token++;
            char *token1 = strtok(NULL, "\"");
            sprintf(name,"%s%s",token,token1);
            //cout<<name<<endl;
        }
        else if(token[1] == '\"')
        {

            token++;
            token++;
            char *token1 = strtok(NULL, "\"");
            sprintf(name,"%s%s",token,token1);
            //cout<<name<<endl;

        }
        else
            sprintf(name, "%s", token);

        token = strtok(NULL, ",");
        height = atoi(token);

        //cout <<"Key: "<< key <<"\t Name: "<< name << "\t\t Height: "<< height <<endl;

        inserth(myList, key, name , height);
        //print_list(myList);

    }

    myfile.close();
}

/**
 * \fn int main(int argc, char *argv[])
 *	@param argc Number of arguments provided by user
 *	@param argv[] Arguments for deciding sleep time for threads and for opening specific LOG file
 *
 *	@return '0' for normal exit, '-1' for abnormal exit/error
 *
 *  @brief Main process, makes the list using input files, perform search opeartions and makes graph
 *
 */
int main( int argc, char *argv[] )
{

    KEYS k;// For using enum (for filtering)
    int findKey;
    int count1, count2, tempI;


    SkipList *myList1;// Master List

    // ref1: starting pointer for Algo List
    // ref2 : end pointer for Algo list
    // tempe1: starting pointer for IR List
    // tempe2 : end pointer for IR list
    node *ref1, *ref2, *tempe1, *tempe2;

    // Read and make lists
    myList1 = init_list();
    reader1(myList1, "MasterList.csv");
    // display(myList1);
    //print_list(myList1);

    SkipList *myList2 = init_list(); // Algo List
    reader1(myList2, "Algo.csv");

    SkipList *myList3 = init_list(); // IR List
    reader1(myList3, "IR.csv");


    /*********PART-1***********/
    /*
        Use find() function to traverse till the desired roll number by iterative searching
    */

    cout<< "******* Part - 1 *******" << endl;

    ofstream outfile1;

    outfile1.open("output1.csv");
    traceFile.open("traces.txt");


    tempI = 0;
    totalCount = 0;

    tempe1 = myList1->next->direct[currLevel];

    // Move iteratively over all key values
    for( int i = 140201; i <= 140399; i++){
        tempe2 = find( tempe1, i);

        if( tempe2->key == i){
            outfile1<< tempe2->key <<","<< tempe2->value <<","<< tempe2->height <<"\n";
            tempI++;
        }
        // Get start pointer for next iteration
        tempe1 = tempe2;
    }


    outfile1.close();
    traceFile.close();
    display(myList1, "myOutput1.dot");
    cout<<"No. of traversal steps: "<<totalCount<<endl;
    cout<< "No. of students satisfying criteria: "<< tempI <<endl;
    /********************************/


    /**********PART-2************/
    /*
        First find students belonging to required departments
        Then check for each element in ALGO list if it is present in IR list
    */
    cout<< "******* Part - 2 *******" << endl;

    ofstream outfile2;
    outfile2.open("output2.csv");
    traceFile.open("traces.txt");

    tempI = 0;
    totalCount = 0;
    currLevel = 0;


    k = DEPT;
    int year = 0;

    // Start values
    tempe1 = find(myList3->next->direct[0], 110300);
    ref1 = find(myList2->next->direct[currLevel], 110300);

    // End values
    ref2 = find(myList2->next->direct[currLevel], 350599);

    currLevel = 0;

    for( node *varNode = ref1; varNode <= ref2; varNode = varNode->direct[0]){

        //cout<<"Comparing: Algo: "<<varNode->key<<" & IR: "<<tempe1->key<<endl;
        year = convert(varNode->key, k);
        if( ( ( year != 3) && ( year != 4) && ( year != 5) )  ){
            continue;
        }

        tempe2 = find( tempe1, varNode->key);

        if( tempe2->key != varNode->key){

            tempI++;
            outfile2<< varNode->key <<","<< varNode->value <<","<< varNode->height <<"\n";
            //cout<< varNode->key <<","<< varNode->value <<","<< varNode->height <<"\n";
            tempe1 = tempe2;
        }
        else
            tempe1 = tempe2->direct[0];
    }


    outfile2.close();
    traceFile.close();
    display(myList1, "myOutput2.dot");
    cout<<"No. of traversal steps: "<<totalCount<<endl;
    cout<< "No. of students satisfying criteria: "<< tempI <<endl;
    /********************************/


    /**********PART-3************/
    /*
        Find elements in each other turn by turn till the end
    */
    cout<< "******* Part - 3 *******" << endl;

    ofstream outfile3;
    outfile3.open("output3.csv");
    traceFile.open("traces.txt");

    tempI = 0;
    totalCount = 0;
    currLevel = 0;

    int buff;
    ref1 = myList2->next->direct[0];
    tempe1 = myList3->next->direct[currLevel];

    while( 1 ){

        //cout<<"Keys: "<<ref1->key<<" & "<<tempe1->key<<endl;

        if( (ref1->key == INT_MAX) || (tempe1->key == INT_MAX))
            break;

        if( ref1->key > tempe1->key){

            tempe2 = find(tempe1, ref1->key);
            //cout<<"2 returned: "<<tempe2->key<<endl;
            if( tempe2->key == INT_MAX)
                break;

            if( tempe2-> key == ref1->key){
                tempI++;
                outfile3<< tempe2->key <<","<< tempe2->value <<","<< tempe2->height <<"\n";
                tempe1 = tempe2->direct[0];
            }
            else
                tempe1 = tempe2;
        }

        else{
            ref2 = find(ref1, tempe1->key);
            //cout<<"1 returned: "<<ref2->key<<endl;
            if( ref2->key == INT_MAX)
                break;

            if( ref2-> key == tempe1->key){
                tempI++;
                outfile3<< ref2->key <<","<< ref2->value <<","<< ref2->height <<"\n";
                ref1 = ref2->direct[0];
            }
            else
                ref1 = ref2;
        }


    }

    outfile3.close();
    traceFile.close();
    display(myList1, "myOutput3.dot");
    cout<<"No. of traversal steps: "<<totalCount<<endl;
    cout<< "No. of students satisfying criteria: "<< tempI <<endl;
    /********************************/




    /**********PART-4************/
    /*
        Iterate both lists and compare them to move forward and writing output
    */
    cout<< "******* Part - 4 *******" << endl;

    ofstream outfile4;
    outfile4.open("output4.csv");
    traceFile.open("traces.txt");

    int i =1;

    tempI = 0;
    totalCount = 0;
    currLevel = 0;

    ref1 = find(myList2->next->direct[currLevel], 110101);
    tempe1 = find(myList3->next->direct[currLevel], 110101);


    while( ( ref1->key < INT_MAX) || ( tempe1->key < INT_MAX) ){


        if( ref1->key ==  tempe1->key){

            outfile4<< ref1->key <<" "<< ref1->value  <<" "<< ref1->height << endl;
            totalCount++;

            if( ref1->key < INT_MAX){
                traceFile<<"\tnode"<< ref1->key <<":l0 -> node"<< ref1->direct[0]->key<<":l0 " <<" [color=\"0.002 0.999 0.999\"];\n";
                tempI++;
                ref1 = ref1->direct[0];
            }
            if( tempe1->key < INT_MAX){
                tempe1 = tempe1->direct[0];
                tempI++;
            }
        }
        else if (ref1->key < tempe1->key){

            outfile4<< ref1->key <<" " << ref1->value <<" "<< ref1->height << endl;
            totalCount++;

            if( ref1->key < INT_MAX){
                traceFile<<"\tnode"<< ref1->key <<":l0 -> node"<< ref1->direct[0]->key<<":l0 " <<" [color=\"0.002 0.999 0.999\"];\n";
                tempI++;
                ref1 = ref1->direct[0];
            }
        }

        else if  (tempe1->key < ref1->key){

            outfile4<< tempe1->key <<" " << tempe1->value <<" " << tempe1->height << endl;
            totalCount++;

            if( tempe1->key < INT_MAX){
                tempe1 = tempe1->direct[0];
                tempI++;
            }

        }

    }
    outfile4.close();
    traceFile.close();
    display(myList1, "myOutput4.dot");
    cout<<"No. of traversal steps: "<< tempI <<endl;
    cout<< "No. of students satisfying criteria: "<< totalCount <<endl;

    /********************************/



    return 0;
}
