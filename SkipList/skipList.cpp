/**
 *	@file skipList.cpp
 *	@author Vishnu Dutt Sharma 12EC35018
 *	
 *	@param None
 *	@return None, Prints number of seraches and makes graph
 *	
 *	@brief Implements Skip List and performs some search operations and makes graph for comparison
 *		
 *		This module reads three files. From the first file it makes a Skip List with pre-decided height 
 *		and makes a graph for it. From the second file, it makes a Skip List with probablistically calculated heights and performs
 *		search by reading inputs from the third file. Then it makes a graph of Probability vs Search operations
 */


#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sys/time.h>
#include <limits.h>

using namespace std;



/**
 * 	\def SKIP_MAX_HEIGHT Maximum height in a Skip List
 * 	\def INT_MAX Maximum key value that a list can have, also the value at the end of the list
 */
#define SKIP_MAX_HEIGHT 16


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
 * \var maxHeight MAximum height in the skip list
 */
int maxHeight;


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
 *  \fn int insertp(SkipList *myList, int key, char * val, float p)
 *
 *  @param myList Skip List where insertion is to be performed
 *  @param key Key of the element to be inserted
 *  @param val Value of the lement to be inserted
 *  @param p Prabability to decide the height of the element
 *
 *	@return height of the inserted element
 *	
 *	@brief	Function to insert an element/node in the list with probabilistic height
 * 		Insert   a   node   into   the   skip   list   with   the   key   and   value   as   given   using p
 * 		as   the   probability   for   calculating   node   height.   
 */
int insertp(SkipList *myList, int key, char * val, float p)
{
    float r; // Variable for calculating height
    node *point, *var, *pArray[SKIP_MAX_HEIGHT]; // Iterator Nodes and nod earrays

	// Temporary node
    var = (node *)malloc( sizeof(node));


    // Initial height
    int height = 2;
    
    // Calculate the height
    while(( height < SKIP_MAX_HEIGHT) ){
        r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (r >= p)
            break;
        height++;
    }


    // Starting the list
    point = myList->next;

    for(int i = SKIP_MAX_HEIGHT-1; i >= 0; i--){
        while( point->direct[i]->key < key){
        	// Kep going while key of iterator value is lesser
            point = point->direct[i];
        }
        // If key value is greater or equal, Assign it to the variable 
        pArray[i] = point;
    }
    point = point->direct[0];



    // Assign values
    var->key = key;
    sprintf(var->value, "%s", val);
    var->height = height;
    var->direct = (node **)malloc( sizeof(node) * height);
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
    for(int i = 0; i < height; i++){
        var->direct[i] = pArray[i]->direct[i];
        if(point->key == key)
            pArray[i] = var;
        else
            pArray[i]->direct[i] = var;

    }

    // Return the height of the element
    return height;

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
 * \fn int find(SkipList *myList, int key)
 *  
 * @param myList Skip List in which search is to be performed
 * @param key Key of the elemnt to be searched
 *
 * @return Number of steps taken in searching, if search is failed, it returns -1
 * 
 * @brief 
 *  Find in the SkipList the value corresponding to the key provided and returns the number of steps taken
 */
int find(SkipList *myList, int key)
{
    int i, step=0;
    node *point;

    // Start of the list
    point = myList->next;

    // Start from top
    //for(i = maxHeight-1; i >= 0; i--){
    for(i = SKIP_MAX_HEIGHT-1; i >= 0; i--){

    	// Keep moving till the iterator key is smaller than search key
        while( (point->direct[i]->key < key)){
            //cout<<point->key<<"-> "<<point->direct[i]->key<<","<<i<<endl;

            point = point->direct[i];
            step++;

            if (point->key == INT_MAX){
                    break;
            }

        }

        // Return if key is found
        if(point->direct[i]->key == key){
            //cout<<"Found: Key: "<<point->direct[i]->key<<" Value: "<<point->direct[i]->value<<" Steps: "<<step<<endl;
            return step;
        }
        //cout<<"minus i: "<<i<<endl;
        step++;
    }
    //cout<<"Key: "<<key<<" not found in the list"<<endl;
    return -1;
}



/**
 *  \fn void display(SkipList *myList)
 *  
 *	@param myList List to be displayed
 *
 *	@return Void, makes a .dot file for making the graph
 * 
 *	@brief Output the skip list as a GraphViz DOT document file
 */
void display(SkipList *myList)
{
    int i = 1, j; //Iterators

    int *arr = (int *)malloc(sizeof(int));
    node *point;

    // Start of the list
    point = myList->next;

    ofstream outfile;
    outfile.open("myOutput.dot");

    // Basic attributes
    outfile<<"digraph skiplist {\n\trankdir = LR;\nnode [shape=record, style = filled];";


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

    cout<<"Display Done"<<endl;
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

}


/**
 *  \fn void reader2( SkipList *myList, string filename, float p)
 *  
 *	@param myList List to be manipulated/written to
 *	@param filename Name of the file to be read from
 *	@param p Probability to determine heights of the nodes
 *
 *	@return Void
 * 
 *	@brief Makes the skip list by reading from a file and inserting values in the list
 */
void reader2( SkipList *myList, string filename, float p)
{
    int num, key, currHt; // number of elements, key, current height	
    
    ifstream myfile;
   
    string buff;	// For reading from the file
    
    char *token, name[256];	// For tokenozation and stroing value for the node

    // Open the file
    myfile.open( filename.c_str() );

    // Read the number of elements
    myfile >> num;

    // Start reading the content
    getline(myfile, buff);


    // Tokenize and read key and value for each line
    for(int i = 0; i < num; i++){
        getline(myfile, buff);

        token = strtok(const_cast<char*>(buff.c_str()), ",");
        key = atoi(token);

        token = strtok(NULL, ",");
        sprintf(name, "%s", token);

        //cout <<"Key: "<< key <<" Name: "<< name << endl;

        currHt = insertp(myList, key, name , p);

        // if( currHt > maxHeight)
        // 	maxHeight = currHt;
    }

    myfile.close();
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
    int num, key, height, maxHt;// number of elements, key, height, max height
   
    ifstream myfile;
    
    string buff; // For reading from the file
    char *token, name[256]; // For tokenozation and stroing value for the node


    // Open the file
    myfile.open( filename.c_str() );

	// Read the number of elements
    myfile >> num;

    // Start reading the content
    getline(myfile, buff);

    // Tokenize and read key and value for each line
    for(int i = 0; i < num; i++){
        getline(myfile, buff);

        token = strtok(const_cast<char*>(buff.c_str()), ",");
        key = atoi(token);

        token = strtok(NULL, ",");
        sprintf(name, "%s", token);

        token = strtok(NULL, ",");
        height = atoi(token);

        // if(height > maxHeight)
        // 	maxHeight = height;

        inserth(myList, key, name , height);
        
        //cout <<"Key: "<< key <<"\t Name: "<< name << "\t\t Height: "<< height <<endl;
    }
    cout<<" File Reading Done \n";
    myfile.close();
}


/**
 * \fn int main(int argc, char *argv[])
 *	@param argc Number of arguments provided by user
 *	@param argv[] Arguments for deciding sleep time for threads and for opening specific LOG file
 *
 *	@return '0' for normal exit, '-1' for abnormal exit/error
 *
 *  @brief Main process, makes the list using input files and plots graphs
 *
 */
int main( int argc, char *argv[] )
{
    
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

    //maxHeight = 1;
    maxHeight = SKIP_MAX_HEIGHT;

    float prob[5] = {0.0675, 0.125, 0.25, 0.3679, 0.5};

    unsigned long int times[5] = {0, 0, 0, 0, 0};
    int num, result, *arr;

	/*          Part 1          */

     SkipList *myList1;
     myList1 = init_list();
     reader1(myList1, "input1.txt");

     display(myList1);




	// /*          Part 2          */
 //     // Probablistic heights and searching
 //    ifstream inp;

 //    inp.open("search.txt");
 //    inp >> num;

 //    //maxHeight = 1;
 //    maxHeight = SKIP_MAX_HEIGHT;

 //    arr = (int *)malloc( sizeof(int) * num);

 //    for( int i = 0; i < num; i++){
 //        inp >> arr[i];
 //    }

 //    inp.close();

 //    SkipList *myList2;

 //    for(int i = 0; i < 5; i++){

 //        myList2 = init_list();
 //        reader2(myList2, "input2.txt", prob[i]);

 //        cout<<"For "<<prob[i]<<endl;

 //        for( int j = 0; j < num; j++){
 //            result = find(myList2, arr[j]);
 //            //cout<<"\t Search for "<< arr[j]<< " : "<< result <<endl;
 //            times[i] += result;
 //        }
 //        //display( myList2 );
 //        skiplist_free(myList2);
 //    }


 //    // Plotting
 //    fprintf(gnuplotPipe, "plot '-' \n");
 //    int i;

 //    for (int i = 0; i < 5; i++)
 //    {
 //        fprintf(gnuplotPipe, "%f %lu \n", prob[i], times[i]);
 //    }

 //    fprintf(gnuplotPipe, "e");


    return 0;
}
